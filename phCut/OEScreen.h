#ifndef OESCREEN_H
#define OESCREEN_H

#include <memory>
#include <QWidget>
#include <QMenu>
using namespace std;

/**
 * @class : OEScreen
 * @brief : 截图器
 * @note  : 主要关乎图片的编辑与保存
*/
class OEScreen : public QWidget
{
    Q_OBJECT

public:

    explicit OEScreen(QWidget *parent = 0);

    ~OEScreen() {}

    QRect* rectCut();

signals:


    /**
     * @brief : 截图器大小修改（信号）
     * @param : int 宽度
     * @param : int 高度
     * @date  : 2017年04月17日
     */
    void sizeChange(int,int);

    /**
     * @brief : 截图器窗口的位置（信号）
     * @param : int 窗口的横向位置
     * @param : int 窗口的纵向位置
     * @date  : 2017年04月17日
     */
    void postionChange(int,int);

    /**
     * @brief : 双击 （信号）
     * @date  : 2017年04月17日
     */
    void doubleClick(void);

protected:

    /// 内边距，决定拖拽的触发。
    const int PADDING_ = 6;

    /// 方位枚举
    enum DIRECTION {
        UPPER=0,
        LOWER=1,
        LEFT,
        RIGHT,
        LEFTUPPER,
        LEFTLOWER,
        RIGHTLOWER,
        RIGHTUPPER,
        NONE
    };

protected:

    /**
     * @brief : 获得当前坐标点的边界方位
     * @param : cursor 当前鼠标的位置
     * @return: DIRECTION 鼠标的方位枚举
     * @date  : 2017年04月17日
     */
    DIRECTION getRegion(const QPoint &cursor);


    /**
     * @brief : 呼出菜单事件
     */
    virtual void contextMenuEvent(QContextMenuEvent *);

    /**
     * @brief : 双击事件
     */
    virtual void mouseDoubleClickEvent(QMouseEvent *e);

    /**
     * @brief : 鼠标按下事件
     */
    virtual void mousePressEvent(QMouseEvent *e);

    /**
     * @brief : 鼠标释放事件
     */
    virtual void mouseReleaseEvent(QMouseEvent *e);
    /**
     * @brief : 鼠标移动事件
     */
    virtual void mouseMoveEvent(QMouseEvent *e);

    /**
     * @brief : 窗口移动事件
     */
    virtual void moveEvent(QMoveEvent *);

    /**
     * @brief : 窗口大小修改事件
     */
    virtual void resizeEvent(QResizeEvent *);

    /**
     * @brief : 窗口隐藏事件
     */
    virtual void hideEvent(QHideEvent *);

    /**
     * @brief : 鼠标进入窗口事件
     */
    virtual void enterEvent(QEvent *e);

    /**
     * @brief : 鼠标离开窗口事件
     */
    virtual void leaveEvent(QEvent *e);

    /**
     * @brief : 界面自绘事件
     */
    virtual void paintEvent(QPaintEvent *);

public slots:


    /**
     * @brief : 根据鼠标位置修改窗口大小
     * @param : x 鼠标的横向位置
     * @param : y 鼠标的纵向位置
     * @date  : 2017年04月16日
     */
    void onMouseChange(int x,int y);

    /**
     * @brief : 保存屏幕到剪切板中
     * @date  : 2017年04月16日
     */
    void onSaveScreen(void);

protected slots:

    /**
     * @brief : 保存编辑屏幕到其他路径下
     * @note  : 会呼出路径选择的窗口
     * @date  : 2017年04月16日
     */
    void onSaveScreenOther(void);

    /**
     * @brief : 退出当前截图窗口
     * @date  : 2017年04月16日
     */
    void quitScreenshot(void);

private:
    /// 窗口大小改变时，记录改变方向
    DIRECTION       direction_;
    /// 起点
    QPoint          originPoint_;
    /// 鼠标是否按下
    bool            isPressed_;
    /// 拖动的距离
    QPoint          movePos_;
    /// 标记锚点
    QPolygon        listMarker_;
    /// 屏幕原画
    std::shared_ptr<QPixmap> originPainting_;
    /// 当前窗口几何数据 用于绘制截图区域
    QRect           currentRect_;
    /// 右键菜单对象
    QMenu           *menu_;

};

#endif // OESCREEN_H
