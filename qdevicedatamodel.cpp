#include "qdevicedatamodel.h"

QDeviceDataModel::QDeviceDataModel(QObject *parent):QAbstractListModel(parent)
{

}

QVariant QDeviceDataModel::data(const QModelIndex &index, int role) const
{

    if ((index.row() < 0) || (index.row() >= spectrum_data.count()))
        return QVariant();

    QDeviceData* spectrum = spectrum_data[index.row()];
    if (role == spectrumExpo)
    {
        return spectrum->spectrumExposition();
    }
    if (role == spectrumTime)
    {
        return spectrum->spectrumDateTime().toString();
    }
    if (role == spectrumData)
    {
        //return QVariant();
      QSpectrumModel* sModel = spectrum->spectrumData();
      return QVariant::fromValue(sModel);

    }
    if (role == spectrumPosition)
    {
        LLACoordinate *spectrum_coord = spectrum->spectrumPosition();

        return QVariant::fromValue("Широта:" + QString::number(spectrum_coord->lat) +
                                   " Долгота:" + QString::number(spectrum_coord->longi));
    }

    return QVariant();
}

int QDeviceDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return spectrum_data.count();
}

void QDeviceDataModel::addSpectrumData(QDeviceData *data)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    spectrum_data.push_back(data);
    endInsertRows();
}

void QDeviceDataModel::addSpectrumData(SpectrumData *spectrumData)
{

    QDeviceData *d_data = new QDeviceData(spectrumData, geo_position);
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    spectrum_data.push_back(d_data);
    endInsertRows();
    //qDebug() << geo_position.coordinate().longitude() << " " << geo_position.coordinate().latitude();
}

void QDeviceDataModel::onGpsRecived(const QGeoPositionInfo &geoPosition)
{
    geo_position = geoPosition;
    //qDebug() << "gps recived";
}

QVector<QDeviceData*> QDeviceDataModel::getSpectrumData()
{
    return spectrum_data;
}

QHash<int, QByteArray> QDeviceDataModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[spectrumData] = "spectrumData";
    roles[spectrumExpo] = "spectrumExpo";
    roles[spectrumTime] = "spectrumTime";
    roles[spectrumPosition] = "spectrumPosition";
    return roles;
}

