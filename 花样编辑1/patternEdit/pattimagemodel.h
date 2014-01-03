#ifndef PATTIMAGEMODEL_H
#define PATTIMAGEMODEL_H

#include <QtCore>
#include <QAbstractTableModel>
#include <QPixmap>

class pattImageModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    pattImageModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags( const QModelIndex &) const;
    bool setData(const QModelIndex &index, const QVariant &var,
                      int role = Qt::EditRole);

public:
    QString itemColorStr[25][50]; // 存储色码字符(‘0’-‘F’，用于编辑)
    uchar itemColorData[25][50]; // 存储色码的二进制数据
    QPixmap itemDisplay[2][16]; // 存储色码的显示图片
    bool enableRevColor;

private:
    uchar itemRevColor; // 反色

};

#endif // PATTIMAGEMODEL_H
