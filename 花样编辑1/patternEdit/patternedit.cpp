#include "patternedit.h"
#include "ui_patternedit.h"
#include "pattimagemodel.h"
#include "pattimagedelegate.h"
#include <QKeyEvent>
#include <QPainter>
#include <QPaintEngine>

patternEdit::patternEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::patternEdit)
{
    ui->setupUi(this);

    modelPatt = new pattImageModel(this);
    delegate = new pattImageDelegate(this);
    revColor = new QLabel(ui->tv_patt);
    revColor->setFocusPolicy(Qt::NoFocus);
    ui->tv_patt->setItemDelegate(delegate);
    ui->tv_patt->setModel(modelPatt);
    ui->tv_patt->installEventFilter(this);

    // 获取当前单元格
    connect(ui->tv_patt->selectionModel(),
            SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this,
            SLOT(currentItem(const QModelIndex &, const QModelIndex &)));
    // 移动表格
    connect(this, SIGNAL(viewMove(QTableView*, int, int)),
            &worker, SLOT(viewMove(QTableView*, int, int)));
    // 更新位置
    connect(&worker, SIGNAL(updatePattPosition(int,int)),
            this, SLOT(updatePattPosition(int,int)));

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()),
            this, SLOT(update()));

    worker.start();
    // 延迟初始化表格
    QTimer::singleShot(200, this, SLOT(initTable()));
}

patternEdit::~patternEdit()
{
    delete ui;
    worker.exit();
}

void patternEdit::initTable()
{
    ushort width, height;

    worker.initTable(ui->tv_patt, 0);
    worker.pattSize(width, height);
    ui->label_width->setNum(width);
    ui->label_height->setNum(height);
    timer->start(1000);
}

bool patternEdit::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
        {
        case Qt::Key_Up:
            if (currentIndex.row() == 0)
            {
                emit viewMove(ui->tv_patt, 1, 0);
            }
            break;
        case Qt::Key_Down:
            if ((currentIndex.row() + 1) == modelPatt->rowCount())
            {
                emit viewMove(ui->tv_patt, 2, 0);
            }
            break;
        case Qt::Key_Left:
            if (currentIndex.column() == 0)
            {
                emit viewMove(ui->tv_patt, 3, 0);
            }
            break;
        case Qt::Key_Right:
            if ((currentIndex.column() + 1) == modelPatt->columnCount())
            {
                emit viewMove(ui->tv_patt, 4, 0);
            }
            break;
        }
    }

    return false;
}

void patternEdit::currentItem(const QModelIndex &current, const QModelIndex &)
{
    ushort row, column;

    currentIndex = current;
    worker.originPosition(&row, &column);
    row += (modelPatt->rowCount() - current.row());
    column += (current.column() + 1);

    ui->label_x->setNum(column);
    ui->label_y->setNum(row);

    delegate->currentIndex = current;
}

void patternEdit::updatePattPosition(int originRow, int originColumn)
{
    int x, y;

    x = currentIndex.column() + 1 + originColumn;
    y = modelPatt->rowCount() - currentIndex.row() + originRow;
    ui->label_x->setNum(x);
    ui->label_y->setNum(y);
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

void patternEdit::update()
{
    static bool revColor = false;
    QWidget *widget = (QWidget*)ui->tv_patt;

    if (ui->tv_patt->focusWidget() != widget)
        return;

    if (revColor)
    {
        revColor = false;
    }
    else
    {
        revColor = true;
    }

    if (modelPatt->enableRevColor)
    {
        // 触发当前单元格数据更改信号从而重绘当前单元格
        modelPatt->setData(currentIndex, revColor, Qt::UserRole);
    }
}
