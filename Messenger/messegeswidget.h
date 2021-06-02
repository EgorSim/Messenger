#ifndef MESSEGESWIDGET_H
#define MESSEGESWIDGET_H

#include <QtWidgets>
#include "dialogwidget.h"
#include "searchline.h"

class MessegesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessegesWidget(QWidget *parent = nullptr);

    void connect();

private:
    QHBoxLayout* hboxLayout;
    QTabWidget* tabs;

    QScrollArea* scrollAreaFriends;
    QScrollArea* scrollAreaRequests;
    QScrollArea* scrollAreaSearch;

    QWidget* wgtFriendsArea;
    QWidget* wgtRequestsArea;
    QWidget* wgtSearchArea;

    QVBoxLayout* vboxFriendsArea;
    QVBoxLayout* vboxRequestsArea;
    QVBoxLayout* vboxSearchArea;

    SearchLine* searchLine;

    QMap<QString, DialogWidget*> mapDialogs;

public slots:
    void addFriend(QString login);
    void addMessege(QString dialog, bool isMy, QString text);
    void showDialog();
    void addRequest(QString login);
    void addSearchResult(QString login);
};

#endif // MESSEGESWIDGET_H
