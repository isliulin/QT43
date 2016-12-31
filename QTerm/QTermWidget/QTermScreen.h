#ifndef QTERMSCREEN_H
#define QTERMSCREEN_H

#include <QPlainTextEdit>

class QTermScreen : public QPlainTextEdit
{
public:
    QTermScreen();

public:
    void CursorStartOfLine();
    void CursorNewLine();
    void CursorUp(int n = 1);
    void CursorDown(int n = 1);
    void CursorLeft(int n = 1);
    void CursorRight(int n = 1);
    void CursorHome(int row = 0, int column = 0);

};

#endif // QTERMSCREEN_H
