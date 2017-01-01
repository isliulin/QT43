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

#include "console.h"

#include <QScrollBar>
#include <QFont>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>
#include <QTextCursor>
#include <QTextBlock>

#include "Ymodem.h"

Console::Console(QWidget *parent)
    : QPlainTextEdit(parent)
    , localEchoEnabled(false)
    , modem(NULL)
    , ModemEn(false)
{
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    setPalette(p);

    setAcceptDrops(true);

    modemCheck = new QTimer;
    modemCheck->setSingleShot(true);
    connect(modemCheck, SIGNAL(timeout()), this, SLOT(newModem()));
}

void Console::CursorStartOfLine()
{
    QTextCursor tc = textCursor();

    tc.movePosition(QTextCursor::StartOfLine);
    setTextCursor(tc);
}

void Console::CursorNewLine()
{
    QTextCursor tc = textCursor();

    tc.movePosition(QTextCursor::EndOfLine);
    tc.insertText("\n");
    setTextCursor(tc);
}

void Console::newModem()
{
    if (fileName.isEmpty())
    {
        showStatus("请拖入文件");
    }
    else
    {
        modem = new Ymodem(this);
        ModemEn = true;
        connect(modem, SIGNAL(finished()), this, SLOT(deleteModem()));
        modem->start();
    }
}

void Console::deleteModem()
{
    ModemEn = false;
    if (modem)
    {
        modem->close();
        delete modem;
        modem = NULL;
    }
}

QColor Console::getColor(ConsoleCorlours col)
{
    QColor color[8] =
    {
        "black", "red", "green", "yellow",
        "blue", "magenta", "cyan", "white"
    };

    if (col >= Black && col <= White)
    {
        return color[col];
    }
    else
    {
        return color[0];
    }
}

void Console::setTextColor(uint8_t c)
{
    QColor col[8] =
    {
        "black", "red", "green", "yellow",
        "blue", "purple", "cyan", "white"
    };

    c = c - 30;
    if (c <= 7)
    {
        QTextCharFormat fmt;
        fmt.setForeground(col[c]);
        QTextCursor cursor = textCursor();
        cursor.mergeCharFormat(fmt);
        setTextCursor(cursor);
    }
}

void Console::showTransfer(int total, int remain, float speed)
{
    QString str;
    float p;

    p = (total - remain)/(float)total;
    str.sprintf("进度:%0.1f%% 速度:%.3f KB/S", p*100, speed/1024);
    emit showStatus(str.toStdString());
}

void Console::showStatus(const char *s)
{
    string ss = s;

    emit showStatus(ss);
}

void Console::putData(const QByteArray &data)
{
    if (ModemEn)
    {
        modem->put(data);
    }
    else
    {
        QByteArray byte;

        modemCheck->stop();
        if (data.at(data.size()- 1) == 'C')
        {
            modemCheck->start(20);
        }
        QString str;

        for (int i = 0; i < data.size(); i ++)
        {
            QString tmp;
            byte[0] = data[i];
            charProcess(byte);
            str += tmp.sprintf("%02X ", byte.data()[0]);
        }
qDebug(str.toStdString().c_str());
    }
}

void Console::EraseLine()
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::LineUnderCursor);
    tc.removeSelectedText();
}

void Console::EraseEndOfLine()
{
    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    tc.removeSelectedText();
    setTextCursor(tc);
}

void Console::EraseDown()
{
    QTextCursor tc = textCursor();

    tc.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    tc.removeSelectedText();
}

void Console::DisplayReset()
{
    QColor color = getColor(White);

    QTextCharFormat fmt;
    fmt.setForeground(color);
    QTextCursor cursor = textCursor();
    cursor.mergeCharFormat(fmt);
    setTextCursor(cursor);
}

void Console::DisplayForegroundColour(ConsoleCorlours col)
{
    QColor color = getColor(col);

    QTextCharFormat fmt;
    fmt.setForeground(color);
    QTextCursor cursor = textCursor();
    cursor.mergeCharFormat(fmt);
    setTextCursor(cursor);
}

void Console::DisplayBackgroundColour(ConsoleCorlours col)
{

}

void Console::CurSorHome(int row, int column)
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

void Console::getConColor(string &param, int &act, int &c1, int &c2)
{
    QString tmp[3];
    int pos = 0;

    act = -1;
    c1 = -1;
    c2 = -1;

    for (int i = 0; i < param.size(); i ++)
    {
        char ch = param.data()[i];

        if (ch == ';')
        {
            pos ++;
            continue;
        }
        tmp[pos].push_back(ch);
    }
    if (!tmp[0].isEmpty())
    {
       act =  tmp[0].toInt();
    }

    if (!tmp[1].isEmpty())
    {
        c1 = tmp[1].toInt();
    }

    if (!tmp[2].isEmpty())
    {
        c2 = tmp[2].toInt();
    }
}

void Console::charProcess(const QByteArray &data)
{
    uint8_t ch;

    ch = data[0];

    if (terCtl.mode == 2)
    {
        switch (ch)
        {
        case 'K':
        {
            if (terCtl.param[0] == '2')
            {
                EraseLine();
            }
            if (terCtl.param[0] == '0')
            {
                qDebug("RRRR");
                EraseEndOfLine();
            }
            terCtl.mode = 0;
        }break;
        case 'm':
        {
            int act, c1, c2;

            getConColor(terCtl.param, act, c1, c2);
            if (act == 0)
            {
                DisplayReset();
            }
            if (c1 <= 37 && c1 >= 30)
            {
                ConsoleCorlours c = (ConsoleCorlours)(c1 - 30);
                DisplayForegroundColour(c);
            }
            terCtl.mode = 0;
        }break;
        case 'J':
            EraseDown();
            terCtl.mode = 0;
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'i':
        case 'r':
        case 'n':
        case 'c':
        case 'H':
        {
            int r, c,n;
            string dbg = terCtl.param + data.toStdString();
            qDebug(dbg.c_str());
            getConColor(terCtl.param, r, c, n);
            CurSorHome(r,c);
            terCtl.mode = 0;
        }
            break;
        default:
        {
            terCtl.param.push_back(ch);
        }break;
        }

        return;
    }

    switch (ch)
    {
#if 1
    case 0x1B:
    {
        terCtl.mode = 1;
    }
    break;
    case '[':
    {
        if (terCtl.mode == 1)
        {
            terCtl.mode = 2;
            terCtl.param.clear();
        }
    }break;
#endif
    case 0x08:
    {
        //选中字符
        QTextCursor  cur = textCursor();

        if (lastkey == Qt::Key_Left)
        {
            cur.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            setTextCursor(cur);
        }
        else
        {
            QKeyEvent ke(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);

            QPlainTextEdit::keyPressEvent(&ke);
        }
    }
    break;
    case '\r':
        CursorStartOfLine();
        break;
    case '\n':
        CursorNewLine();
        break;
    default:
    {
        if (lastkey == Qt::Key_Right)
        {
            QTextCursor cur = textCursor();

            lastkey = 0;
            cur.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);
            setTextCursor(cur);
        }
        else
        {
            insertPlainText(QString(data));
        }

        QScrollBar *bar = verticalScrollBar();
        bar->setValue(bar->maximum());
    }
    break;
    }
}

void Console::setLocalEchoEnabled(bool set)
{
    localEchoEnabled = set;
}

void Console::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls;

    urls = event->mimeData()->urls();
    if (urls.isEmpty())
       return;

    fileName = urls.first().toLocalFile();

    emit showStatus(fileName.toStdString());
    QDropEvent et(event->pos(), Qt::IgnoreAction, NULL,
                  Qt::NoButton, Qt::NoModifier);

    QPlainTextEdit::dropEvent(&et);
}

void Console::getFile(QString &name)
{
   name = fileName;
}

void Console::setEnabled(bool on)
{
    if (!on)
    {
        deleteModem();
    }

    QPlainTextEdit::setEnabled(on);
}


void Console::keyPressEvent(QKeyEvent *e)
{
    QByteArray byte;

    lastkey = e->key();

    switch (lastkey)
    {
    case Qt::Key_Backspace:
        byte[0] = 0x08;
        break;
    case Qt::Key_Left:
        byte[0] = 0x1B; byte[1] = 0x5B, byte[2] = 0x44;
        break;
    case Qt::Key_Right:
        byte[0] = 0x1B; byte[1] = 0x5B, byte[2] = 0x43;
        break;
    case Qt::Key_Up:
        byte[0] = 0x1B; byte[1] = 0x5B, byte[2] = 0x41;
        break;
    case Qt::Key_Down:
        byte[0] = 0x1B; byte[1] = 0x5B, byte[2] = 0x42;
        break;
    case Qt::Key_Enter:
        break;
    case Qt::Key_Return:
    {
        QTextCursor cur = textCursor();

        cur.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor, 0);
        setTextCursor(cur);
    }
    default:
        byte = e->text().toLocal8Bit();
        break;
    }

    if (byte.size() != 0)
    {
        emit getData(byte);
    }
}

void Console::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
    {
        QClipboard *clipboard = QApplication::clipboard();
        QByteArray byte = clipboard->text().toStdString().c_str();

        getData(byte);
    }
    Q_UNUSED(e)
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void Console::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}
