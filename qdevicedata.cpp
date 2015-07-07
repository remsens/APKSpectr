#include "qdevicedata.h"

QDeviceData::QDeviceData(QObject *parent) : QObject(parent)
{

}

QDeviceData::QDeviceData(SpectrumData *data, const QGeoPositionInfo &geoPosition, QObject *parent):QDeviceData(parent)
{
    spectrum_pixels = new QSpectrumModel();
    std::vector<short> s_data(data->spectrum, data->spectrum + 3648);
    spectrum_pixels->addData(s_data);
    spectrum_exposition = data->header.exposition;
    spectrum_date_time  = QDateTime::currentDateTime();
    geo_position = geoPosition;
    spectrum_data = data;

}

QDeviceData::~QDeviceData()
{

}

QSpectrumModel* QDeviceData::spectrumData() const
{
    return spectrum_pixels;
}

SpectrumData *QDeviceData::spectrumRawData() const
{
    return spectrum_data;
}

int QDeviceData::spectrumExposition()
{
    return spectrum_exposition;
}

QDateTime QDeviceData::spectrumDateTime()
{
    return spectrum_date_time;
}

LLACoordinate *QDeviceData::spectrumPosition() const
{
    return new LLACoordinate(geo_position.coordinate().longitude(), geo_position.coordinate().latitude(),0);

   // return "Широта: " + QString::number(geo_position.coordinate().latitude())
   //         + " Долгота: " + QString::number(geo_position.coordinate().longitude()) ;
}

