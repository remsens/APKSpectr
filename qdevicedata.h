#ifndef QDEVICEDATA_H
#define QDEVICEDATA_H

#include <QObject>
#include <QDateTime>
#include <QVector>
#include <QGeoPositionInfo>
#include "qspectrummodel.h"
#include "qspecmodule.h"

struct LLACoordinate
{
    double longi;
    double lat;
    double alt;
    LLACoordinate(const double& longitude, const double& latitude, const double& altitude):
        longi(longitude), lat(latitude), alt(altitude){};
};

class QDeviceData : public QObject
{
    Q_OBJECT
    QSpectrumModel* spectrum_pixels;
    int             spectrum_exposition;
    QDateTime       spectrum_date_time;
    SpectrumData*   spectrum_data;
    QGeoPositionInfo geo_position;

public:
    explicit QDeviceData(QObject *parent = 0);
    QDeviceData(SpectrumData *data, const QGeoPositionInfo &geoPosition = QGeoPositionInfo(),
                QObject *parent = 0);
    ~QDeviceData();

    QSpectrumModel *spectrumData() const;
    SpectrumData  *spectrumRawData() const;
    int            spectrumExposition();
    QDateTime      spectrumDateTime();
    LLACoordinate *spectrumPosition() const;

};

#endif // QDEVICEDATA_H
