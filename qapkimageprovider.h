#ifndef QAPKIMAGEPROVIDER_H
#define QAPKIMAGEPROVIDER_H
#include <QQuickImageProvider>
#include <QPixmap>
#include <vector>
#include "qapkimageloader.h"



class QAPKImageProvider : public QQuickImageProvider
{

public:
    explicit QAPKImageProvider();
    ~QAPKImageProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
private:
    std::vector<QPixmap> image_data;
    QNetworkAccessManager network_acces_manager;
};

#endif // QAPKIMAGEPROVIDER_H
