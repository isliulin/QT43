#include <QtGui>
#include <QtCore>
#include "pattimagemodel.h"

pattImageModel::pattImageModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    for (int i = 0; i < 16; i ++)
    {
        QString name;

        name.sprintf(":/colorImage/%d.bmp", i);
        itemDisplay[0][i].load(name); // 正色图片
        name.sprintf(":/colorImage/%d.bmp", 100+i);
        itemDisplay[1][i].load(name); // 反色图片
    }
    enableRevColor = false;
    itemRevColor = 0;
    memset(itemColorData, 0, sizeof(itemColorData));
}

int pattImageModel::rowCount(const QModelIndex & /* parent */) const
{
    return 25;
}

int pattImageModel::columnCount(const QModelIndex & /* parent */) const
{
    return 50;
}

QVariant pattImageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::EditRole)
    {
        return itemColorStr[index.row()][index.column()];
    }

    uchar value = itemColorData[index.row()][index.column()];
    if (role == Qt::DisplayRole)
    {
        return itemDisplay[0][value];
    }
    else if (role == Qt::UserRole)
    {
        return itemDisplay[itemRevColor][value];
    }

    return QVariant();
}

QVariant pattImageModel::headerData(int /* section */,
                                Qt::Orientation /* orientation */,
                                int role) const
{

    return QVariant();
}

Qt::ItemFlags pattImageModel::flags( const QModelIndex &) const
{
    return (Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
}

bool pattImageModel::setData(const QModelIndex &index, const QVariant &var, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        QString input;
        uchar data;
        bool ok;

        input = var.toString();
        data = input.toInt(&ok, 16);

        if (ok)
        {
            itemColorData[index.row()][index.column()] = data;
            itemColorStr[index.row()][index.column()] = input;

            emit dataChanged(index, index);
            return true;
        }
    }
    else if (index.isValid() && role == Qt::UserRole)
    {
        if (enableRevColor)
        {
            // 切换反色图片
            itemRevColor = var.toInt();
            emit dataChanged(index, index);
            enableRevColor = false;
            return true;
        }
    }
    else if (index.isValid() && role == Qt::UserRole + 1)
    {
        enableRevColor = var.toBool();
        return true;
    }

    return false;
}
