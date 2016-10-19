#ifndef SENCONSOLE_H
#define SENCONSOLE_H

#include <QPlainTextEdit>
#include <QScrollBar>
#include <QtCore/QDebug>

/****** Sen Start *******/
#include <QTime>
#include <QCoreApplication>
/****** Sen End *******/

class SenConsole : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void getData(const QByteArray &data);

public:
    explicit SenConsole(QWidget *parent = 0);
    void putData(const QByteArray &data, int colorNumber);
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

#endif // CONSOLE_H
