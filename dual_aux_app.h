#ifndef DUAL_AUX_APP_H
#define DUAL_AUX_APP_H

#include <QMainWindow>
#include "auto_detect_uart.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class dual_aux_app;
}
QT_END_NAMESPACE

class dual_aux_app : public QMainWindow{
    Q_OBJECT
private:
    QSerialPort *ser_port;
    auto_detect_uart *autodetect_thread;

public:
    void disable_controls();
    void enable_controls();
    dual_aux_app(QWidget *parent = nullptr);
    ~dual_aux_app();

private slots:
    void on_ch0_pushbutton_clicked();
    void on_ch1_pushbutton_clicked();
    void on_actionDetect_triggered();
    void on_actionExit_2_triggered();
    void on_uart_device_search(bool found, QSerialPortInfo dev);
    void on_uart_error(QSerialPort::SerialPortError error);
    void on_uart_ready_read();

private:
    Ui::dual_aux_app *ui;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};
#endif // DUAL_AUX_APP_H
