#include "QTermScreen.h"

#include <QTextCursor>

QTermScreen::QTermScreen()
{
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    setPalette(p);
}

void QTermScreen::CursorStartOfLine()
{
    QTextCursor tc = textCursor();

    tc.movePosition(QTextCursor::StartOfBlock);
    setTextCursor(tc);
}

void QTermScreen::CursorNewLine()
{
    QTextCursor tc = textCursor();

    tc.movePosition(QTextCursor::EndOfBlock);
    tc.insertText("\n");
    setTextCursor(tc);
}

void QTermScreen::CursorUp(int n)
{
    QTextCursor tc = textCursor();

    tc.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, n);
    setTextCursor(tc);
}

void QTermScreen::CursorDown(int n)
{
    QTextCursor tc = textCursor();

    tc.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, n);
    setTextCursor(tc);
}

void QTermScreen::CursorLeft(int n)
{
    QTextCursor tc = textCursor();

    tc.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, n);
    setTextCursor(tc);
}

void QTermScreen::CursorRight(int n)
{
    QTextCursor tc = textCursor();

    tc.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, n);
    setTextCursor(tc);
}

void QTermScreen::CursorHome(int row, int column)
{
    QTextCursor tc = cursorForPosition(QPoint(0, 0));

    for (int i = 0; i < row - 1; i ++)
    {
        tc.movePosition(QTextCursor::Down);
    }
    for (int i = 0; i < column - 1; i ++)
    {
        tc.movePosition(QTextCursor::Right);
    }
    setTextCursor( tc );
}

void QTermScreen::DisplayForeground(QColor &color)
{
    QTextCharFormat fmt;

    fmt.setForeground(color);
    QTextCursor cursor = textCursor();
    cursor.mergeCharFormat(fmt);
    setTextCursor(cursor);
}

void QTermScreen::DisplayBackground(QColor &color)
{
    QTextCharFormat fmt;

    fmt.setBackground(color);
    QTextCursor cursor = textCursor();
    cursor.mergeCharFormat(fmt);
    setTextCursor(cursor);
}

void QTermScreen::DisplayAttribute(QVector <int> &param)
{
    for(int i = 0; i < param.count(); i ++)
    {
        int v = param[i];

        switch (v)
        {
        case 0:
        {
            DisplayReset();
        }break;
        default:
        {
            if (v >= 30 && v <= 37)
            {
                QColor c = GetColor(v - 30);
                DisplayForeground(c);
            }
            if (v >= 40 && v <= 47)
            {
                QColor c = GetColor(v - 40);
                DisplayBackground(c);
            }
        }break;
        }
    }
}

QColor QTermScreen::GetColor(int col)
{
    QColor color[8] =
    {
        "black", "red", "green", "yellow",
        "blue", "magenta", "cyan", "white"
    };

    if (col >= 0 && col <= 7)
    {
        return color[col];
    }
    else
    {
        return color[0];
    }
}

void QTermScreen::DisplayReset()
{
    QColor color;

    color = GetColor(0);
    DisplayBackground(color);
    color = GetColor(7);
    DisplayForeground(color);
}
