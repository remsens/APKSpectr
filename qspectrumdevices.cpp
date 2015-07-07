#include "qspectrumdevices.h"

QSpectrumDevices::QSpectrumDevices(QObject *parent)
{
    serial_port = new QSerialPort("COM7");
    serial_port->setBaudRate(QSerialPort::Baud9600);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setStopBits(QSerialPort::OneStop);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->open(QIODevice::ReadOnly);
    info_source = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::RealTimeMode);
    connect(info_source , SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(onGpsRecived(QGeoPositionInfo)));
    info_source->setDevice(serial_port);
    info_source->startTimer(500);
    info_source ->startUpdates();

    device_count = 0;
    device_position = "NAN";
}

void QSpectrumDevices::addDevice(const FTDI_NUM device)
{
    QString device_serial(FTDI_SERIAL_NUM[device]);
    QString device_name(DEVICE_NAME[device]);
    QSpecModule      *s_dev_module = new QSpecModule(device_serial);
    if (s_dev_module->getDeviceState() == D_OK)
    {
        QDeviceDataModel *s_dev_model  = new QDeviceDataModel(this);
        QThread          *s_dev_thread = new QThread();

        s_dev_module->moveToThread(s_dev_thread);
        connect(s_dev_thread, SIGNAL(finished()), s_dev_module, SLOT(deleteLater()));
        connect(s_dev_thread, SIGNAL(started()), s_dev_module, SLOT(initDevice()));
        connect(this, SIGNAL(makeSinglShot(int)), s_dev_module, SLOT(singleShot(int)));
        connect(s_dev_module, SIGNAL(onCompletePass(SpectrumData*)), s_dev_model, SLOT(addSpectrumData(SpectrumData*)));
        connect(info_source, SIGNAL(positionUpdated(QGeoPositionInfo)), s_dev_model, SLOT(onGpsRecived(QGeoPositionInfo)));




        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        device_threads.append(s_dev_thread);
        device_modules.append(s_dev_module);
        device_views.append(s_dev_model);
        device_names.append(device_name);
        endInsertRows();
        device_count++;
        s_dev_thread->start();
    }
}

int QSpectrumDevices::rowCount(const QModelIndex &parent) const
{
    return device_count;
}

QVariant QSpectrumDevices::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 or index.row() >= device_count)
    {
        return QVariant();
    }
    if (role == deviceRole)
    {
        return QVariant::fromValue(device_views[index.row()]);
    }
    if (role == deviceName)
    {
        return QVariant::fromValue(device_names[index.row()]);
    }
    if (role == devicePosition)
    {
        return QVariant::fromValue(device_position);
    }
    return QVariant();

}


void QSpectrumDevices::onShotButPressed(QVariant exposition)
{
    if(exposition.isValid()) emit makeSinglShot(exposition.toInt());
    else emit makeSinglShot(0);
}

void QSpectrumDevices::onSeriesButPressed(QVariant shotCount, QVariant exposition)
{
    for(int i=0; i < shotCount.toInt(); i++)
    {
        if(exposition.isValid()) emit makeSinglShot(exposition.toInt());
        else emit makeSinglShot(0);
    }
}

void QSpectrumDevices::onSavePressed(const QVariant fileName)
{
    if (device_count > 0)
    {
        QFile *spectrum_file  = new QFile();
        spectrum_file->setFileName("./" + QDateTime::currentDateTime().toString("yyyy_mm_dd_HH_mm_ss")+ ".spec");
          spectrum_file->open(QIODevice::WriteOnly);
          spectrum_file->write("SPECTRUM_FILE");
          for (int i=0; i < device_count; i++)
          {
              QVector<QDeviceData*> device_data = device_views[i]->getSpectrumData();
              SpectrumFileRecord file_record(device_names[i].toStdString().c_str(),
                                                                         device_data.count());
              spectrum_file->write((const char*)(&file_record), sizeof(SpectrumFileRecord));
              for (int j=0; j < device_data.count(); j++)
              {

                  const char* data = (const char*)(device_data[j]->spectrumRawData());
                  const char* position = (const char*)(device_data[j]->spectrumPosition());
                  spectrum_file->write(position, sizeof(LLACoordinate));
                  spectrum_file->write(device_data[j]->spectrumDateTime().toString().toStdString().c_str());
                  spectrum_file->write(data, sizeof(SpectrumData));
                  spectrum_file->write("EOS", 3);
              }
              spectrum_file->write("EOD", 3);
          }
          spectrum_file->close();
    }

}

QHash<int, QByteArray> QSpectrumDevices::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[deviceRole] = "deviceRole";
    roles[deviceName] = "deviceName";
    roles[devicePosition] = "devicePosition";
    return roles;
}



void QSpectrumDevices::onGpsRecived(const QGeoPositionInfo &geoPosition)
{
    device_position = "Широта: " + QString::number(geoPosition.coordinate().latitude())
                    + " Долгота: " + QString::number(geoPosition.coordinate().longitude()) ;
    //qDebug() << device_position;
}

