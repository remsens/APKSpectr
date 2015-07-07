#ifndef QSPECTRUMMODEL_H
#define QSPECTRUMMODEL_H
#include <QAbstractListModel>
#include <QMap>
#include <vector>


class QSpectrumModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QSpectrumModel(QObject *parent = 0);
    enum spectrumRoles {xvalue = Qt::UserRole + 1,
                        yvalue};
    void addData(std::vector<short> &spectrumData);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QMap<int,int>      spectrum_pixels;
    std::vector<short> visible_pixels;
public slots:
    int getYat(int i);
    QVariant getYData();


};

#endif // QSPECTRUMMODEL_H
