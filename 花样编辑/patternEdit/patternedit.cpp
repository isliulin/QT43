#include "patternedit.h"
#include "ui_patternedit.h"
#include <QScrollBar>
#include "gotodialog.h"
#include "copydialog.h"
#include "pastedialog.h"

patternEdit::patternEdit(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint),
    ui(new Ui::patternEdit)
{
    ui->setupUi(this);

    ui->table_patt->setModel(&model_patt);
    ui->table_ctrl->setModel(&model_ctrl);
    ui->table_patt->setItemDelegate(&delegate);

    ui->spinBox_repeat->setMinimum(1);
    /* 初始化花样文件大小信息 */
    width_patt = 0;
    height_patt = 0;
    /* 添加单元格 */
    connect(&worker, SIGNAL(addItem(int,int,int,QStandardItem*)),
            this, SLOT(addItem(int,int,int,QStandardItem*)));
    /* 左右两个表垂直联动 */
    connect(ui->table_patt->verticalScrollBar(), SIGNAL(valueChanged(int)),
            ui->table_ctrl->verticalScrollBar(), SLOT(setValue(int)));
    /* 获取当前单元格 */
    connect(ui->table_patt->selectionModel(),
            SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this,
            SLOT(currentItem(const QModelIndex &, const QModelIndex &)));

    worker.start();
}

patternEdit::~patternEdit()
{
    delete ui;

    worker.exit();
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
    ui->label_x->setNum(current.column() + 1);
    ui->label_y->setNum(model_patt.rowCount() - current.row());
}

void patternEdit::addItem(int model, int row, int column, QStandardItem *item)
{
    if (model == MODEL_PATT)
    {
        model_patt.setItem(row, column, item);
    }
    else if (model == MODEL_CTRL)
    {
        model_ctrl.setItem(row, column, item);
    }
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
