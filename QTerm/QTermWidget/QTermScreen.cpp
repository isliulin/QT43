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
