/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>
#include <QTimer>
#include <string>

using namespace std;
class Ymodem;

typedef enum
{
    TerActColor = 'm',
}term_act_t;

typedef struct
{
    int mode;
    string param;
    term_act_t act;
}term_ctl_t;

class Console : public QPlainTextEdit
{
    Q_OBJECT

public:
    typedef enum
    {
        Black = 0,
        Red = 1,
        Green = 2,
        Yellow = 3,
        Blue = 4,
        Magenta = 5,
        Cyan = 6,
        White = 7,
    }ConsoleCorlours;

public:
    void setEnabled(bool on);
    void showStatus(const char *s);
    void showTransfer(int total, int remain, float speed);
    void setTextColor(uint8_t c);

signals:
    void getData(const QByteArray &data);
    void showStatus(string s);

private slots:
    void dropEvent(QDropEvent *event);
    void newModem();
    void deleteModem();

public:
    explicit Console(QWidget *parent = 0);

    void putData(const QByteArray &data);
    void setLocalEchoEnabled(bool set);
    void getFile(QString &name);
    QColor getColor(ConsoleCorlours col);

public:
    /*Cursor Control */

    void CursorStartOfLine();
    void CursorNewLine();
    void CursorUp(int n);
    void CursorDown(int n);
    void CurSorHome(int row = 0, int column = 0);

public:
    /* Erasing Text */

    void EraseLine();
    void EraseDown();
    void EraseEndOfLine();

public:
    /* Set Display Attributes */

    void DisplayReset();
    void DisplayForegroundColour(ConsoleCorlours col);
    void DisplayBackgroundColour(ConsoleCorlours col);

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *e);

private:
    void charProcess(const QByteArray &data);
    void getConColor(string &param, int &act, int &bc, int &fc);

private:
    bool localEchoEnabled;
    Ymodem *modem;
    bool ModemEn;
    QTimer *modemCheck;
    QString fileName;
    int lastkey;
    term_ctl_t terCtl;
};

#endif // CONSOLE_H
