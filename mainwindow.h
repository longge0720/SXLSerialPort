#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>        //提供访问串口的功能
#include <QSerialPortInfo>    //提供系统中存在的串口的信息
#include <QTimer>
#include "mytools.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void testSig(void);
    void testSig(int);

private slots:
    void on_btnOpen_clicked();

    void on_btnSend_clicked();
    void serialPort_readyRead();

    void on_btnClose_clicked();

    void saveConfig();
    void on_btnClearRcv_clicked();

    void on_btnRefreshPort_clicked();

    void on_cbxPortName_activated(const QString &arg1);

    void on_btnSaveRcv_clicked();
    void onTimeout();

    void on_chkRepeatSnd_stateChanged(int arg1);

    void on_actionCalcBinSum_triggered();

private:
    Ui::MainWindow *ui;
    QSerialPort m_serialPort;
    QString Available_serial_ports;
    QTimer timer;
    MyTools myTools;
    void initUi();
    void initConfig();
    void saveFile();
    char ConvertHexChar(char ch);
    void StringToHex(QString str, QByteArray &senddata);
    QByteArray HexStringToByteArray(QString HexString);
};
#endif // MAINWINDOW_H
