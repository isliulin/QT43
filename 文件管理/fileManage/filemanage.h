#ifndef FILEMANAGE_H
#define FILEMANAGE_H

#include <QWidget>
#include <QFileSystemModel>
#include "fileworker.h"

namespace Ui {
class fileManage;
}

class fileManage : public QWidget
{
    Q_OBJECT
    
public:
    explicit fileManage(QWidget *parent = 0);
    ~fileManage();

private slots:
    void initTree();

private:
    Ui::fileManage *ui;
    fileWorker worker;
    QFileSystemModel model[2];
};

#endif // FILEMANAGE_H
