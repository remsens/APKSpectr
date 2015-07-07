#include "qapkengine.h"

QApkEngine::QApkEngine()
{

}

QApkEngine::~QApkEngine()
{

}

void QApkEngine::load(const QString &filePath)
{
    devices = new QSpectrumDevices();
    devices->addDevice(FTDI_DMS);
    devices->addDevice(FTDI_SSP);
    devices->addDevice(FTDI_FSRA);
    devices->addDevice(FTDI_FSRB);

    rootContext()->setContextProperty("spectrum_model", devices);
    image_provider = new QAPKImageProvider();
    addImageProvider("apkimage",image_provider);
    QQmlApplicationEngine::load(filePath);
}

void QApkEngine::addDevice(QString deviceSerial)
{

}

void QApkEngine::onSpectrumData(SpectrumData *spectrumData)
{

}

void QApkEngine::onSaveSpectrumFile()
{
//    QString file_name = QFileDialog::getSaveFileName(0, "Saving spectrum", "","Spectrum files (*.spec)");
//    current_file_name = file_name;
//    QFile spectrum_file(file_name);
//    spectrum_file.open(QIODevice::ReadWrite);
//    spectrum_file.write("SPECTRUM_FILE");
//    for (int i=0; i< spectrum_data.size() - 1; i++)
//    {
//        spectrum_file.write((char*)&spectrum_data[i], sizeof(SpectrumData));
//    }
//    spectrum_file.close();
    //    qDebug() << file_name;
}

void QApkEngine::onGpsRecived(const QGeoPositionInfo &geoPosition)
{
    qDebug() << "geoooooooooo";
}

