#include "PhView.h"
#include "ui_phview.h"

#include <QMouseEvent>

PhView::PhView(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::PhView)
{
    ui->setupUi(this);
}

PhView::~PhView()
{
    delete ui;
}

void PhView::mouseMoveEvent(QMouseEvent * event)
{
    QWidget *label = ui->cutRect;
    QPoint pos = event->pos();

}

void PhView::mousePressEvent(QMouseEvent * event)
{
    QWidget *label = ui->cutRect;
    QPoint pos = event->pos();


}
