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
    void parseParam(QVector<int> &param, int np = 1, int defval = 0);
    void eraseText(char ch);
    void moveCursor(char ch);

private:
    int m_Mode;
    QString m_Param;
    QByteArray m_Text;
};

#endif // QTERMWIDGET_H
