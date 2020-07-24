#include "config.h"
#include <QString>
#include <QtWidgets>
#include <QDir>


QString Config::Com_Port = "COM1";           //串口
QString Config::SerialPortStu = "open";   //是否打开串口
QString Config::Com_ByteSize = "8bit";      //数据位

QString Config::Com_parity = "None";     //奇偶校验
int Config::Com_BaudRate = 9600;        //波特率
QString Config::Com_StopBits = "1bit";   //停止位
QString Config::Com_FlowControl = "None";   //停止位


void Config::ReadConfig()
{

    if (!CheckConfig())
    {
        return;
    }
    QString fileName;
    //QDir dir;
    //fileName = dir.currentPath() + "/my.ini";
     fileName = QString("D:/SxlSerialPort/my.ini");
    QSettings set(fileName, QSettings::IniFormat);

    set.beginGroup("Serial_PortStu_config");
    Config::Com_Port = set.value("Com_Port").toString();
    Config::SerialPortStu = set.value("SerialPortStu").toString();
    Config::Com_BaudRate = set.value("Com_BaudRate").toInt();
    Config::Com_ByteSize = set.value("Com_ByteSize").toString();
    Config::Com_parity = set.value("Com_Parity").toString();
    Config::Com_StopBits = set.value("Com_StopBits").toString();
    Config::Com_FlowControl = set.value("Com_FlowControl").toString();
    set.endGroup();
}

void Config::WriteConfig()
{
    QString fileName;
    QDir dir;
//    fileName = dir.currentPath() + "/my.ini";
    fileName = QString("D:/SxlSerialPort/my.ini");
    QSettings set(fileName, QSettings::IniFormat);

    set.beginGroup("Serial_PortStu_config");
    set.setValue("Com_Port",Config::Com_Port);
    set.setValue("SerialPortStu",Config::SerialPortStu);
    set.setValue("Com_BaudRate",Config::Com_BaudRate);
    set.setValue("Com_ByteSize",Config::Com_ByteSize);
    set.setValue("Com_Parity",Config::Com_parity);
    set.setValue("Com_StopBits",Config::Com_StopBits);
    set.setValue("Com_FlowControl",Config::Com_FlowControl);
    set.endGroup();
}

void Config::NewConfig()
{

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#endif
    WriteConfig();

}

bool Config::CheckConfig()
{
//    QString fileName = QString("%1/%2_Config.ini").arg(ConfigPath).arg(ConfigName);
    QString fileName;
    QDir dir;
//    fileName = dir.currentPath() + "/my.ini";
    fileName = QString("D:/SxlSerialPort/my.ini");
    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(fileName);

    if (file.size() == 0)
    {
        NewConfig();
        return false;
    }

    //如果配置文件不完整,则以初始值继续运行,并生成配置文件
    if (file.open(QFile::ReadOnly))
    {
        bool ok = true;

        while (!file.atEnd())
        {
            QString line = file.readLine();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            QStringList list = line.split("=");

            if (list.count() == 2) {
                if (list.at(1) == "") {
                    ok = false;
                    break;
                }
            }
        }

        if (!ok)
        {
            NewConfig();
            return false;
        }
    }
    else
    {
        NewConfig();
        return false;
    }
    return true;
}

