
#include "mainwindow.h"
#include "clientserver.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {
    loginWidget = new LoginWidget{this};
    messegesWidget = new MessegesWidget{this};
    messegesWidget->hide();
    setFixedSize(800, 500);
    setStyleSheet("QWidget{background-color: rgb(80, 144, 161);}");
    loginWidget->move(150, 75);
}

MainWindow::~MainWindow() {
}

void MainWindow::showErrorMessege(QString errorMessege) {
    loginWidget->showErrorMessege(errorMessege);
    if (errorMessege == "Loading...") {
        loginWidget->hide();
        messegesWidget->show();
    }
}

void MainWindow::addNewFriend(QString login) {
    messegesWidget->addFriend(login);
}

void MainWindow::setServer(ClientServer *server) {
    this->server = server;
    loginWidget->server = server;
    loginWidget->connect();
    messegesWidget->connect();
    QObject::connect(server, SIGNAL(messegeRecieved(QString, bool, QString)), messegesWidget, SLOT(addMessege(QString, bool, QString)));
}

ClientServer* MainWindow::getServer() const {
    return server;
}
