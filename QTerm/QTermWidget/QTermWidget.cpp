#include "QTermWidget.h"

QTermWidget::QTermWidget()
{
    m_Mode = 0;
}

void QTermWidget::putData(const QByteArray &data)
{
    if (data.size() == 0)
        return;

    m_Text.clear();

    for (int i = 0; i < data.size(); i ++)
    {
        recvChar(data[i]);
    }

    insertPlainText(m_Text);
}

void QTermWidget::recvChar(char ch)
{
    switch (m_Mode)
    {
    case 2:
    {
        switch (ch)
        {
        case 'A':
        {
            m_Mode = 0;
            CursorUp();
        }break;
        case 'B':
        {
            m_Mode = 0;
            CursorDown();
        }break;
        case 'C':
        {
            m_Mode = 0;
            CursorLeft();
        }break;
        case 'D':
        {
            m_Mode = 0;
            CursorRight();
        }break;
        case 'H':
        {
            m_Mode = 0;
            CursorHome();
        }break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case ';':
        {
            m_Param.push_back(ch);
        }break;
        default:
        {
            m_Mode = 0;
        }break;
        }
    }break;
    case 1:
    {
        switch (ch)
        {
        case '[':
        {
            m_Mode = 2;
        }break;
        default:
        {
            m_Mode = 0;
        }break;
        }
    }break;
    default:
    {
        switch (ch)
        {
        case '\e':
        {
            m_Mode = 1;
            m_Param.clear();

            insertPlainText(m_Text);
            m_Text.clear();
        }break;
        case '\r':
        {
            CursorStartOfLine();
        }break;
        case '\n':
        {
            CursorNewLine();
        }break;
        default:
        {
            m_Text.append(ch);
        }break;
        }
    }break;
    }
}

void QTermWidget::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}
