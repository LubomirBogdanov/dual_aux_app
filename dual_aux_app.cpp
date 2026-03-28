#include "dual_aux_app.h"
#include "ui_dual_aux_app.h"
#include "common.h"

dual_aux_app::dual_aux_app(QWidget *parent) : QMainWindow(parent), ui(new Ui::dual_aux_app){
    QString temp_str(IDN_STRING);
    ui->setupUi(this);

    ser_port = nullptr;

    ui->ch0_radiobutton->setFocusPolicy(Qt::NoFocus);
    ui->ch0_radiobutton->installEventFilter(this);
    ui->ch1_radiobutton->setFocusPolicy(Qt::NoFocus);
    ui->ch1_radiobutton->installEventFilter(this);

    connect(&autodetect_thread, SIGNAL(uart_device_search(bool, QSerialPortInfo)), this, SLOT(on_uart_device_search(bool, QSerialPortInfo)));
    autodetect_thread.set_device_to_search_for(temp_str);
    autodetect_thread.start();

    this->setWindowTitle(APPLICATION_NAME);
}

dual_aux_app::~dual_aux_app(){
    if(ser_port){
        ser_port->close();
        delete ser_port;
    }

    delete ui;
}

void dual_aux_app::on_uart_device_search(bool found, QSerialPortInfo dev){
    qDebug() << "(dual_aux_app) Search result: " << found << ", dev_name: " << dev.portName();

    if(found){
        if(!ser_port){
            ser_port = new QSerialPort(this);
            ser_port->setBaudRate(AUTODETECT_DEVICE_BAUD);
            ser_port->setDataBits(QSerialPort::Data8);
            ser_port->setFlowControl(QSerialPort::NoFlowControl);
            ser_port->setParity(QSerialPort::NoParity);
            ser_port->setStopBits(QSerialPort::OneStop);
            ser_port->setPort(dev);

            if(ser_port->open(QIODevice::ReadWrite)){
                qDebug() << "(dual_aux_app) Port: " << dev.portName() << " opened successfully!";

                connect(ser_port, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(on_uart_error(QSerialPort::SerialPortError)));
                connect(ser_port, SIGNAL(readyRead()), this, SLOT(on_uart_ready_read()));
                ui->ch0_pushbutton->setDisabled(0);
                ui->ch1_pushbutton->setDisabled(0);

                qDebug()<<"(dual_aux_app) tx: " << CH_GET_STRING;
                ser_port->write(CH_GET_STRING, strlen(CH_GET_STRING));
                ser_port->flush();
            }
            else{
                qDebug() << "(dual_aux_app) Failed to open port: " << dev.portName() << "!";
                delete ser_port;
                ser_port = nullptr;
                ui->ch0_pushbutton->setDisabled(1);
                ui->ch1_pushbutton->setDisabled(1);
            }
        }
        else{
            ui->ch0_pushbutton->setDisabled(1);
            ui->ch1_pushbutton->setDisabled(1);
        }
    }
}

void dual_aux_app::on_uart_error(QSerialPort::SerialPortError error){
    if (error == QSerialPort::ResourceError || error == QSerialPort::DeviceNotFoundError) {
        qDebug() << "(dual_aux_app) Device disconnected!";

        ui->ch0_pushbutton->setDisabled(1);
        ui->ch1_pushbutton->setDisabled(1);
        ui->ch0_pushbutton->setAutoExclusive(0);
        ui->ch1_radiobutton->setAutoExclusive(0);
        ui->ch0_radiobutton->setChecked(0);
        ui->ch1_radiobutton->setChecked(0);
        ui->ch0_pushbutton->setAutoExclusive(1);
        ui->ch1_radiobutton->setAutoExclusive(1);

        if (ser_port->isOpen()){
            ser_port->close();
            delete ser_port;
            ser_port = nullptr;
        }
    }
}

void dual_aux_app::on_uart_ready_read(){
    static QByteArray buffer;
    int index;
    QString reply;

    buffer.append(ser_port->readAll());

    //Parsing with while is ChatGPT generated!
    while ((index = buffer.indexOf('\n')) != -1) {
        QByteArray line = buffer.left(index);
        buffer.remove(0, index + 1);

        qDebug() << "(dual_aux_app) Line received:" << line;

        reply = QString::fromUtf8(line);

        if(reply.contains(CH0_SELECTED_STRING)){
            qDebug() << "(dual_aux_app) Reply: " << CH0_SELECTED_STRING;
            ui->ch0_radiobutton->setChecked(1);
            ui->ch1_radiobutton->setChecked(0);
        }

        if(reply.contains(CH1_SELECTED_STRING)){
            qDebug() << "(dual_aux_app) Reply: " << CH1_SELECTED_STRING;
            ui->ch0_radiobutton->setChecked(0);
            ui->ch1_radiobutton->setChecked(1);
        }

        if(reply.contains(SAVING_IN_EEPROM)){
            qDebug() << "(dual_aux_app) Reply: " << SAVING_IN_EEPROM;
        }
    }
}

//This entire method is ChatGPT generated!
bool dual_aux_app::eventFilter(QObject *obj, QEvent *event){
    if (obj == ui->ch0_radiobutton) {
        if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick) {
            return true;
        }
    }

    if (obj == ui->ch1_radiobutton) {
        if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick) {
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

void dual_aux_app::on_ch0_pushbutton_clicked(){
    qDebug() << "(dual_aux_app) on_ch0_pushbutton_clicked";

    qDebug()<<"(dual_aux_app) tx: " << CH0_SELECT_STRING;
    ser_port->write(CH0_SELECT_STRING, strlen(CH0_SELECT_STRING));
    ser_port->flush();
}

void dual_aux_app::on_ch1_pushbutton_clicked(){
    qDebug() << "(dual_aux_app) on_ch1_pushbutton_clicked";

    qDebug()<<"(dual_aux_app) tx: " << CH0_SELECT_STRING;
    ser_port->write(CH1_SELECT_STRING, strlen(CH0_SELECT_STRING));
    ser_port->flush();
}

void dual_aux_app::on_actionDetect_triggered(){
    qDebug() << "(dual_aux_app) on_actionDetect_triggered";
    autodetect_thread.start();
}

void dual_aux_app::on_actionExit_2_triggered(){
    qDebug() << "(dual_aux_app) on_actionExit_2_triggered";
    close();
}

