#include "clientserver.h"
#include "SignInResponse.h"

#include <QDebug>
#include <QHostAddress>
#include <QByteArray>

ClientServer::ClientServer(MainWindow* guiMainWindow, QObject *parent) :
    QObject(parent),
    guiMainWindow(guiMainWindow),
    blockSize(0) {
    QObject::connect(this, SIGNAL(serverError(QString)), guiMainWindow, SLOT(showErrorMessege(QString)));
    QObject::connect(this, SIGNAL(newFriend(QString)), guiMainWindow, SLOT(addNewFriend(QString)));


    tcpSocket = new QTcpSocket{this};
    tcpSocket->connectToHost(QHostAddress::LocalHost, 1111);

    QObject::connect(tcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
}

void ClientServer::slotReadyRead() {
    TypeOfInputBlock typeOfBlock;

    QDataStream in(tcpSocket);
    while (true) {
        if (!blockSize) {
            if (tcpSocket->bytesAvailable() < sizeof(blockSize)) break;
            in >> blockSize;
        }
        if (tcpSocket->bytesAvailable() < blockSize) break;

        in >> typeOfBlock;
        switch (typeOfBlock) {
        case TypeOfInputBlock::SIGNINRESPONSE:
        {
            SignInResponse tempSignInResponse;
            in >> tempSignInResponse;
            if (tempSignInResponse.getIsCorrect()) emit serverError("Loading...");
            else if (tempSignInResponse.getIsLogin()) emit serverError("Incorrect login or password");
            else emit serverError("This login already used");
        }
            break;
        case TypeOfInputBlock::FRIEND:
        {
            QString login;
            in >> login;
            emit newFriend(login);
        }
            break;
        case TypeOfInputBlock::MESSEGE:
        {
            bool isMy;
            QString dialog;
            QString text;
            in >> dialog >> isMy >> text;
            emit messegeRecieved(dialog, isMy, text);
        }
            break;
        case TypeOfInputBlock::SEARCH:
        {
            QString searchLogin;
            in >> searchLogin;
            emit searchResult(searchLogin);
        }
            break;
        case TypeOfInputBlock::REQUEST:
        {
            QString requestLogin;
            in >> requestLogin;
            emit request(requestLogin);
        }
            break;
        }

        blockSize = 0;
    }
}

void ClientServer::slotAuthorizationAttempt(SignIn info) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << TypeOfInputBlock::SIGNIN << info;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    tcpSocket->write(arrBlock);
}

void ClientServer::slotSendMessege(QString to, QString text) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << TypeOfInputBlock::MESSEGE << to << false << text;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    tcpSocket->write(arrBlock);
}

void ClientServer::slotSearch(QString searchLogin) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << TypeOfInputBlock::SEARCH << searchLogin;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    tcpSocket->write(arrBlock);
}

void ClientServer::slotRequest(QString requestLogin) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << TypeOfInputBlock::REQUEST << requestLogin;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    tcpSocket->write(arrBlock);
}

void ClientServer::slotRequestResponse(QString login, bool isAccept) {
    qDebug() << "login: " << login << "isAccept" << isAccept;
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << TypeOfInputBlock::REQUSTRESPONSE << login << isAccept;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    tcpSocket->write(arrBlock);
}
