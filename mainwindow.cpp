#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include "config.h"
#include <QTime>
#include <QDataStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   //设置固定大小
    int width = this->geometry().width();
    int height = this->geometry().height();
    this->setFixedSize(width,height);
    this->setWindowTitle(QString("Wisdom-ZD:嵌入式软件"));

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
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    ui->rbtnRcvASCII->setChecked(true);
    ui->rbtnSndASCII->setChecked(true);
    ui->btnClose->setDisabled(true);
    ui->btnSend->setEnabled(false);
    ui->chkRepeatSnd->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::serialPort_readyRead()
{
    QString nowTime = NULL;

    //从接收缓冲区中读取数据
    QString head = "[receive:] ";
    QByteArray buffer = m_serialPort.readAll();


//    buffer=head+buffer;
    if(ui->chxShowTime->isChecked())
    {
        QTime time(QTime::currentTime());
        nowTime = time.toString("hh:mm:ss:zzz");
        nowTime = "[ "+nowTime+" ] ";
    }
    //从界面中读取以前收到的数据
    QString recv = ui->rcvTxtEdit->toPlainText();
    recv+=head;
    recv+=nowTime;
    if(ui->rbtnRcvHEX->isChecked())
    {
        QDataStream out(&buffer,QIODevice::ReadWrite);
        while(!out.atEnd())
        {
            qint8 outChar = 0;
            out>>outChar;
            QString str = QString("%1").arg(outChar&0xff,2,16,QLatin1Char('0'));
            recv+=str;
            recv+=" ";

        }

    }else
    {
        recv += QString(buffer);
    }
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
        ui->btnOpen->setDisabled(true);
        ui->btnClose->setEnabled(true);
        ui->chkRepeatSnd->setEnabled(true);

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

    }

    if(m_serialPort.isOpen())
    {
         ui->btnSend->setEnabled(true);
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
    QString nowTime = NULL;
    QByteArray sendData;
    QByteArray showData;
    QString str = ui->sndTxtEdit->document()->toPlainText();
    showData = str.toUtf8();
    if(ui->rbtnSndHEX->isChecked())
    {
        StringToHex(str, sendData);
//        sendData = HexStringToByteArray(str);
    }else
    {
         sendData = str.toUtf8();
    }


    m_serialPort.write(sendData);
    ui->cbxSndHistory->addItem(showData);

    if(ui->chxShowSend->isChecked())
    {
        QString head = "[send:] ";
        QString recv = ui->rcvTxtEdit->toPlainText();
        if(ui->chxShowTime->isChecked())
        {
            QTime time(QTime::currentTime());
            nowTime = time.toString("hh:mm:ss:zzz");
            nowTime = "[ "+nowTime+" ] ";
        }
        recv+=head;
        recv+=nowTime;
        recv += QString(showData);
        if(ui->chkAutoLine->isChecked())
        {
            recv += QString("\n");
        }

        //清空以前的显示
        ui->rcvTxtEdit->clear();
        //重新显示
        ui->rcvTxtEdit->append(recv);
    }
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
    ui->btnSend->setEnabled(false);
    ui->chkRepeatSnd->setEnabled(false);

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


void MainWindow::saveFile()
{
    QString path = QFileDialog::getSaveFileName(this,
                                                tr("Open File"),
                                                ".",
                                                tr("Text Files(*.txt)"));
    if(!path.isEmpty()){
        QFile file(path);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::warning(this, tr("Write File"),
                                 tr("Can't open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << ui->rcvTxtEdit->toPlainText();
        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"),
                             tr("You did not select any file."));
    }
}


void MainWindow::on_btnSaveRcv_clicked()
{
    saveFile();
}

void MainWindow::onTimeout()
{
    on_btnSend_clicked();
}

void MainWindow::on_chkRepeatSnd_stateChanged(int arg1)
{
    if(ui->chkRepeatSnd->isChecked())
    {
        if(m_serialPort.isOpen())
        {
            int timeSpace = ui->sbTimeInterval->value();
            if( (timeSpace<20) || (timeSpace >9999) )
            {
                QMessageBox::warning(this, tr("TimeError"),
                                     tr("Time interval: 20 < time < 9999 "));
                return ;
            }
            else
            {
                timer.start(timeSpace);
            }
        }


    }
    else
    {
        timer.stop();
    }
}




char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}


void MainWindow::StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
//        hstr=str[i].toAscii();
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
//        lstr = str[i].toAscii();
         lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}



//这个中间必须有空格
QByteArray MainWindow::HexStringToByteArray(QString HexString)
 {
     bool ok;
     QByteArray ret;
     HexString = HexString.trimmed();
     HexString = HexString.simplified();
     QStringList sl = HexString.split(" ");

     foreach (QString s, sl) {
         if(!s.isEmpty()) {
             char c = s.toInt(&ok,16)&0xFF;
             if(ok){
                 ret.append(c);
             }else{
                 qDebug()<<"非法的16进制字符："<<s;
             }
         }
     }
     return ret;
 }
