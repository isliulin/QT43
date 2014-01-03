#include "patternworker.h"
#include <QFile>
#include "pattern.h"

patternWorker::patternWorker(QObject *parent) :
    QObject(parent)
{
    connect(&thread, SIGNAL(started()), this, SLOT(doWork()));
    this->moveToThread(&thread);
    exit_notify = 0;
    // 左下角在花样文件中的绝对位置(起点视为0行0列)
    startIndex.column = 0;
    startIndex.row = 0;
}

void patternWorker::start()
{
    thread.start();
}

void patternWorker::exit()
{
    exit_notify = 1;
    thread.quit();
    thread.wait();
}

void patternWorker::doWork()
{
    QFile file;

    file.setFileName("7x7.dat");
    if (file.open(QIODevice::ReadOnly))
    {
        baPatt = file.readAll();
        file.close();
    }

    //emit setPattData(ba_patt, ba_ctrl);

    /*
    int row, column;
    QFile fpatt; // 花样文件
    QFile fctrl; // 控制文件
    QByteArray dpatt;
    QByteArray dctrl;

    // 读取花样文件
    fpatt.setFileName("/root/work/mq/flower2/7x7.dat");
    if (!fpatt.open(QIODevice::ReadWrite))
    {
        qDebug("file fail\n");
    }
    dpatt = fpatt.readAll();
    if (!dpatt.isEmpty())
    {

    }
    fpatt.close();
*/
/*
    // 读取控制文件
    fctrl.setFileName("/root/mq/flower2/7x7.con");
    if (!fctrl.open(QIODevice::ReadOnly))
    {
        qDebug("file fail\n");
    }
    dctrl = fctrl.readAll();
    if (!dctrl.isEmpty())
    {
        // 创建控制文件表格
        for (row = 0; row < 25; row ++)
        {
            for (column = 0; column < 5; column ++)
            {
                QStandardItem *item = new QStandardItem();
                item->setForeground(QBrush(Qt::white));
                emit addItem(MODEL_CTRL, row, column, item);
            }
        }
    }
*/

}

void patternWorker::readFile(void *model_patt, void *model_ctrl)
{
    /*
    QFile file;
    qDebug("zzzzz\n");
    file.setFileName("7x7.dat");
    if (file.open(QIODevice::ReadOnly))
    {
        ba_patt = file.readAll();
        file.close();
    }

    if (!ba_patt.isEmpty())
    {
        QStandardItemModel *model = (QStandardItemModel*)model_patt;
        patt_dat_t *patt = (patt_dat_t*)ba_patt.data();
        int width, height;
        int pos;

        width = patt->width;
        height = patt->height;
        if (width > 50)
            width = 50;
        if (height > 25)
            height = 25;
        height --;

        for (int row = 0; row <= height; row++)
        {
            pos = row * patt->width;
            for (int column = 0; column < width; column++)
            {
                QStandardItem *item = model->item(height - row, column);
                QString value;
                int num;

                if (column & 1)
                {
                    num = patt->color[pos+(column>>1)].right;
                }
                else
                {
                    num = patt->color[pos+(column>>1)].left;
                }
                value.setNum(num);

                item->setText(value);
            }
        }
    }
    */
}

void patternWorker::viewMove(QTableView* tbPatt, int dir, int type)
{
    patt_dat_t *patt;

    if (baPatt.isEmpty())
        return;

    patt = (patt_dat_t*)baPatt.data();
    QStandardItemModel *model = (QStandardItemModel*)tbPatt->model();
    tbPatt->setUpdatesEnabled(0);

    switch (dir)
    {
    case 1:
        break;
    case 2:
        break;
    case 3: // 左方向键
        if (startIndex.column == 0)
            goto _exit;

        if (type == 0) // 移动一格
        {
            setPage(model, startIndex.row, startIndex.column - 1);
        }
        else if (type == 1) // 翻页
        {
            setPage(model, startIndex.row, startIndex.column - 50);
        }
        break;
    case 4: // 右方向键
        if (type == 0) // 移动一格
        {
            setPage(model, startIndex.row, startIndex.column + 1);
        }
        else if (type == 1) // 翻页
        {
            setPage(model, startIndex.row, startIndex.column + 50);
        }
        break;
    }

_exit:
    tbPatt->setUpdatesEnabled(1);
}

void patternWorker::setPage(QStandardItemModel *model, int startRow, int startColumn)
{
    QStandardItem *item;
    int num;
    int width, height;
    int pos;
    bool rightFirst = false;
    static const char *pattDis[16] =
    {
        "", "1", "2", "3", "4", "5", "6", "7",
        "8", "9", "a", "b", "c", "d", "e", "f"
    };

    patt_dat_t* patt = (patt_dat_t*)baPatt.data();
    width = patt->width;
    height = patt->height;
    // 到达宽边的末尾
    if ((startColumn + 50) > width)
        return;
    // 一个字节包含两个宽度的花样
    if (width & 1)
        width ++;
    // 当左下角在奇数列时应该先显示一个字节的低4位
    if (startColumn & 1)
        rightFirst = true;

    if (height > 25)
        height = 25;
    height --;

    for (int row = 0; row <= height; row++)
    {
        pos = row * (width>>1) + (startColumn>>1);

        for (int column = 0; column < 50; column++)
        {
            item  = model->item(height - row, column);

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
            QString value(pattDis[num]);

            item->setText(value);
        }
    }

    startIndex.column = startColumn;
    startIndex.row = startRow;

    emit updatePattPosition(startColumn, startRow);
}

void patternWorker::initTable(QTableView *tbPatt, QTableView *tbCtrl)
{
    // 设置花样文件表格
    if (!baPatt.isEmpty())
    {
        QStandardItemModel *model;
        patt_dat_t *patt;
        int width, height;
        int pos;
        QChar ch[16] = {'\0', '1', '2', '3', '4', '5', '6', '7',
                        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

        patt = (patt_dat_t*)baPatt.data();
        model = (QStandardItemModel*)tbPatt->model();
        width = patt->width;
        height = patt->height;

        if (width & 1)
            width ++;
        if (width > 50)
            width = 50;
        if (height > 25)
            height = 25;
        height --;

        tbPatt->setUpdatesEnabled(0); // 禁止表格更新
        for (int row = 0; row < height; row++)
        {
            pos = row * (width>>1);
            for (int column = 0; column < width; column++)
            {
                QStandardItem *item = model->item(height - row, column);
                int num;

                if (column & 1)
                {
                    num = patt->color[pos+(column>>1)].right;
                }
                else
                {
                    num = patt->color[pos+(column>>1)].left;
                }

                QString value(ch[num]);
                item->setText(QString(ch[num]));
            }
        }

        tbPatt->setUpdatesEnabled(1); // 允许表格更新
    }
}
