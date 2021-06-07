#ifndef MESSENGERSERVER_H
#define MESSENGERSERVER_H

#include "messengerdatabase.h"
#include "SignIn.h"
#include "SignInResponse.h"

#include <QObject>
#include <QTcpServer>
#include <QMap>

class MessengerServer : public QObject
{
    Q_OBJECT
public:
    explicit MessengerServer(int port, QObject *parent = nullptr);
    ~MessengerServer();
private:
    QTcpServer* tcpServer;
    MessengerDatabase db;
    QMap<QTcpSocket*, int> currentClients;
    QTcpSocket* getSocketById(int id);
    enum TypeOfInputBlock {
        SIGNIN,
        SIGNINRESPONSE,
        FRIEND,
        MESSEGE,
        SEARCH,
        REQUEST,
        REQUSTRESPONSE
    };
    quint16 blockSize;

    void syncInfo(QTcpSocket* socket, int id);
    void proccessSignIn(QTcpSocket* socket, SignIn& signIn);
    void proccessMessege(int idFrom, int idTo, QString text);
    void proccessSearch(QTcpSocket* socket, QString searchLogin);
    void proccessRequest(int idFrom, int idTo);
    void proccessRequestResponse(int idFrom, int idTo, bool isAccept);


    void send(QTcpSocket* socket, SignInResponse signInResponse);
    void send(QTcpSocket* socket, TypeOfInputBlock type, QString login);
    void send(QTcpSocket* socket, QString dialogName, bool isYour, QString text);
public slots:
    void slotNewConnection();
    void slotReadClient();

};

#endif // MESSENGERSERVER_H
