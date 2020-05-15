#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include "config.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   //设置固定大小
    int width = this->geometry().width();
    int height = this->geometry().height();
    this->setFixedSize(width,height);

    //初始化串口端口号列表
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        // Example use QSerialPort
        Available_serial_ports = info.portName();
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
        //    ui->PortBox->addItem(info.portName());
            serial.close();
        }
    }
    this->initConfig();


    QObject::connect(&m_serialPort, &QSerialPort::readyRead, this, &MainWindow::serialPort_readyRead);
}
void MainWindow::initUi()
{

}
void MainWindow::initConfig()
{
    Config::ReadConfig();

    QStringList port_arg;
    port_arg << "COM1" << "COM2" << "COM3" << "COM4" << "COM5" << "COM6" << "COM7" << "COM8" << "COM9" << "COM10"<<"COM11" << "COM12" << "COM13" << "COM14" << "COM15" << "COM16" << "COM17" << "COM18" << "COM19" << "COM20" ;
    ui->cbxPortName->addItems(port_arg);
    ui->cbxPortName->setCurrentIndex(ui->cbxPortName->findText((Config::Com_Port)));
    connect(ui->cbxPortName, SIGNAL(currentIndexChanged(int)),this,SLOT(saveConfig()));

    QStringList BaudRate_arg;
    BaudRate_arg << "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";
    ui->cbxBaudRate->addItems(BaudRate_arg);
    ui->cbxBaudRate->setCurrentIndex(ui->cbxBaudRate->findText(QString::number(Config::Com_BaudRate)));
    connect(ui->cbxBaudRate, SIGNAL(currentIndexChanged(int)),this,SLOT(saveConfig()));

    QStringList ByteSize_arg;
    ByteSize_arg << "5bit" << "6bit" << "7bit" << "8bit";
    ui->cbxDataBits->addItems(ByteSize_arg);
    ui->cbxDataBits->setCurrentIndex(ui->cbxDataBits->findText((Config::Com_ByteSize)));
    connect(ui->cbxDataBits, SIGNAL(currentIndexChanged(int)),this,SLOT(saveConfig()));

    QStringList StopBits_arg;
    StopBits_arg << "1bit" << "1.5bit"<< "2bit";
    ui->cbxStopBits->addItems(StopBits_arg);
    ui->cbxStopBits->setCurrentIndex(ui->cbxStopBits->findText((Config::Com_StopBits)));
    connect(ui->cbxStopBits, SIGNAL(currentIndexChanged(int)),this,SLOT(saveConfig()));

    QStringList Parity_arg;
    Parity_arg << "None" << "Even" << "Old" << "Space" << "Mark";
    ui->cbxParity->addItems(Parity_arg);
    ui->cbxParity->setCurrentIndex(ui->cbxParity->findText((Config::Com_parity)));
    connect(ui->cbxParity, SIGNAL(currentIndexChanged(int)),this,SLOT(saveConfig()));

    QStringList FlowControl_arg;
    FlowControl_arg << "None" << "RTS/CTS" << "XON/XOFF";
    ui->cbxFlowControl->addItems(FlowControl_arg);
    ui->cbxFlowControl->setCurrentIndex(ui->cbxFlowControl->findText((Config::Com_FlowControl)));
    connect(ui->cbxFlowControl, SIGNAL(currentIndexChanged(int)),this,SLOT(saveConfig()));

    //默认情况下不打开任何串口
    if(Config::SerialPortStu == "open")
    {
//        on_btnOpen_clicked();
    }
    else
    {
        Config::SerialPortStu = "off";
    }
    connect(ui->btnOpen, SIGNAL(clicked(bool)),this,SLOT(saveConfig()));
    ui->rbtnRcvASCII->setChecked(true);
    ui->rbtnSndASCII->setChecked(true);
    ui->btnClose->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::serialPort_readyRead()
{
    //从接收缓冲区中读取数据
    QByteArray buffer = m_serialPort.readAll();
    //从界面中读取以前收到的数据
    QString recv = ui->rcvTxtEdit->toPlainText();
    recv += QString(buffer);
    if(ui->chkAutoLine->isChecked())
    {
        recv += QString("\n");
    }
    //清空以前的显示
    ui->rcvTxtEdit->clear();
    //重新显示
    ui->rcvTxtEdit->append(recv);
}

void MainWindow::on_btnOpen_clicked()
{
    static bool btnflag = false;

    if(!m_serialPort.isOpen())
    {
        m_serialPort.setPortName(ui->cbxPortName->currentText());
        if(m_serialPort.open(QIODevice::ReadWrite) != true)
        {
            QMessageBox::information(this,tr("error"),tr("串口打开失败"));
            return;
        }

        m_serialPort.setBaudRate(ui->cbxBaudRate->currentText().toInt());
        int baudRate = ui->cbxBaudRate->currentIndex();
        switch(baudRate)
        {
        case 0:
            m_serialPort.setBaudRate(QSerialPort::Baud1200);
            break;
        case 1:
            m_serialPort.setBaudRate(QSerialPort::Baud2400);
            break;
        case 2:
            m_serialPort.setBaudRate(QSerialPort::Baud4800);
            break;
        case 3:
            m_serialPort.setBaudRate(QSerialPort::Baud9600);
            break;
        case 4:
            m_serialPort.setBaudRate(QSerialPort::Baud19200);
            break;
        case 5:
            m_serialPort.setBaudRate(QSerialPort::Baud38400);
            break;
        case 6:
            m_serialPort.setBaudRate(QSerialPort::Baud57600);
            break;
        case 7:
            m_serialPort.setBaudRate(QSerialPort::Baud115200);
            break;
        }

        int databits = ui->cbxDataBits->currentIndex();
        switch(databits)
        {
        case 0:
            m_serialPort.setDataBits(QSerialPort::Data5);
            break;
        case 1:
            m_serialPort.setDataBits(QSerialPort::Data6);
            break;
        case 2:
            m_serialPort.setDataBits(QSerialPort::Data7);
            break;
        case 3:
            m_serialPort.setDataBits(QSerialPort::Data8);
            break;
        }

        int stopBits = ui->cbxStopBits->currentIndex();
        switch(stopBits)
        {
        case 0:
            m_serialPort.setStopBits(QSerialPort::OneStop);
            break;
        case 1:
            m_serialPort.setStopBits(QSerialPort::OneAndHalfStop);
            break;
        case 2:
            m_serialPort.setStopBits(QSerialPort::TwoStop);
            break;
        default:
            break;
        }

        int parity = ui->cbxParity->currentIndex();
        switch (parity)
        {
        case 0:
            m_serialPort.setParity(QSerialPort::NoParity);
            break;
        case 1:
            m_serialPort.setParity(QSerialPort::OddParity);
            break;
        case 2:
            m_serialPort.setParity(QSerialPort::EvenParity);
        default:
            break;
        }

        m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

        ui->cbxPortName->setEnabled(false);
        ui->cbxBaudRate->setEnabled(false);
        ui->cbxDataBits->setEnabled(false);
        ui->cbxParity->setEnabled(false);
        ui->cbxStopBits->setEnabled(false);
         ui->cbxFlowControl->setEnabled(false);
        btnflag = true;
        ui->btnOpen->setDisabled(true);
        ui->btnClose->setEnabled(true);

    }
    else
    {
        m_serialPort.close();
        ui->btnClose->setDisabled(true);
        ui->btnOpen->setEnabled(true);

        ui->cbxPortName->setEnabled(true);
        ui->cbxBaudRate->setEnabled(true);
        ui->cbxDataBits->setEnabled(true);
        ui->cbxParity->setEnabled(true);
        ui->cbxStopBits->setEnabled(true);
        ui->cbxFlowControl->setEnabled(true);
        btnflag = false;
    }

    ui->btnClose->setEnabled(true);
    ui->btnOpen->setDisabled(true);
    ui->btnRefreshPort->setEnabled(false);
}

void MainWindow::saveConfig()
{
    Config::Com_Port = ui->cbxPortName->currentText();
    Config::Com_BaudRate = ui->cbxBaudRate->currentText().toInt();
    Config::Com_ByteSize = ui->cbxDataBits->currentText();

    Config::SerialPortStu = "open" ;

    Config::Com_StopBits = ui->cbxStopBits->currentText();
    Config::Com_parity = ui->cbxParity->currentText();

    Config::WriteConfig();
}

void MainWindow::on_btnSend_clicked()
{
    QString str = ui->sndTxtEdit->document()->toPlainText();
    QByteArray sendData = str.toUtf8();
    m_serialPort.write(sendData);
    ui->cbxSndHistory->addItem(sendData);
}

void MainWindow::on_btnClose_clicked()
{
    if(!m_serialPort.isOpen())//串口关闭
    {
        QMessageBox::information(this,tr("error"),tr("串口关闭失败，串口未打开！"));
        ui->btnRefreshPort->setEnabled(true);

    }
    else//串口打开
    {
        m_serialPort.close();
        ui->cbxPortName->setEnabled(true);
        ui->cbxBaudRate->setEnabled(true);
        ui->cbxDataBits->setEnabled(true);
        ui->cbxParity->setEnabled(true);
        ui->cbxStopBits->setEnabled(true);
        ui->cbxFlowControl->setEnabled(true);
        ui->btnRefreshPort->setEnabled(false);


    }
    ui->btnClose->setDisabled(true);
    ui->btnOpen->setEnabled(true);
     ui->btnRefreshPort->setEnabled(true);
}

void MainWindow::on_btnClearRcv_clicked()
{
    //清空以前的显示
    ui->rcvTxtEdit->clear();
}

void MainWindow::on_btnRefreshPort_clicked()
{
    ui->cbxPortName->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        // Example use QSerialPort
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            ui->cbxPortName->addItem(info.portName());
            serial.close();
        }
    }
}

void MainWindow::on_cbxPortName_activated(const QString &arg1)
{
    on_btnRefreshPort_clicked();
}
