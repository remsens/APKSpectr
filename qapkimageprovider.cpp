#include "qapkimageprovider.h"

QAPKImageProvider::QAPKImageProvider():QQuickImageProvider(QQuickImageProvider::Pixmap)
{

}

QAPKImageProvider::~QAPKImageProvider()
{

}

QPixmap QAPKImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    int image_id = id.toInt();
    if (image_id < image_data.size())
    {
        return image_data[image_id];
    }else
    {
        QAPKImageLoader loader;
//#ifdef QT_DEBUG
//        QPixmap pixmap = loader.getUrlPixmap(QUrl("http://192.168.1.28:8080/photoaf.jpg"));
//#else
        QPixmap pixmap = loader.getUrlPixmap(QUrl("http://192.168.43.1:8080/photo.jpg"));
//#endif
        image_data.push_back(pixmap);
        return image_data[image_id];
    }
}



