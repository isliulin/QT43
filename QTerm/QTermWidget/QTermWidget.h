#ifndef QTERMWIDGET_H
#define QTERMWIDGET_H

#include "QTermScreen.h"

class QTermWidget : public QTermScreen
{
public:
    QTermWidget();

public slots:
    void putData(const QByteArray &data);

signals:
    void outData(const QByteArray &data);

protected:
    virtual void mousePressEvent(QMouseEvent *e);

private:
    void recvChar(char ch);

private:
    int m_Mode;
    QByteArray m_Param;
    QByteArray m_Text;
};

#endif // QTERMWIDGET_H
