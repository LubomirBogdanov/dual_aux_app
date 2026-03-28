#include "auto_detect_uart.h"

auto_detect_uart::auto_detect_uart(){
    uart_dev = NULL;
}

auto_detect_uart::~auto_detect_uart(){
    if(uart_dev){
        delete uart_dev;
    }
}

void auto_detect_uart::set_device_to_search_for(QString &dev_name){
    device_to_search_for = dev_name;
}

void auto_detect_uart::run(){
    bool search_result = 0;
    QString cmd;
    QByteArray msg_buff;
    QString msg_buff_str;

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    qDebug() << "Ports present:";
    for (int i = 0; i < ports.size(); ++i) {
        qDebug() << ports.at(i).portName();
    }

    qDebug()<<"(auto_detect_uart) Starting device auto-detect ...";

    uart_dev = new QSerialPort;

    if(uart_dev){
        uart_dev->setBaudRate(AUTODETECT_DEVICE_BAUD);
        uart_dev->setDataBits(QSerialPort::Data8);
        uart_dev->setFlowControl(QSerialPort::NoFlowControl);
        uart_dev->setParity(QSerialPort::NoParity);
        uart_dev->setStopBits(QSerialPort::OneStop);

        for(int i = 0; i < ports.size(); i++){
            uart_dev->setPort(ports.at(i));

            if(uart_dev->open(QIODevice::ReadWrite)){
                cmd = DEVICE_CMD_IDN;
                qDebug()<<"(auto_detect_uart) tx: " << cmd.toUtf8();
                uart_dev->write(cmd.toUtf8(), cmd.size());
                uart_dev->flush();
                while(uart_dev->waitForReadyRead(100) || uart_dev->canReadLine()){
                    msg_buff.append(uart_dev->readAll());
                }

                if(msg_buff.contains("Wrong command!")){
                    qDebug()<<"(auto_detect_uart) Attempting again ...";
                    qDebug()<<"(auto_detect_uart) tx: " << cmd.toUtf8();
                    msg_buff.resize(0);
                    uart_dev->write(cmd.toUtf8(), cmd.size());
                    uart_dev->flush();
                    while(uart_dev->waitForReadyRead(100) || uart_dev->canReadLine()){
                        msg_buff.append(uart_dev->readAll());
                    }
                }

                qDebug()<<"(auto_detect_uart) rx: "<<msg_buff;
                msg_buff_str = msg_buff;

                if(msg_buff_str.contains(device_to_search_for)){
                    qDebug()<<"(auto_detect_uart) Device "<<device_to_search_for<<" found!";
                    search_result = 1;
                    emit uart_device_search(true, ports.at(i).portName());
                }

                msg_buff.resize(0);

                uart_dev->close();
            }
            else{
                qDebug()<<"(auto_detect_uart) Can't open: " << uart_dev->portName();
            }
        }
    }
    else{
        qDebug()<<"(auto_detect_uart) Failed to create QSerialPort object!";
    }

    if(!search_result){
        emit uart_device_search(false, "");
    }
}
