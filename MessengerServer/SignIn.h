#ifndef SIGNIN_H
#define SIGNIN_H

#include <QString>
#include <QDataStream>

class SignIn {
    bool isLogin;
    QString login;
    QString password;
public:
    SignIn() {}
    SignIn(bool isLogin, QString login, QString password) : isLogin(isLogin), login(login), password(password) {}

    friend QDataStream& operator>>(QDataStream& stream, SignIn& obj) {
        stream >> obj.isLogin;
        stream >> obj.login;
        stream >> obj.password;
        return stream;
    }
    friend QDataStream& operator<<(QDataStream& stream, SignIn& obj) {
        stream << obj.isLogin;
        stream << obj.login;
        stream << obj.password;
        return stream;
    }

    QString getLogin() const {return login;}
    QString getPassword() const {return password;}
    bool getIsLogin() const {return isLogin;}
};

#endif // SIGNIN_H
