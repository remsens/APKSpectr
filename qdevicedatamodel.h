#ifndef QDEVICEDATAMODEL_H
#define QDEVICEDATAMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include "qdevicedata.h"
#include "qspectrummodel.h"
#include "qspecmodule.h"
#include <QDebug>
#include <QNmeaPositionInfoSource>
#include <QGeoPositionInfo>


class QDeviceDataModel : public QAbstractListModel
{
    Q_OBJECT
    QGeoPositionInfo geo_position;
public:
    explicit QDeviceDataModel(QObject *parent = 0);
    enum deviceDataRoles {spectrumPosition = Qt::UserRole + 1,
                          spectrumExpo = Qt::UserRole +2,
                          spectrumTime = Qt::UserRole +3,
                          spectrumData};

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

public slots:
    void    addSpectrumData(QDeviceData *data);
    void    addSpectrumData(SpectrumData* spectrumData);
    void    onGpsRecived(const QGeoPositionInfo &geoPosition);
    QVector<QDeviceData*> getSpectrumData();
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QVector<QDeviceData*> spectrum_data;
};

#endif // QDEVICEDATAMODEL_H
