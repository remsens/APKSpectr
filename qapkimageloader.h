#ifndef QAPKIMAGELOADER_H
#define QAPKIMAGELOADER_H
#include <QObject>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QNetworkReply>
#include <QPixmap>
#include <QSignalSpy>

class QAPKImageLoader: public QObject
{
    Q_OBJECT
public:
    QAPKImageLoader();
    ~QAPKImageLoader();
    QPixmap getUrlPixmap(QUrl pixmapUrl);
public slots:
    void onDownloaded(QNetworkReply* reply);
signals:
    void onProcessed();
private:
    QPixmap pixmap;
    QNetworkAccessManager na_manager;
};

#endif // QAPKIMAGELOADER_H
