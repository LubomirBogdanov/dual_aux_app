#ifndef DUAL_AUX_APP_H
#define DUAL_AUX_APP_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class dual_aux_app;
}
QT_END_NAMESPACE

class dual_aux_app : public QMainWindow
{
    Q_OBJECT

public:
    dual_aux_app(QWidget *parent = nullptr);
    ~dual_aux_app();

private slots:
    void on_ch0_pushbutton_clicked();

    void on_ch1_pushbutton_clicked();

    void on_actionDetect_triggered();

    void on_actionExit_2_triggered();

private:
    Ui::dual_aux_app *ui;
};
#endif // DUAL_AUX_APP_H
