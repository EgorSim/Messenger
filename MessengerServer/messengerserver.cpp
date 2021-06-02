#include <QDebug>
#include <QDataStream>
#include <QTcpSocket>

#include "messengerserver.h"
#include "SignIn.h"

MessengerServer::MessengerServer(int port, QObject *parent) : QObject(parent), blockSize(0) {
    tcpServer = new QTcpServer{this};

    if (!tcpServer->listen(QHostAddress::LocalHost, port)) {
        qDebug() << "Server run error";
    }
    QObject::connect(tcpServer, SIGNAL(newConnection()), SLOT(slotNewConnection()));
}

QTcpSocket* MessengerServer::getSocketById(int id) {
    QTcpSocket* result{nullptr};
    foreach (QTcpSocket* temp, currentClients.keys()) {
        if (currentClients.value(temp) == id) return temp;
    }
    return result;
}

void MessengerServer::slotNewConnection() {
    QTcpSocket* tcpSocket = tcpServer->nextPendingConnection();
    currentClients.insert(tcpSocket, -1);

    QObject::connect(tcpSocket, &QTcpSocket::disconnected, [=](){
        QMap<QTcpSocket*, int>::iterator it = currentClients.find(tcpSocket);
        currentClients.erase(it);
    });
    QObject::connect(tcpSocket, SIGNAL(disconnected()), tcpSocket, SLOT(deleteLater()));
    QObject::connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
}

void MessengerServer::slotReadClient() {
    TypeOfInputBlock typeOfBlock;
    SignIn tempSignIn;

    QTcpSocket* tempTcpSocket = (QTcpSocket*)sender();

    QDataStream in(tempTcpSocket);
    while (true) {
        if (!blockSize) {
            if (tempTcpSocket->bytesAvailable() < sizeof(blockSize)) break;
            in >> blockSize;
        }
        if (tempTcpSocket->bytesAvailable() < blockSize) break;

        in >> typeOfBlock;
        switch (typeOfBlock) {
        case TypeOfInputBlock::SIGNIN:
            in >> tempSignIn;
            proccessSignIn(tempTcpSocket, tempSignIn);
            break;
        case TypeOfInputBlock::MESSEGE:
        {
            QString to;
            QString text;
            bool temp;
            in >> to >> temp >> text;
            proccessMessege(currentClients[tempTcpSocket], db.getIdByLogin(to), text);
        }
            break;
        case TypeOfInputBlock::SEARCH:
        {
            QString searchLogin;
            in >> searchLogin;
            proccessSearch(tempTcpSocket, searchLogin);
        }
            break;
        case TypeOfInputBlock::REQUEST:
        {
            QString requestLogin;
            in >> requestLogin;
            proccessRequest(currentClients[tempTcpSocket], db.getIdByLogin(requestLogin));
        }
            break;
        case TypeOfInputBlock::REQUSTRESPONSE:
        {
            QString requestLogin;
            bool isAccept;
            in >> requestLogin >> isAccept;
            proccessRequestResponse(currentClients[tempTcpSocket], db.getIdByLogin(requestLogin), isAccept);
        }
            break;
        }

        blockSize = 0;
    }
}

void MessengerServer::syncInfo(QTcpSocket *socket, int id) {
    QVector<QString> friends = db.getFriendsById(id);
    foreach (QString login, friends) {
        send(socket, TypeOfInputBlock::FRIEND, login);
    }

    QVector<std::tuple<QString, bool, QString>> messeges = db.getMessegesById(id);
    std::tuple<QString, bool, QString> messege;
    foreach (messege, messeges) {
        send(socket, std::get<0>(messege), std::get<1>(messege), std::get<2>(messege));
    }

    QVector<int> requests = db.getRequestsById(id);
    foreach (int i, requests) {
        send(socket, TypeOfInputBlock::REQUEST, db.getLoginById(i));
    }
}

void MessengerServer::proccessSignIn(QTcpSocket *socket, SignIn &signIn) {
    if (db.checkSignIn(signIn)) {
        currentClients[socket] = db.getIdByLogin(signIn.getLogin());
        syncInfo(socket, db.getIdByLogin(signIn.getLogin()));
        if (signIn.getIsLogin()) send(socket, SignInResponse{true, true});
        else send(socket, SignInResponse{false, true});
    }
    else {
        if (signIn.getIsLogin()) send(socket, SignInResponse{true, false});
        else send(socket, SignInResponse{false, false});
    }
}

void MessengerServer::proccessMessege(int idFrom, int idTo, QString text) {
    QTcpSocket* receiver = getSocketById(idTo);
    if (receiver) send(receiver, db.getLoginById(idFrom), false, text);
    db.addMessege(idFrom, idTo, text);
}

void MessengerServer::proccessSearch(QTcpSocket* socket, QString searchLogin) {
    if (db.getIdByLogin(searchLogin)) send(socket, TypeOfInputBlock::SEARCH, searchLogin);
}

void MessengerServer::proccessRequest(int idFrom, int idTo) {
    if (idFrom == idTo) return;
    QTcpSocket* receiver = getSocketById(idTo);
    if (receiver) if (!db.isContainRequest(idFrom, idTo) && !db.isContainFriends(idFrom, idTo)) send(receiver, TypeOfInputBlock::REQUEST, db.getLoginById(idFrom));
    db.addRequest(idFrom, idTo);
}

void MessengerServer::proccessRequestResponse(int idFrom, int idTo, bool isAccept) {
    db.removeRequest(idFrom, idTo);
    if (isAccept) {
        db.addFriends(idFrom, idTo);
        QTcpSocket* receiver1 = getSocketById(idFrom);
        if (receiver1) send(getSocketById(idFrom), TypeOfInputBlock::FRIEND, db.getLoginById(idTo));
        QTcpSocket* receiver2 = getSocketById(idTo);
        if (receiver2) send(getSocketById(idTo), TypeOfInputBlock::FRIEND, db.getLoginById(idFrom));
    }
}

void MessengerServer::send(QTcpSocket* socket, SignInResponse signInResponse) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << TypeOfInputBlock::SIGNINRESPONSE << signInResponse;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    socket->write(arrBlock);
}

void MessengerServer::send(QTcpSocket *socket, TypeOfInputBlock type, QString login) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << type << login;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    socket->write(arrBlock);
}

void MessengerServer::send(QTcpSocket *socket, QString dialogName, bool isYour, QString text) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << TypeOfInputBlock::MESSEGE << dialogName << isYour << text;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    socket->write(arrBlock);
}
