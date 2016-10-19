#ifndef SENCONSOLE_H
#define SENCONSOLE_H

#include <QPlainTextEdit>
#include <QScrollBar>
#include <QtCore/QDebug>

/****** Sen Start *******/
#include <QTime>
#include <QCoreApplication>

#include <QGroupBox>
#include <QSlider>
/****** Sen End *******/

class SenConsole : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void getData(const QByteArray &data);

    void emitStepUp();
    void emitStepDown();
    void emitStepLeft();
    void emitStepRight();

    void emitStep10Up();
    void emitStep10Down();
    void emitStep10Left();
    void emitStep10Right();

public:
    explicit SenConsole(QWidget *parent = 0);
    void putData(const QByteArray &data);
    void setLocalEchoEnabled(bool set);

    /****** Sen Start *******/
    bool lineDebugActive;

    void SenDelay(int msec);
    void SenDeadDelay(int msec);
    void SenKeyPressBlocking();
    /****** Sen End *******/

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *e);

private:
    bool localEchoEnabled;

    /****** Sen Start *******/
    bool keyPressBlockingActive;
    void SenKeyPressBlockingControlTest(const QByteArray &data);

    QTextCharFormat zeroSerialCharFormat, firstSerialCharFormat, secondSerialCharFormat;

    /****** Sen End *******/
};

class SenSlidersConsole : public QGroupBox
{
    Q_OBJECT

public:
    SenSlidersConsole(const QString &title,
                 QWidget *parent = 0);

signals:
    void valueChanged(int value);

public slots:
//    void setValue(int value);
//    void setMinimum(int value);
//    void setMaximum(int value);
//    void invertAppearance(bool invert);
//    void invertKeyBindings(bool invert);

private:
    SenConsole *mySenConsole;
    QSlider *verticalSlider, *horizontalSlider;
    QScrollBar *scrollBar;
};

#endif // CONSOLE_H
