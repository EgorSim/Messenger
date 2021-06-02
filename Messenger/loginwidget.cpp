#include "loginwidget.h"
#include "mainwindow.h"
#include "clientserver.h"
#include "SignIn.h"

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent) {
    // Memory allocate
    vboxLayout = new QVBoxLayout{};

    lblLogin = new QLabel{"Login:"};
    lblLogin->setStyleSheet("QLabel{font-size: 20px; font: sans-serif large; font-weight: 400;}");
    lineLogin = new QLineEdit{};
    lineLogin->setStyleSheet("QLineEdit{background-color: rgb(99, 183, 206); font-size: 18px; border: 1px solid black; border-radius: 7px; font: sans-serif large; font-weight: 500;}");
    lineLogin->setMaxLength(8);
    lineLogin->setFixedHeight(30);
    lblLogin->setBuddy(lineLogin);

    lblPassword = new QLabel{"Password:"};
    lblPassword->setStyleSheet("QLabel{font-size: 20px; font: sans-serif large; font-weight: 400;}");
    linePassword = new QLineEdit{};
    linePassword->setEchoMode(QLineEdit::Password);
    linePassword->setStyleSheet("QLineEdit{background-color: rgb(99, 183, 206); font-size: 16px; border: 1px solid black; border-radius: 7px;}");
    linePassword->setMaxLength(8);
    linePassword->setFixedHeight(30);
    lblPassword->setBuddy(lineLogin);

    lblError = new QLabel{};
    lblError->setStyleSheet("QLabel{font-size: 20px; color: rgb(224, 66, 66);}");

    hboxLayout = new QHBoxLayout{};

    btnLogin = new QPushButton{"Sign in"};
    btnLogin->setStyleSheet("QPushButton{background-color: rgb(93, 140, 152); font-size: 20px; border: 1px solid black; border-radius: 10px;}"
                            "QPushButton:hover{background-color: rgb(132, 199, 218); font-size: 20px; border: 1px solid black; border-radius: 10px;}");
    btnRegistration = new QPushButton{"Registration"};
    btnRegistration->setStyleSheet("QPushButton{background-color: rgb(93, 140, 152); font-size: 20px; border: 1px solid black; border-radius: 10px;}"
                                   "QPushButton:hover{background-color: rgb(132, 199, 218); font-size: 20px; border: 1px solid black; border-radius: 10px;}");


    QObject::connect(btnLogin, &QPushButton::clicked, [&](){
        if (!lineLogin->text().isEmpty() && !linePassword->text().isEmpty()) {
            showErrorMessege("");
            emit authorizationAttempt(SignIn{true, lineLogin->text(), linePassword->text()});
        }
        else showErrorMessege("Enter your login or password");
    });
    QObject::connect(btnRegistration, &QPushButton::clicked, [&](){
        if (!lineLogin->text().isEmpty() && !linePassword->text().isEmpty()) {
            showErrorMessege("");
            emit authorizationAttempt(SignIn{false, lineLogin->text(), linePassword->text()});
        }
        else showErrorMessege("Enter your login or password");
    });

    // Layout set up
    vboxLayout->addWidget(lblLogin);
    vboxLayout->addWidget(lineLogin);
    vboxLayout->addWidget(lblPassword);
    vboxLayout->addWidget(linePassword);
    vboxLayout->addWidget(lblError);

    hboxLayout->addWidget(btnLogin);
    hboxLayout->addWidget(btnRegistration);

    vboxLayout->addLayout(hboxLayout);

    setLayout(vboxLayout);
    resize(500, 300);
}

void LoginWidget::showErrorMessege(QString errorMessege) {
    lblError->setText(errorMessege);
}

void LoginWidget::connect() {
    QObject::connect(this, SIGNAL(authorizationAttempt(SignIn)), server, SLOT(slotAuthorizationAttempt(SignIn)));
}
