#ifndef MESSENGERDATABASE_H
#define MESSENGERDATABASE_H

#include "SignIn.h"

#include <QObject>
#include <QSqlDatabase>

class MessengerDatabase : public QObject
{
    Q_OBJECT
public:
    explicit MessengerDatabase(QObject *parent = nullptr);

    int getIdByLogin(QString login);
    QString getLoginById(int id);

    QVector<QString> getFriendsById(int id);
    QVector<std::tuple<QString, bool, QString>> getMessegesById(int id);
    QVector<int> getRequestsById(int id);

    bool checkSignIn(SignIn& signIn);
    bool isContainRequest(int idFrom, int idTo);
    bool isContainFriends(int friend1, int friend2);
    void addMessege(int idFrom, int idTo, QString text);
    void addRequest(int idForm, int idTo);
    void removeRequest(int idFrom, int idTo);
    void addFriends(int friend1, int friend2);
private:
    QSqlDatabase db;
};

#endif // MESSENGERDATABASE_H
