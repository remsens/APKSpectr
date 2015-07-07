#ifndef QAPKENGINE_H
#define QAPKENGINE_H

#include <QQmlApplicationEngine>
#include <QThread>
#include <QQmlContext>
#include <QList>
#include <QFile>
#include <QFileDialog>
#include "qspecmodule.h"
#include "qspectrumdevices.h"
#include <QVector>
#include <vector>
#include <ftd2xx.h>

#include "qapkimageprovider.h"





using namespace std;


class QApkEngine : public QQmlApplicationEngine
{
    Q_OBJECT
    QList<QObject*>        spectrum_objects;
    int                    spectrum_count;
    QString                current_file_name;
    QSpectrumDevices*       devices;
    QNmeaPositionInfoSource *infoSource;
    QSerialPort             *sPort;
    QAPKImageProvider       *image_provider;

public:
    QApkEngine();
    QThread     s_thread;
    QSpecModule *s_module;
    ~QApkEngine();

    void load(const QString &filePath);
    void addDevice(QString deviceSerial);

signals:
    deviceSend(QVector<int> &);

public slots:
    void onSpectrumData(SpectrumData* spectrumData);
    void onSaveSpectrumFile();
    void onGpsRecived(const QGeoPositionInfo& geoPosition);

};

#endif // QAPKENGINE_H
