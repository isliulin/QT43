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
#include <QMessageBox>
#include <math.h>

static const unsigned char aucCRCHi[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const unsigned char aucCRCLo[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

static const QString nValI[7] = {"0.0", "0.05", "0.1", "1.0", "2.0", "4.0", "5.0"};

static const QString nValU[7] = {"0.0", "1.0", "5.0", "30.0", "50.0", "80", "110"};

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

unsigned short MainWindow::crc16(unsigned char *pucFrame, unsigned short usLen)
{
    unsigned char ucCRCHi = 0xFF;
    unsigned char ucCRCLo = 0xFF;
    int           iIndex;

    while(usLen--)
    {
        iIndex  = ucCRCLo ^ *(pucFrame++);
        ucCRCLo = (unsigned char)(ucCRCHi ^ aucCRCHi[iIndex]);
        ucCRCHi = aucCRCLo[iIndex];
    }

    return (unsigned short)(ucCRCHi << 8 | ucCRCLo);
}

unsigned short MainWindow::chksum(unsigned short *buf, int size)
{
    long sum = 0;

    while (size)
    {
        sum += *buf ++;
        size -= 2;
    }

    while (sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return ~sum;
}

void MainWindow::modbus(QByteArray &adu, calib_t *data)
{
    mbadu_t *mb;

    adu.resize(sizeof(mbadu_t));

    mb = (mbadu_t*)adu.data();
    mb->addr = ui->cBox_addr->currentIndex()+1;
    mb->funcode = 100;
    mb->data = *data;
    mb->crc16 = crc16((unsigned char*)mb, sizeof(mbadu_t) - 2);
}

void MainWindow::calib_do_rsp_received(calib_t *rsp)
{
    QString str;

    ui->tbW_display->insertRow(0);

    /* 支路 */
    str = ui->cBox_sensorid->itemText(rsp->hdr.sid);
    ui->tbW_display->setItem(0, 0, new QTableWidgetItem(str));
    /* 通道 */
    str = ui->cBox_chn->itemText(rsp->chn);
    ui->tbW_display->setItem(0, 1, new QTableWidgetItem(str));
    /* 量程段 */
    str = ui->cBox_seg->itemText(rsp->seg);
    ui->tbW_display->setItem(0, 2, new QTableWidgetItem(str));
    /* 测量值 */
    str.setNum(rsp->xvalue, 'f');
    ui->tbW_display->setItem(0, 3, new QTableWidgetItem(str));
    /* 标称值 */
    str.setNum(rsp->nvalue, 'f');
    ui->tbW_display->setItem(0, 4, new QTableWidgetItem(str));

    ui->tbW_display->item(0, 0)->setBackgroundColor(QColor(11, 240, 11));
    if (ui->tbW_display->rowCount() != 1)
    {
        ui->tbW_display->item(1, 0)->setBackgroundColor(QColor(255, 255, 255));
    }
}

void MainWindow::calib_sam_rsp_received(calib_sam_t *rsp)
{
    QString str;
    float *nvalue, *mvalue;

    /* 数据定位 */
    nvalue = rsp->data;
    mvalue = nvalue + rsp->seg;

    for (int i = 0; i < rsp->seg; i ++)
    {
        ui->tbW_display->insertRow(0);

        /* 支路 */
        str = ui->cBox_sensorid->itemText(rsp->hdr.sid);
        ui->tbW_display->setItem(0, 0, new QTableWidgetItem(str));
        /* 通道 */
        str = ui->cBox_chn->itemText(rsp->chn);
        ui->tbW_display->setItem(0, 1, new QTableWidgetItem(str));
        /* 量程段 */
        str.setNum(i + 1);
        ui->tbW_display->setItem(0, 2, new QTableWidgetItem(str));
        /* 测量值 */
        str.setNum(mvalue[i], 'f');
        ui->tbW_display->setItem(0, 3, new QTableWidgetItem(str));
        /* 标称值 */
        str.setNum(nvalue[i], 'f');
        ui->tbW_display->setItem(0, 4, new QTableWidgetItem(str));
    }
}

void MainWindow::frameEnd()
{
    pkthead_t *pkt;
    calib_t *cal;

    if (ui->chkB_nomb->checkState() == Qt::Checked)
    {
        pkt = (pkthead_t*)(&rxbuf.data()[0]);
        if (chksum((unsigned short*)rxbuf.data(), rxbuf.size()) != 0)
        {
            ui->statusBar->showMessage(tr("SUM校验错误"));
            goto _EXIT;
        }
    }
    else
    {
        pkt = (pkthead_t*)(&rxbuf.data()[2]);
        if (crc16((unsigned char*)rxbuf.data(), rxbuf.size()) != 0)
        {
            ui->statusBar->showMessage(tr("CRC校验错误"));
            goto _EXIT;
        }
        if (rxbuf.data()[1] != 100)
        {
            ui->statusBar->showMessage(tr("操作失败"));
            goto _EXIT;
        }
    }

    ui->statusBar->showMessage(tr("收到应答"));
    cal = (calib_t*)pkt;

    switch (pkt->dtype)
    {
    case DT_CALIB_RSP:
    {
        if (cal->cmd == CALCMD_DO)
        {
            calib_do_rsp_received(cal);
        }
        else if (cal->cmd == CALCMD_SAM_DAT)
        {
            calib_sam_rsp_received((calib_sam_t*)cal);
        }

    }
    break;
    case DT_STATUS_RSP:
    {
        status_rsp_t *rsp;

        rsp = (status_rsp_t*)pkt;
        if (rsp->dtype == DT_CALIB_REQ)
        {
            if (rsp->status == 0)
            {
                if (rsp->value == CALCMD_ENTER)
                {
                    ui->statusBar->showMessage(tr("进入校准模式"));
                }
                else if (rsp->value == CALCMD_EXIT)
                {
                    ui->statusBar->showMessage(tr("退出校准模式"));
                }
            }
        }
    }
    break;
    }


_EXIT:

    rxbuf.resize(0);
}

void MainWindow::initComboBoxs()
{

}

void MainWindow::initEdit()
{
    QRegExp regnval("^[0-9]+(.[0-9]{1,2})?$");
    QRegExp regpawd("[1-9]*[1-9][0-9]*$");

    ui->lEdit_nvalue->setValidator(new QRegExpValidator(regnval, this));
    ui->lEdit_nvalue->setMaxLength(6);
    ui->lEdit_nvalue->setText("0.0");

    ui->lEdit_pawd->setValidator(new QRegExpValidator(regpawd, this));
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
    QMessageBox::about(this, tr("关于"),
                       tr("<b>作者：</b> heyuanjie87"));
}

void MainWindow::writeData(const QByteArray &data)
{
    ui->statusBar->clearMessage();
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

void MainWindow::on_cBox_chn_currentTextChanged(const QString &arg1)
{
    int cbindex;

    cbindex = ui->cBox_seg->currentIndex();
    if (arg1.at(0) == 'I')
    {
        ui->lEdit_nvalue->setText(nValI[cbindex]);
    }
    else
    {
        ui->lEdit_nvalue->setText(nValU[cbindex]);
    }
}

void MainWindow::on_pBt_enter_clicked()
{
    calib_t req;

    req.hdr.dtype  = DT_CALIB_REQ;
    req.hdr.size   = sizeof(calib_t);
    req.hdr.sid    = ui->cBox_sensorid->currentIndex();
    req.hdr.cid    = CID_REMOTE;
    req.hdr.chksum = 0;
    req.cmd        = CALCMD_ENTER;

    req.chn        = 0;
    req.seg        = 0;
    req.nvalue     = 0.05;
    req.xvalue     = ui->lEdit_pawd->text().toFloat();

    modbus(txbuf, &req);
    writeData(txbuf);
}

void MainWindow::on_pBt_exit_clicked()
{
    calib_t req;

    req.hdr.dtype  = DT_CALIB_REQ;
    req.hdr.size   = sizeof(calib_t);
    req.hdr.sid    = ui->cBox_sensorid->currentIndex();
    req.hdr.cid    = CID_REMOTE;
    req.hdr.chksum = 0;
    req.cmd        = CALCMD_EXIT;

    req.chn        = 0;
    req.seg        = 0;
    req.nvalue     = 0;
    req.xvalue     = ui->lEdit_pawd->text().toFloat();
    req.hdr.chksum = chksum((unsigned short*)&req, sizeof(req));

    /* 不使用Modbus数据帧 */
    if (ui->chkB_nomb->checkState() == Qt::Checked)
    {
        txbuf.resize(sizeof(calib_t));
        *((calib_t*)txbuf.data()) = req;
    }
    else
    {
        modbus(txbuf, &req);
    }

    writeData(txbuf);
}

void MainWindow::on_pBt_calib_clicked()
{
    calib_t req;

    req.hdr.dtype  = DT_CALIB_REQ;
    req.hdr.size   = sizeof(calib_t);
    req.hdr.sid    = ui->cBox_sensorid->currentIndex();
    req.hdr.cid    = CID_REMOTE;
    req.hdr.chksum = 0;
    req.cmd        = CALCMD_DO;

    req.chn        = ui->cBox_chn->currentIndex();
    req.seg        = ui->cBox_seg->currentIndex();
    req.nvalue     = ui->lEdit_nvalue->text().toFloat();
    req.xvalue     = ui->lEdit_pawd->text().toFloat();

    if ((req.chn <= CHN_VABC) || (req.chn >= CHN_PA && req.chn <= CHN_PC))
    {
        /* 输入的电流/电压/功率为幅值 */
        if (ui->cBox_valtype->currentIndex() == 1)
        {
            req.nvalue /= sqrt(2);
        }
    }
    req.hdr.chksum = chksum((unsigned short*)&req, sizeof(req));

    /* 不使用Modbus数据帧 */
    if (ui->chkB_nomb->checkState() == Qt::Checked)
    {
        txbuf.resize(sizeof(calib_t));
        *((calib_t*)txbuf.data()) = req;
    }
    else
    {
        modbus(txbuf, &req);
    }

    writeData(txbuf);
}

void MainWindow::on_pBt_readcal_clicked()
{
    calib_t req;

    req.hdr.dtype  = DT_CALIB_REQ;
    req.hdr.size   = sizeof(calib_t);
    req.hdr.sid    = ui->cBox_sensorid->currentIndex();
    req.hdr.cid    = CID_REMOTE;
    req.hdr.chksum = 0;
    req.cmd        = CALCMD_SAM_GET;

    req.chn        = ui->cBox_chn->currentIndex();
    req.seg        = 0;
    req.nvalue     = 0;
    req.xvalue     = ui->lEdit_pawd->text().toFloat();

    modbus(txbuf, &req);
    writeData(txbuf);
}

void MainWindow::on_cBox_seg_currentIndexChanged(int index)
{
    int cbindex;

    cbindex = ui->cBox_chn->currentIndex();
    if (cbindex < 4 || cbindex == 7)
    {
        ui->lEdit_nvalue->setText(nValI[index]);
    }
    else
    {
        ui->lEdit_nvalue->setText(nValU[index]);
    }
}
