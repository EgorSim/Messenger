#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QtWidgets>

#include "SignIn.h"

class ClientServer;

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

    void showErrorMessege(QString errorMessege);
    void connect();

    ClientServer* server;

private:
    QVBoxLayout* vboxLayout;
    QLabel* lblLogin;
    QLineEdit* lineLogin;
    QLabel* lblPassword;
    QLineEdit* linePassword;

    QLabel* lblError;

    QHBoxLayout* hboxLayout;
    QPushButton* btnLogin;
    QPushButton* btnRegistration;

signals:
    void authorizationAttempt(SignIn info);
};

#endif // LOGINWIDGET_H
