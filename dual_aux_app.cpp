#include "dual_aux_app.h"
#include "ui_dual_aux_app.h"

dual_aux_app::dual_aux_app(QWidget *parent) : QMainWindow(parent), ui(new Ui::dual_aux_app){
    QString temp_str(IDN_STRING);
    ui->setupUi(this);

    autodetect_thread.set_device_to_search_for(temp_str);
    autodetect_thread.start();
}

dual_aux_app::~dual_aux_app(){
    delete ui;
}

void dual_aux_app::on_ch0_pushbutton_clicked(){
    qDebug() << "(dual_aux_app) on_ch0_pushbutton_clicked";
}


void dual_aux_app::on_ch1_pushbutton_clicked(){
    qDebug() << "(dual_aux_app) on_ch1_pushbutton_clicked";
}


void dual_aux_app::on_actionDetect_triggered(){
    qDebug() << "(dual_aux_app) on_actionDetect_triggered";
}


void dual_aux_app::on_actionExit_2_triggered(){
    qDebug() << "(dual_aux_app) on_actionExit_2_triggered";
}

