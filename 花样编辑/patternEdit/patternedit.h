#ifndef PATTERNEDIT_H
#define PATTERNEDIT_H

#include <QtCore>
#include <QWidget>
#include <QStandardItemModel>
#include "patternworker.h"
#include "editdelegate.h"

namespace Ui {
class patternEdit;
}

class patternEdit : public QWidget
{
    Q_OBJECT

public:
    explicit patternEdit(QWidget *parent = 0);
    ~patternEdit();

    bool eventFilter(QObject *obj, QEvent *event);

protected:
    void changeEvent(QEvent *e);
    //void keyPressEvent(QKeyEvent *);

signals:
    void viewMove(QTableView *tbPatt, int dir, int type);
    void readFile(void *model_patt, void *model_ctrl);
    void initTable(QTableView *patt, QTableView *ctrl);

public slots:
    void setPattData(const QByteArray &, const QByteArray &);
    void updatePattPosition(int originColumn, int originRow);

private slots:
    void on_button_back_clicked();
    void currentItem(const QModelIndex & current, const QModelIndex & previous);
    void startWork();

    void on_button_goto_clicked();

    void on_button_copy_clicked();

    void on_button_paste_clicked();

    void on_button_delrrow_clicked();

    void on_button_delcolumn_clicked();

    void on_button_leftinc_clicked();

    void on_button_rightinc_clicked();

    void on_button_upinc_clicked();

    void on_button_downinc_clicked();
public:
    QStandardItemModel model_patt; // 花样文件显示
    QStandardItemModel model_ctrl; // 控制文件显示

private:
    Ui::patternEdit *ui;
    patternWorker worker;
    EditDelegate delegate;         // 输入代理
    int width_patt, height_patt;   // 花样宽度和高度
    int currentRow, currentColumn;
    QModelIndex currentIndex;
};

#endif // PATTERNEDIT_H
