#include "OEScreen.h"

#include <QMouseEvent>
#include <QPainter>

OEScreen::OEScreen(QWidget *parent)
    : QWidget(parent),
      direction_(NONE),
      isPressed_(false)
{
    menu_ = new QMenu(this);
    menu_->addAction(QStringLiteral("完成"), this, SLOT(onSaveScreen()));
    menu_->addSeparator();
    menu_->addAction(QStringLiteral("退出"), this, SLOT(quitScreenshot()));

    /// 双击即完成
    connect(this, SIGNAL(doubleClick()), this, SLOT(onSaveScreen()));

    /// 开启鼠标实时追踪
    setMouseTracking(true);
}

OEScreen::DIRECTION OEScreen::getRegion(const QPoint &cursor)
{
    OEScreen::DIRECTION ret_dir = NONE;
    // left upper
    QPoint pt_lu = mapToParent(rect().topLeft());
    // right lower
    QPoint pt_rl = mapToParent(rect().bottomRight());

    int x = cursor.x();
    int y = cursor.y();

    /// 获得鼠标当前所处窗口的边界方向
    if(pt_lu.x() + PADDING_ >= x
    && pt_lu.x() <= x
    && pt_lu.y() + PADDING_ >= y
    && pt_lu.y() <= y) {
        // 左上角
        ret_dir = LEFTUPPER;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x >= pt_rl.x() - PADDING_
           && x <= pt_rl.x()
           && y >= pt_rl.y() - PADDING_
           && y <= pt_rl.y()) {
        // 右下角
        ret_dir = RIGHTLOWER;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= pt_lu.x() + PADDING_
           && x >= pt_lu.x()
           && y >= pt_rl.y() - PADDING_
           && y <= pt_rl.y()) {
        // 左下角
        ret_dir = LEFTLOWER;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= pt_rl.x()
           && x >= pt_rl.x() - PADDING_
           && y >= pt_lu.y()
           && y <= pt_lu.y() + PADDING_) {
        // 右上角
        ret_dir = RIGHTUPPER;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= pt_lu.x() + PADDING_
           && x >= pt_lu.x()) {
        // 左边
        ret_dir = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= pt_rl.x()
            && x >= pt_rl.x() - PADDING_) {
        // 右边
        ret_dir = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if(y >= pt_lu.y()
          && y <= pt_lu.y() + PADDING_){
        // 上边
        ret_dir = UPPER;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= pt_rl.y()
           && y >= pt_rl.y() - PADDING_) {
        // 下边
        ret_dir = LOWER;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }else {
        // 默认
        ret_dir = NONE;
        this->setCursor(QCursor(Qt::SizeAllCursor));
    }
    return ret_dir;
}


void OEScreen::contextMenuEvent(QContextMenuEvent *) {
    /// 在鼠标位置弹射出菜单栏
    menu_->exec(cursor().pos());
}

void OEScreen::mouseDoubleClickEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        emit doubleClick();
        e->accept();
    }
}

void OEScreen::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        isPressed_ = true;
        if(direction_ != NONE) {
            this->mouseGrabber();
        }
        /// @bug :这里可能存在问题, 不应当使用globalPos
        movePos_ = e->globalPos() - pos();
    }
}

void OEScreen::mouseReleaseEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton)
    {
        isPressed_ = false;
        if(direction_ != NONE)
        {
            setCursor(QCursor(Qt::SizeAllCursor));
        }
    }
}

void OEScreen::mouseMoveEvent(QMouseEvent * e)
{
    QPoint gloPoint = mapToParent(e->pos());
    // left upper
    QPoint pt_lu = mapToParent(rect().topLeft());
    // left lower
    QPoint pt_ll = mapToParent(rect().bottomLeft());
    // right lower
    QPoint pt_rl = mapToParent(rect().bottomRight());
    // right upper
    QPoint pt_ru = mapToParent(rect().topRight());
    if(!isPressed_)
    {
        /// 检查鼠标鼠标方向
        direction_ = getRegion(gloPoint);

        /// 根据方位判断拖拉对应支点
        switch(direction_) {
        case NONE:
        case RIGHT:
        case RIGHTLOWER:
            originPoint_ = pt_lu;
            break;
        case RIGHTUPPER:
            originPoint_ = pt_ll;
            break;
        case LEFT:
        case LEFTLOWER:
            originPoint_ = pt_ru;
            break;
        case LEFTUPPER:
        case UPPER:
            originPoint_ = pt_rl;
            break;
        case LOWER:
            originPoint_ = pt_lu;
            break;
        }
    }
    else
    {
        if(direction_ != NONE)
        {
            const int& global_x = gloPoint.x();
            /// 鼠标进行拖拉拽
            switch(direction_) {
            case LEFT:
                return onMouseChange(global_x, pt_ll.y() + 1);
            case RIGHT:
                return onMouseChange(global_x, pt_rl.y() + 1);
            case UPPER:
                return onMouseChange(pt_lu.x(), gloPoint.y());
            case LOWER:
                return onMouseChange(pt_rl.x() + 1, gloPoint.y());
            case LEFTUPPER:
            case RIGHTUPPER:
            case LEFTLOWER:
            case RIGHTLOWER:
                return onMouseChange(global_x, gloPoint.y());
            default:
                break;
            }
        }
        else
        {
            /// 窗口的移动
            /// @bug :这里可能存在问题, 不应当使用globalPos
            move(e->globalPos() - movePos_);
            movePos_ = e->globalPos() - pos();
        }
    }
    currentRect_ = geometry();
}

void OEScreen::moveEvent(QMoveEvent *)
{
    emit postionChange(x(), y());
}

void OEScreen::resizeEvent(QResizeEvent *)
{
    listMarker_.clear();

    /// 重新计算八个锚点
    // 角点
    listMarker_.push_back(QPoint(0, 0));
    listMarker_.push_back(QPoint(width(), 0));
    listMarker_.push_back(QPoint(0, height()));
    listMarker_.push_back(QPoint(width(), height()));

    // 中点
    listMarker_.push_back(QPoint((width() >> 1), 0));
    listMarker_.push_back(QPoint((width() >> 1), height()));
    listMarker_.push_back(QPoint(0, (height() >> 1)));
    listMarker_.push_back(QPoint(width(), (height() >> 1)));

    emit sizeChange(width(), height());
}

void OEScreen::hideEvent(QHideEvent *) {
    currentRect_ = {};
    movePos_ = {};
    originPoint_ = {};
}

void OEScreen::enterEvent(QEvent *e) {
    setCursor(Qt::SizeAllCursor);
    QWidget::enterEvent(e);
}

void OEScreen::leaveEvent(QEvent *e) {
    setCursor(Qt::ArrowCursor);
    QWidget::leaveEvent(e);
}

void OEScreen::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    /// 绘制边框线
    QPen pen(QColor(0,174,255),6);
    painter.setPen(pen);
    painter.drawRect(rect());

    /// 绘制八个点
    //改变点的宽度
    pen.setWidth(12);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPoints(listMarker_);
}

void OEScreen::onSaveScreenOther(void) {

    QString fileName;

    if (fileName.length() > 0) {
        originPainting_->copy(currentRect_).save(fileName, "jpg");
        quitScreenshot();
    }
}

void OEScreen::onSaveScreen(void)
{

}

void OEScreen::quitScreenshot(void)
{
    close();
    parentWidget()->close();
}

void OEScreen::onMouseChange(int x, int y)
{
    show();
    if (x < 0 || y < 0)
    {
        return;
    }
    const int& rx = (x >= originPoint_.x()) ? originPoint_.x() : x;
    const int& ry = (y >= originPoint_.y()) ? originPoint_.y() : y;
    const int& rw = abs(x - originPoint_.x());
    const int& rh = abs(y - originPoint_.y());

    /// 改变大小
    currentRect_ = QRect(rx, ry, rw, rh);

    this->setGeometry(currentRect_);
    /// 改变大小后更新父窗口，防止父窗口未及时刷新而导致的问题
    parentWidget()->update();
}
