#include "qspectrummodel.h"

QSpectrumModel::QSpectrumModel(QObject* parent): QAbstractListModel(parent)
{

}

void QSpectrumModel::addData(std::vector<short> &spectrumData)
{

    for (int i=0; i < spectrumData.size(); i++)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        spectrum_pixels.insert(i, spectrumData.at(i));
        endInsertRows();
    }

}

QVariant QSpectrumModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > spectrum_pixels.count())
    {return QVariant();}
    if (role == xvalue)
    {
        return spectrum_pixels.keys().at(index.row());
    } else if (role == yvalue)
    {
       return spectrum_pixels.values().at(index.row());
    }
}

int QSpectrumModel::rowCount(const QModelIndex &parent) const
{
    return spectrum_pixels.count();
}

int QSpectrumModel::getYat(int i)
{
    return spectrum_pixels.values().at(i);
}

QVariant QSpectrumModel::getYData()
{
   return QVariant::fromValue(spectrum_pixels.values());
}


QHash<int, QByteArray> QSpectrumModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[xvalue] = "xValue";
    roles[yvalue] = "yValue";
    return roles;
}
