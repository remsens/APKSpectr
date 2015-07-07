#ifndef QSPECTRUMDEVICES_H
#define QSPECTRUMDEVICES_H
#include <QObject>
#include <QAbstractListModel>
#include <QThread>
#include <iterator>
#include <vector>
#include <QSerialPort>
#include <QNmeaPositionInfoSource>
#include <QGeoPositionInfo>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <iostream>
#include "qspecmodule.h"
#include "qdevicedatamodel.h"

enum FTDI_NUM {
    FTDI_SSP, FTDI_DMS, FTDI_FSRA, FTDI_FSRB, FTDI_FSR0
};
const QString DEVICE_NAME[5] = {"ССП", "ДМС", "ФСР1","ФСР2","ФСР0"};

const QString FTDI_SERIAL_NUM[5] = {"A703OLZV", "A703OLZY", "A703OLZU", "A703OLZX", "FSR0"};

struct SpectrumFileRecord
{
    char*   device_name[80];
    int     spectrum_count;
    SpectrumFileRecord(const char deviceName[80], const int spectrumCount)
        {
        memcpy(&device_name, deviceName, 80);
        spectrum_count = spectrumCount;
    }

};


class QSpectrumDevices : public QAbstractListModel
{
    Q_OBJECT
    int device_count;
    QSerialPort             *serial_port;
    QNmeaPositionInfoSource *info_source;
    QString                 device_position;
public:
    enum deviceRoles {devicePosition = Qt::UserRole + 1, deviceName = Qt::UserRole +2 , deviceRole};
    explicit QSpectrumDevices(QObject *parent = 0);
    void addDevice(const FTDI_NUM device);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

public slots:
    void onShotButPressed(QVariant exposition);
    void onSeriesButPressed(QVariant shotCount, QVariant exposition);
    void onSavePressed(const QVariant fileName);
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QVector<QThread*>     device_threads;
    QVector<QSpecModule*> device_modules;
    QVector<QDeviceDataModel*> device_views;
    QVector<QString>      device_names;
private slots:
    void onGpsRecived(const QGeoPositionInfo &geoPosition);
signals:
    void deviceSend(QVector<int> &);
    void makeSinglShot(int exposition);
};

#endif // QSPECTRUMDEVICES_H
