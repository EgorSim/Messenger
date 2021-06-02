#ifndef SIGNINRESPONSE_H
#define SIGNINRESPONSE_H

#include <QDataStream>

class SignInResponse {
    bool isLogin;
    bool isCorrect;

public:
    SignInResponse() {}
    SignInResponse(bool isLogin, bool isCorrect) : isLogin(isLogin), isCorrect(isCorrect) {}
    bool getIsLogin() const {return isLogin;}
    bool getIsCorrect() const {return isCorrect;}

    friend QDataStream& operator>>(QDataStream& stream, SignInResponse& obj) {
        stream >> obj.isLogin;
        stream >> obj.isCorrect;
        return stream;
    }
    friend QDataStream& operator<<(QDataStream& stream, SignInResponse& obj) {
        stream << obj.isLogin;
        stream << obj.isCorrect;
        return stream;
    }
};

#endif // SIGNINRESPONSE_H
