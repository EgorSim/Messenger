#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include <QObject>
#include <QTcpSocket>

#include "mainwindow.h"
#include "SignIn.h"

class ClientServer : public QObject
{
    Q_OBJECT
public:
    explicit ClientServer(MainWindow* guiMainWindow, QObject *parent = nullptr);

private:
    MainWindow* guiMainWindow;

    QTcpSocket* tcpSocket;
    quint16 blockSize;

    enum TypeOfInputBlock {
        SIGNIN = 5,
        SIGNINRESPONSE,
        FRIEND,
        MESSEGE,
        SEARCH,
        REQUEST,
        REQUSTRESPONSE
    };



signals:
    void serverError(QString errorMessege);
    void newFriend(QString login);
    void messegeRecieved(QString dialog, bool isMy, QString text);
    void searchResult(QString searchLogin);
    void request(QString requestLogin);

public slots:
    void slotAuthorizationAttempt(SignIn info);
    void slotSendMessege(QString to, QString text);
    void slotSearch(QString searchLogin);
    void slotRequest(QString requestLogin);
    void slotRequestResponse(QString login, bool isAccept);
    void slotReadyRead();
};

#endif // CLIENTSERVER_H
