#include "clientserver.h"
#include "SignInResponse.h"

#include <QHostAddress>
#include <QByteArray>

template<typename Enum,
         typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
QDataStream& operator>>(QDataStream& stream, Enum& e) {
    quint16 v;
    stream >> v;
    e = static_cast<Enum>(v);
    return stream;
}

template<typename Enum,
         typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
QDataStream& operator<<(QDataStream& stream, Enum& e) {
    qint16 v;
    stream << v;
    e = static_cast<Enum>(v);
    return stream;
}

ClientServer::ClientServer(MainWindow* guiMainWindow, QObject *parent) :
    QObject(parent),
    guiMainWindow(guiMainWindow),
    blockSize(0) {
    QObject::connect(this, SIGNAL(serverError(QString)), guiMainWindow, SLOT(showErrorMessege(QString)));
    QObject::connect(this, SIGNAL(newFriend(QString)), guiMainWindow, SLOT(addNewFriend(QString)));


    tcpSocket = new QTcpSocket{this};
    tcpSocket->connectToHost(QHostAddress("80.249.150.137"), 1111);

    QObject::connect(tcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
}

ClientServer::~ClientServer() { tcpSocket->close(); }

void ClientServer::slotReadyRead() {
    QDataStream in(tcpSocket);
    if (!blockSize) {
        if (tcpSocket->bytesAvailable() < sizeof(blockSize)) return;
        in >> blockSize;
    }
    if (tcpSocket->bytesAvailable() < blockSize) return;
    else blockSize = 0;

    quint16 typeOfBlock;
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
}

void ClientServer::slotAuthorizationAttempt(SignIn info) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << (quint16)TypeOfInputBlock::SIGNIN << info;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    tcpSocket->write(arrBlock);
}

void ClientServer::slotSendMessege(QString to, QString text) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << (quint16)TypeOfInputBlock::MESSEGE << to << false << text;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    tcpSocket->write(arrBlock);
}

void ClientServer::slotSearch(QString searchLogin) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << (quint16)TypeOfInputBlock::SEARCH << searchLogin;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    tcpSocket->write(arrBlock);
}

void ClientServer::slotRequest(QString requestLogin) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << (quint16)TypeOfInputBlock::REQUEST << requestLogin;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    tcpSocket->write(arrBlock);
}

void ClientServer::slotRequestResponse(QString login, bool isAccept) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16{0} << (quint16)TypeOfInputBlock::REQUSTRESPONSE << login << isAccept;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    tcpSocket->write(arrBlock);
}
