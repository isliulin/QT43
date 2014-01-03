#include "pattworker.h"
#include "pattern.h"
#include <QFile>

pattWorker::pattWorker(QObject *parent) :
    QObject(parent)
{
    connect(&thread, SIGNAL(started()), this, SLOT(loadPattern()));
    this->moveToThread(&thread);

    // 视图左下角在花样中的绝对位置
    originIndex.row = 0;
    originIndex.column = 0;
    // 花样大小信息
    pattWidth = 0;
    pattHeight = 0;
}

void pattWorker::loadPattern(void) //TODO
{
    QFile file;

    file.setFileName("7x7.dat");
    if (file.open(QIODevice::ReadOnly))
    {
        baPatt = file.readAll();
        file.close();
    }
}

void pattWorker::viewMove(QTableView *tbPatt, int dir, int type)
{
    if (baPatt.isEmpty())
        return;

    QAbstractItemModel *model = tbPatt->model();
    tbPatt->setUpdatesEnabled(0);

    switch (dir)
    {
    case 1:
        break;
    case 2:
        break;
    case 3: // 左方向键
        if (originIndex.column == 0)
            break;

        if (type == 0) // 移动一格
        {
            setPage(model, originIndex.row, originIndex.column - 1);
        }
        else if (type == 1) // 翻页
        {
            setPage(model, originIndex.row, originIndex.column - 50);
        }
        break;
    case 4: // 右方向键
        if (type == 0) // 移动一格
        {
            setPage(model, originIndex.row, originIndex.column + 1);
        }
        else if (type == 1) // 翻页
        {
            setPage(model, originIndex.row, originIndex.column + 50);
        }
        break;
    }

    tbPatt->setUpdatesEnabled(1);
}

void pattWorker::setPage(QAbstractItemModel *model, int destRow, int destColumn)
{
    int num = 0;
    int width, height;
    int pos;
    bool rightFirst = false;

    patt_dat_t* patt = (patt_dat_t*)baPatt.data();
    width = patt->width;
    height = patt->height;
    // 到达宽边的末尾
    if ((destColumn + 50) > width)
        return;
    // 一个字节包含两个宽度的花样
    if (width & 1)
        width ++;
    // 当左下角在奇数列时应该先显示一个字节的低4位
    if (destColumn & 1)
        rightFirst = true;

    if (height > 25)
        height = 25;
    height --;

    QModelIndex index;

    for (int row = 0; row <= 24; row++)
    {
        pos = row * (width>>1) + (destColumn>>1);

        for (int column = 0; column < 50; column++)
        {
            if (rightFirst)
            {
                if (column & 1)
                {
                    num = patt->color[pos + 1 +(column>>1)].left;
                }
                else
                {
                    num = patt->color[pos+(column>>1)].right;
                }
            }
            else
            {
                if (column & 1)
                {
                    num = patt->color[pos+(column>>1)].right;
                }
                else
                {
                    num = patt->color[pos+(column>>1)].left;
                }
            }

            index = model->index(24 - row, column);
            //model->setData(index, num, Qt::EditRole);
        }
    }

    originIndex.column = destColumn;
    originIndex.row = destRow;

    emit updatePattPosition(destRow, destColumn);
}

void pattWorker::initTable(QTableView *tvPatt, QTableView *tvCtrl)
{
    if (baPatt.isEmpty())
        return;
    setPage(tvPatt->model(), 0, 0);
}

void pattWorker::start(void)
{
    thread.start();
}

void pattWorker::exit(void)
{
    thread.quit();
    thread.wait();
}
