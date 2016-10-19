#include "SenOpticalFlowSensorsMainWindow.h"

#include "SenMotionTrackingWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    SenMotionTrackingWidget motionTrackingWidget;
//    motionTrackingWidget.show();
    SenOpticalFlowSensorsMainWindow myMainWindow (0, QString("Welcome to "));
    myMainWindow.setWindowTitle(QString("Welcome to Sen Optical-Flow World"));
    myMainWindow.show();

    return a.exec();
}
