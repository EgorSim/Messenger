#ifndef REQUESTWIDGET_H
#define REQUESTWIDGET_H

#include "clientserver.h"

#include <QtWidgets>
#include <QFrame>

class RequestWidget : public QFrame
{
    Q_OBJECT
public:
    RequestWidget(QString login);

    void connect(ClientServer* server);
private:
    QHBoxLayout* hboxLayout;

    QLabel* lblLogin;
    QPushButton* btnAccept;
    QPushButton* btnReject;

signals:
    void requestResponse(QString login, bool isAccept);
};

#endif // REQUESTWIDGET_H
