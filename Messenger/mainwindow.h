#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "loginwidget.h"
#include "messegeswidget.h"

class ClientServer;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setServer(ClientServer* server);
    ClientServer* getServer() const;

private:
    LoginWidget* loginWidget;
    MessegesWidget* messegesWidget;

    ClientServer* server;
public slots:
    void showErrorMessege(QString errorMessege);
    void addNewFriend(QString login);
};
#endif // MAINWINDOW_H
