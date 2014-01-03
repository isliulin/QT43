#include "patternedit.h"
#include "ui_patternedit.h"
#include <QScrollBar>
#include <QKeyEvent>
#include "gotodialog.h"
#include "copydialog.h"
#include "pastedialog.h"
#include "pattern.h"

patternEdit::patternEdit(QWidget *parent) :
    QWidget(parent/*, Qt::FramelessWindowHint*/),
    ui(new Ui::patternEdit)
{
    ui->setupUi(this);

    for (int row = 0; row < 25; row++)
    {
        for (int column = 0; column < 50; column++)
        {
            QStandardItem *item = new QStandardItem();
            model_patt.setItem(row, column, item);
        }
    }

    for (int row = 0; row < 25; row++)
    {
        for (int column = 0; column < 5; column++)
        {
            QStandardItem *item = new QStandardItem();
            model_ctrl.setItem(row, column, item);
        }
    }

    ui->table_patt->setModel(&model_patt);
    ui->table_ctrl->setModel(&model_ctrl);
    ui->table_patt->setItemDelegate(&delegate);
    ui->table_patt->installEventFilter(this);

    ui->spinBox_repeat->setMinimum(1);
    /* 初始化花样文件大小信息 */
    width_patt = 0;
    height_patt = 0;

    /* 左右两个表垂直联动 */
    connect(ui->table_patt->verticalScrollBar(), SIGNAL(valueChanged(int)),
            ui->table_ctrl->verticalScrollBar(), SLOT(setValue(int)));
    /* 获取当前单元格 */
    connect(ui->table_patt->selectionModel(),
            SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this,
            SLOT(currentItem(const QModelIndex &, const QModelIndex &)));

    connect(this, SIGNAL(viewMove(QTableView*, int, int)),
            &worker, SLOT(viewMove(QTableView*, int, int)));
    connect(&worker, SIGNAL(setPattData(const QByteArray &, const QByteArray &)),
            this, SLOT(setPattData(const QByteArray &, const QByteArray &)));
    connect(this, SIGNAL(initTable(QTableView*,QTableView*)),
            &worker, SLOT(initTable(QTableView*,QTableView*)));
    connect(&worker, SIGNAL(updatePattPosition(int,int)),
            this, SLOT(updatePattPosition(int,int)));

    QTimer::singleShot(50, this, SLOT(startWork()));
}

patternEdit::~patternEdit()
{
    worker.exit();
    delete ui;
}

void patternEdit::startWork()
{
    worker.start();

    emit initTable(ui->table_patt, ui->table_ctrl);
}

bool patternEdit::eventFilter(QObject *obj, QEvent *event)
{
    int row, column;

    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
        {
        case Qt::Key_Up:
            //viewMove(ui->table_patt, 1, 0);
            break;
        case Qt::Key_Down:
            //viewMove(ui->table_patt, 2, 0);
            break;
        case Qt::Key_Left:
            if (currentIndex.column() == 0)
            {
                viewMove(ui->table_patt, 3, 0);
            }
            break;
        case Qt::Key_Right:
            if ((currentIndex.column() + 1) == model_patt.columnCount())
            {
                emit viewMove(ui->table_patt, 4, 0);
            }
            break;
        }
    }

    return QObject::eventFilter(obj, event);
}

//void patternEdit::keyPressEvent(QKeyEvent *e)
//{

//}

void patternEdit::setPattData(const QByteArray &baPatt, const QByteArray &bactrl)
{
    QStandardItemModel *model = &model_patt;
    patt_dat_t *patt = (patt_dat_t*)baPatt.data();
    int width, height;
    int pos;

    width = patt->width;
    height = patt->height;
    if (width > 50)
        width = 50;
    if (height > 25)
        height = 25;
    height --;
    ui->table_patt->setUpdatesEnabled(0);
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
    ui->table_patt->setUpdatesEnabled(1);
}

void patternEdit::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void patternEdit::on_button_back_clicked()
{
    this->close();
}

void patternEdit::currentItem(const QModelIndex & current, const QModelIndex &)
{
    int x, y;

    currentIndex = current;
    x = currentIndex.column() + 1 + worker.startIndex.column;
    y = model_patt.rowCount() - currentIndex.row() + worker.startIndex.row;
    ui->label_x->setNum(x);
    ui->label_y->setNum(y);
}

void patternEdit::updatePattPosition(int originColumn, int originRow)
{
    int x, y;

    x = currentIndex.column() + 1 + originColumn;
    y = model_patt.rowCount() - currentIndex.row() + originRow;
    ui->label_x->setNum(x);
    ui->label_y->setNum(y);
}

void patternEdit::on_button_goto_clicked()
{
    gotoDialog dialog(this);
    int row, column;

    /* 限定用户输入的最大值 */
    dialog.setXYMax(model_patt.columnCount(), model_patt.rowCount());
    if (dialog.exec() == QDialog::Accepted)
    {
        QModelIndex index;

        /* 获取用户输入 */
        dialog.getXY(&column, &row);
        /* 视图中是将左下角作为原点的 */
        column -= 1;
        row = model_patt.rowCount() - row;
        index = model_patt.index(row, column);
        /* 选择指定的单元格 */
        ui->table_patt->selectionModel()->setCurrentIndex(index,
            QItemSelectionModel::Select | QItemSelectionModel::Current);
        /* 激活选择的单元格 */
        ui->table_patt->setFocus();
    }
}

void patternEdit::on_button_copy_clicked()
{
    copyDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {

    }
}

void patternEdit::on_button_paste_clicked()
{
    pasteDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {

    }
}

void patternEdit::on_button_delrrow_clicked()
{
    QModelIndex index;

    index = ui->table_patt->selectionModel()->currentIndex();
    model_patt.removeRow(index.row());
}

void patternEdit::on_button_delcolumn_clicked()
{
    QModelIndex index;

    index = ui->table_patt->selectionModel()->currentIndex();
    model_patt.removeColumn(index.column());
}

void patternEdit::on_button_leftinc_clicked()
{
    QModelIndex index;

    index = ui->table_patt->selectionModel()->currentIndex();
    model_patt.insertColumns(index.column(), ui->spinBox_repeat->value());
}

void patternEdit::on_button_rightinc_clicked()
{
    QModelIndex index;

    index = ui->table_patt->selectionModel()->currentIndex();
    model_patt.insertColumns(index.column() + 1, ui->spinBox_repeat->value());
}

void patternEdit::on_button_upinc_clicked()
{
    QModelIndex index;

    index = ui->table_patt->selectionModel()->currentIndex();
    model_patt.insertRows(index.row(), ui->spinBox_repeat->value());
}

void patternEdit::on_button_downinc_clicked()
{
    QModelIndex index;

    index = ui->table_patt->selectionModel()->currentIndex();
    model_patt.insertRows(index.row() + 1, ui->spinBox_repeat->value());
}
