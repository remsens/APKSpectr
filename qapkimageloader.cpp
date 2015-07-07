#include "qapkimageloader.h"

QAPKImageLoader::QAPKImageLoader():QObject()
{
    connect(&na_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloaded(QNetworkReply*)));
}

QAPKImageLoader::~QAPKImageLoader()
{

}

QPixmap QAPKImageLoader::getUrlPixmap(QUrl pixmapUrl)
{
    QNetworkRequest request;
    request.setUrl(pixmapUrl);
    na_manager.get(request);
    QSignalSpy sig1(this, SIGNAL(onProcessed()));
    sig1.wait(10000);
    return pixmap;

}

void QAPKImageLoader::onDownloaded(QNetworkReply *reply)
{
    QByteArray data_array = reply->readAll();
    pixmap.loadFromData(data_array);
    emit onProcessed();
}

