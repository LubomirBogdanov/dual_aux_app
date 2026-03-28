#include "dual_aux_app.h"

#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    dual_aux_app w;
    w.show();
    return a.exec();
}
