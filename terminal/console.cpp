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
        int pos = 0;

        modemCheck->stop();
        if (data.at(data.size()- 1) == 'C')
        {
            modemCheck->start(20);
        }

        if (data.startsWith("\x1B[2K"))
        {
            //删除当前行
            QTextCursor tc = textCursor();
            tc.select(QTextCursor::BlockUnderCursor);
            tc.removeSelectedText();
            pos = 4;
        }

        for (int i = 0; i < data.size() - pos; i ++)
        {
            byte[0] = data[i + pos];
            charProcess(byte);
        }
    }
}

void Console::charProcess(const QByteArray &data)
{
    switch (data.at(0))
    {
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
        break;
    case '\n':
        insertPlainText(QString(data));
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
    }
    break;
    }

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
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
