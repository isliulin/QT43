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

protected:
    void changeEvent(QEvent *e);

public slots:
    void addItem(int model, int row, int column, QStandardItem * item);

private slots:
    void on_button_back_clicked();
    void currentItem(const QModelIndex & current, const QModelIndex & previous);

    void on_button_goto_clicked();

    void on_button_copy_clicked();

    void on_button_paste_clicked();

    void on_button_delrrow_clicked();

    void on_button_delcolumn_clicked();

    void on_button_leftinc_clicked();

    void on_button_rightinc_clicked();

    void on_button_upinc_clicked();

    void on_button_downinc_clicked();

private:
    Ui::patternEdit *ui;
    QStandardItemModel model_patt; // 花样文件显示
    QStandardItemModel model_ctrl; // 控制文件显示
    patternWorker worker;
    EditDelegate delegate;         // 输入代理
    int width_patt, height_patt;   // 花样宽度和高度
};

#endif // PATTERNEDIT_H
