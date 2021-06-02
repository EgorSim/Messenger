#ifndef SEARCHRESULTWIDGET_H
#define SEARCHRESULTWIDGET_H

#include "clientserver.h"

#include <QtWidgets>
#include <QFrame>

class SearchResultWidget : public QFrame
{
    Q_OBJECT
public:
    SearchResultWidget(QString login);

    void connect(ClientServer* server);

private:
    QHBoxLayout* hboxLayout;

    QLabel* lblLogin;
    QPushButton* btnMakeRequest;

signals:
    void request(QString login);
};

#endif // SEARCHRESULTWIDGET_H
