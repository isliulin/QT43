#include "SerialAssistant.h"
#include "ui_SerialAssistant.h"

SerialAssistant::SerialAssistant(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialAssistant)
{
    ui->setupUi(this);
}

SerialAssistant::~SerialAssistant()
{
    delete ui;
}
