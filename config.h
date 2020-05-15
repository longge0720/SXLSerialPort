#ifndef CONFIG_H
#define CONFIG_H

class QString;
class Config
{
public:
    //全局配置参数
    static QString SerialPortStu;       //是否打开串口
    static QString Com_Port;            //端口
    static QString Com_ByteSize;        //数据位
    static int Com_BaudRate;            //波特率
    static QString Com_parity;         //奇偶校验
    static QString Com_StopBits;       //停止位
    static QString Com_FlowControl;       //停止位

public:
    //读写配置参数及其他操作
    static void ReadConfig();           //读取配置参数
    static void WriteConfig();          //写入配置参数
    static void NewConfig();            //以初始值新建配置文件
    static bool CheckConfig();          //校验配置文件

};

#endif // CONFIG_H

