/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "calib.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);
    settings = new SettingsDialog;

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    rxcomp.setSingleShot(true);

    initActionsConnections();
    initEdit();
    initComboBoxs();
    initTable();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(&rxcomp, SIGNAL(timeout()), this, SLOT(frameEnd()));
}

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

void MainWindow::initButtons()
{


}

void MainWindow::initTable()
{

}

void MainWindow::frameEnd()
{
    pkthead_t *pkt = (pkthead_t*)rxbuf.data();
    QTableWidgetItem *item;
    calib_sam_t cal;//TODO
    QString str;

    ui->tbW_display->insertRow(0);


    cal.hdr.dtype = 15;
    for (int i = 0; i < 6; i++)
    {
        cal.nval[i] = 0.5*i;
        cal.mval[i] = 0.1234 + i*0.1;
    }
    pkt = &cal.hdr;

    switch (pkt->dtype)
    {
    case 15:
        str.setNum(cal.mval[0], 'f');
        ui->tbW_display->setItem(0,2,new QTableWidgetItem(str));
        break;
    case 13:
        cal.hdr.dtype = 13;
        break;
    }
}

void MainWindow::initComboBoxs()
{

}

void MainWindow::initEdit()
{
    QRegExp regxp("^[0-9]+(.[0-9]{1,2})?$");

    ui->lEdit_value->setValidator(new QRegExpValidator(regxp, this));
    ui->lEdit_value->setMaxLength(6);
    ui->lEdit_value->setText("0.0");
}

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();

    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);

    if (serial->open(QIODevice::ReadWrite))
    {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        ui->statusBar->showMessage(tr("连接到 %1") .arg(p.name));
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
}

void MainWindow::closeSerialPort()
{
    serial->close();

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("断开连接"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Terminal"),
                       tr("The <b>Simple Terminal</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::readData()
{
    rxcomp.stop();
    rxbuf.append(serial->readAll());
    rxcomp.start(5);
}

void MainWindow::clear()
{
    while (ui->tbW_display->rowCount())
        ui->tbW_display->removeRow(0);
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionClear, SIGNAL(triggered()), this, SLOT(clear()));
}

void MainWindow::on_cBox_range_currentIndexChanged(int index)
{
    QString c[7] = {"0.0", "0.05", "0.1", "0.5", "1.0", "1.5", "4.0"};
    QString v[7] = {"0.0", "0.5", "1.0", "20.0", "50.0", "110", "220"};
    int cbindex;

    cbindex = ui->cBox_chn->currentIndex();
    if (cbindex < 4 || cbindex == 7)
    {
        ui->lEdit_value->setText(c[index]);
    }
    else
    {
        ui->lEdit_value->setText(v[index]);
    }
}

void MainWindow::on_cBox_chn_currentTextChanged(const QString &arg1)
{
    QString c[7] = {"0.0", "0.05", "0.1", "0.5", "1.0", "1.5", "4.0"};
    QString v[7] = {"0.0", "0.5", "1.0", "20.0", "50.0", "110", "220"};
    int cbindex;

    cbindex = ui->cBox_range->currentIndex();
    if (arg1.at(0) == 'I')
    {
        ui->lEdit_value->setText(c[cbindex]);
    }
    else
    {
        ui->lEdit_value->setText(v[cbindex]);
    }
}

void MainWindow::on_pBt_enter_clicked()
{
    calib_t *req;

    txbuf.resize(sizeof(calib_t));

    req = (calib_t*)txbuf.data();

    req->hdr.dtype = DT_CALIB_REQ;
    req->hdr.size  = sizeof(calib_t);
    req->hdr.sid    = ui->cBox_sensorid->currentIndex();
    req->hdr.chksum = 0;
    req->cmd = CALCMD_ENTER;

    writeData(txbuf);
}

void MainWindow::on_pBt_exit_clicked()
{

}

void MainWindow::on_pBt_calib_clicked()
{

}

void MainWindow::on_pBt_readcal_clicked()
{

}
