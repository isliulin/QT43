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

    modemCheck.setSingleShot(true);
    connect(&modemCheck, SIGNAL(timeout()), this, SLOT(newModem()));
}

void Console::newModem()
{
    modem = new Ymodem(this);
    ModemEn = true;
    connect(modem, SIGNAL(finished()), this, SLOT(deleteModem()));
    modem->start(); 
}

void Console::deleteModem()
{
    ModemEn = false;
    delete modem;
    modem = NULL;
}

void Console::showMsg(const char *s)
{
    QString qs = s;

    insertPlainText(qs);
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
        modemCheck.stop();
        if (data.at(data.size()- 1) == 'C')
        {
            modemCheck.start(20);
        }
    }

    insertPlainText(QString(data));

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void Console::setLocalEchoEnabled(bool set)
{
    localEchoEnabled = set;
}

void Console::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void Console::getFile(QString &name)
{
   name = fileName;
}

void Console::setEnabled(bool on)
{
    if (!on)
    {
        if (modem)
        {
            modem->close();
            delete modem;
            modem = NULL;
        }
    }

    QPlainTextEdit::setEnabled(on);
}

void Console::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls;

    urls = event->mimeData()->urls();
    if (urls.isEmpty())
       return;

    fileName = urls.first().toLocalFile();

    emit showStatus(fileName.toStdString());
}

void Console::keyPressEvent(QKeyEvent *e)
{
    if (localEchoEnabled)
    {
        switch (e->key())
        {
        case Qt::Key_Backspace:
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Enter:
        case Qt::Key_Return:
            break;
        default:
            QPlainTextEdit::keyPressEvent(e);
            break;
        }
    }

    emit getData(e->text().toLocal8Bit());
}

void Console::mousePressEvent(QMouseEvent *e)
{
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
