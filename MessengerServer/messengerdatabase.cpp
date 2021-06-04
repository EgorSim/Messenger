#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>

#include "messengerdatabase.h"

MessengerDatabase::MessengerDatabase(QObject *parent) : QObject(parent) {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("messenger");
    db.setUserName("root");
    db.setPassword("123456");
    db.open();
}

int MessengerDatabase::getIdByLogin(QString login) {
    QSqlQuery query = QSqlQuery{db};
    query.exec(QStringLiteral("select id from users where login=%1").arg(login));
    query.next();
    return query.record().value(0).toInt();
}

QString MessengerDatabase::getLoginById(int id) {
    QSqlQuery query = QSqlQuery{db};
    query.exec(QStringLiteral("select login from users where id=%1;").arg(QString::number(id)));
    query.next();
    return query.record().value(0).toString();
}

QVector<QString> MessengerDatabase::getFriendsById(int id) {
    QVector<QString> result;

    QSqlQuery query = QSqlQuery{db};
    query.exec(QStringLiteral("select friend1_id, friend2_id from friends where friend1_id=%1 or friend2_id=%2;")
               .arg(QString::number(id), QString::number(id)));

    while (query.next()) {
        int a = query.value("friend1_id").toInt();
        int b = query.value("friend2_id").toInt();
        if (a != id) result.push_back(getLoginById(a));
        else result.push_back(getLoginById(b));
    }

    return result;
}

QVector<std::tuple<QString, bool, QString>> MessengerDatabase::getMessegesById(int id) {
    QVector<std::tuple<QString, bool, QString>> result;
    QSqlQuery query = QSqlQuery{db};
    query.exec(QStringLiteral("select from_id, to_id, messege_text from messeges where from_id=%1 or to_id=%2;")
               .arg(QString::number(id), QString::number(id)));

    while (query.next()) {
        int a = query.value("from_id").toInt();
        int b = query.value("to_id").toInt();

        if (a == id) result.push_back(std::tuple<QString, bool, QString>{getLoginById(b), true, query.value("messege_text").toString()});
        else result.push_back(std::tuple<QString, bool, QString>{getLoginById(a), false, query.value("messege_text").toString()});
    }

    return result;
}

QVector<int> MessengerDatabase::getRequestsById(int id) {
    QVector<int> result;

    QSqlQuery query = QSqlQuery{db};
    query.exec(QStringLiteral("select from_id from friendship_requests where to_id=%1;").arg(QString::number(id)));
    while (query.next()) {
        result.push_back(query.value("from_id").toInt());
    }
    return result;
}

bool MessengerDatabase::checkSignIn(SignIn &signIn) {
    QSqlQuery query = QSqlQuery{db};
    if (signIn.getIsLogin()) {
        query.exec(QStringLiteral("select login, password from users where login=%1 and password=%2;")
                   .arg(signIn.getLogin(), signIn.getPassword()));
        if (query.numRowsAffected() == 0) {
            return false;
        }
    }
    else {
        query.exec(QStringLiteral("select * from users where login=%1").arg(signIn.getLogin()));
        if (query.numRowsAffected() != 0) {
            return false;
        }
        query.exec(QStringLiteral("insert into users (login, password) values ('%1', '%2')")
                   .arg(signIn.getLogin(), signIn.getPassword()));
    }
    return true;
}

void MessengerDatabase::addMessege(int idFrom, int idTo, QString text) {
    QSqlQuery query = QSqlQuery{db};

    query.exec(QStringLiteral("insert into messeges (from_id, to_id, messege_text) values('%1', '%2', '%3');")
               .arg(QString::number(idFrom), QString::number(idTo), text));
}

void MessengerDatabase::addRequest(int idFrom, int idTo) {
    QSqlQuery query = QSqlQuery{db};
    if (!isContainRequest(idFrom, idTo) && !isContainFriends(idFrom, idTo))
        query.exec(QStringLiteral("insert into friendship_requests (from_id, to_id) values(%1, %2);")
                   .arg(QString::number(idFrom), QString::number(idTo)));
}

void MessengerDatabase::removeRequest(int idFrom, int idTo) {
    QSqlQuery query = QSqlQuery{db};
    if (!query.exec(QStringLiteral("delete from friendship_requests where from_id=%1 and to_id=%2")
                    .arg(QString::number(idTo), QString::number(idFrom))))
        qDebug() << query.lastError().text();
}

void MessengerDatabase::addFriends(int friend1, int friend2) {
    QSqlQuery query = QSqlQuery{db};
    if (!isContainFriends(friend1, friend2))
        query.exec(QStringLiteral("insert into friends (friend1_id, friend2_id) values(%1, %2);")
                   .arg(QString::number(friend1), QString::number(friend2)));
}

bool MessengerDatabase::isContainRequest(int idFrom, int idTo) {
    QSqlQuery query = QSqlQuery{db};
    query.exec(QStringLiteral("select * from friendship_requests where from_id=%1 and to_id=%2;")
               .arg(QString::number(idFrom), QString::number(idTo)));
    return query.numRowsAffected();
}

bool MessengerDatabase::isContainFriends(int friend1, int friend2) {
    QSqlQuery query = QSqlQuery{db};
    query.exec(QStringLiteral("select * from friends where (friend1_id=%1 and friend2_id=%2) or (friend1_id=%3 and friend2_id=%4)")
               .arg(QString::number(friend1), QString::number(friend2), QString::number(friend2), QString::number(friend1)));
    return query.numRowsAffected();
}
