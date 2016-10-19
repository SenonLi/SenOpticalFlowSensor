#include "SenOpticalFlowSensorsMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SenOpticalFlowSensorsMainWindow myMainWindow;
    myMainWindow.setWindowTitle(QString("Sen Pan Tilt Unit User Interface"));
    myMainWindow.show();

    return a.exec();
}
