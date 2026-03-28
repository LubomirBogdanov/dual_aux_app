#ifndef AUTO_DETECT_UART_H
#define AUTO_DETECT_UART_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QFileInfo>
#include <QDebug>

#define DEVICE_CMD_IDN "*IDN?\n"
#define AUTODETECT_DEVICE_TIMEOUT 100 //in ms
#define AUTODETECT_DEVICE_BAUD QSerialPort::Baud9600

class auto_detect_uart : public QThread{
    Q_OBJECT
private:
    QSerialPort *uart_dev;
    QString device_to_search_for;
    void run() override;

public:
    auto_detect_uart();
    ~auto_detect_uart();
    void set_device_to_search_for(QString &dev_name);

signals:
    void uart_device_search(bool found, QString dev_name);
};

#endif // AUTO_DETECT_UART_H
