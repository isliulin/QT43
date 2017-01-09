#ifndef SERIALASSISTANT_H
#define SERIALASSISTANT_H

#include <QWidget>

namespace Ui {
class SerialAssistant;
}

class SerialAssistant : public QWidget
{
    Q_OBJECT

public:
    explicit SerialAssistant(QWidget *parent = 0);
    ~SerialAssistant();

private:
    Ui::SerialAssistant *ui;
};

#endif // SERIALASSISTANT_H
