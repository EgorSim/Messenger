#include "messegeswidget.h"
#include "writemessegewidget.h"
#include "requestwidget.h"
#include "searchline.h"
#include "searchresultwidget.h"
#include "dialogwidget.h"
#include "mainwindow.h"
#include "clientserver.h"

MessegesWidget::MessegesWidget(QWidget *parent) : QWidget(parent) {
    // Memory allocate
    hboxLayout = new QHBoxLayout{};
    tabs = new QTabWidget{};

    scrollAreaFriends = new QScrollArea{};
    scrollAreaRequests = new QScrollArea{};
    scrollAreaSearch = new QScrollArea{};

    wgtFriendsArea = new QWidget{};
    wgtRequestsArea = new QWidget{};
    wgtSearchArea = new QWidget{};

    vboxFriendsArea = new QVBoxLayout{};
    vboxRequestsArea = new QVBoxLayout{};
    vboxSearchArea = new QVBoxLayout{};

    // Layout set up
    scrollAreaFriends->setWidget(wgtFriendsArea);
    scrollAreaFriends->setWidgetResizable(true);
    scrollAreaFriends->setStyleSheet("QWidget{border: none;}"
                                      "QScrollBar{width: 8px; border: 1px solid rgb(40, 75, 85); border-radius: 2px; background: rgb(67, 160, 185);}"
                                      "QScrollBar::add-line:vertical{width: 0px; height: 0px;}"
                                      "QScrollBar::sub-line:vertical{width: 0px; height: 0px;}"
                                      "QScrollBar::handle:vertical{background: rgb(67, 160, 185);}"
                                      "QScrollBar:vertical{background-color: rgb(67, 160, 185);}");
    scrollAreaRequests->setWidget(wgtRequestsArea);
    scrollAreaRequests->setWidgetResizable(true);
    scrollAreaRequests->setStyleSheet("QWidget{border: none;}"
                                      "QScrollBar{width: 8px; border: 1px solid rgb(40, 75, 85); border-radius: 2px; background: rgb(67, 160, 185);}"
                                      "QScrollBar::add-line:vertical{width: 0px; height: 0px;}"
                                      "QScrollBar::sub-line:vertical{width: 0px; height: 0px;}"
                                      "QScrollBar::handle:vertical{background: rgb(67, 160, 185);}"
                                      "QScrollBar:vertical{background-color: rgb(67, 160, 185);}");
    scrollAreaSearch->setWidget(wgtSearchArea);
    scrollAreaSearch->setWidgetResizable(true);
    scrollAreaSearch->setStyleSheet("QWidget{border: none;}"
                                    "QScrollBar{width: 8px; border: 1px solid rgb(40, 75, 85); border-radius: 2px; background: rgb(67, 160, 185);}"
                                    "QScrollBar::add-line:vertical{width: 0px; height: 0px;}"
                                    "QScrollBar::sub-line:vertical{width: 0px; height: 0px;}"
                                    "QScrollBar::handle:vertical{background: rgb(67, 160, 185);}"
                                    "QScrollBar:vertical{background-color: rgb(67, 160, 185);}");


    vboxFriendsArea->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    vboxFriendsArea->setSpacing(15);
    vboxRequestsArea->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    vboxRequestsArea->setSpacing(15);
    vboxSearchArea->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    vboxSearchArea->setSpacing(15);

    searchLine = new SearchLine{};
    vboxSearchArea->addWidget(searchLine);

    wgtFriendsArea->setLayout(vboxFriendsArea);
    wgtRequestsArea->setLayout(vboxRequestsArea);
    wgtSearchArea->setLayout(vboxSearchArea);

    tabs->setFixedWidth(270);
    tabs->setStyleSheet("QWidget{background-color: rgb(62, 108, 120); border: 1px solid rgb(37, 109, 128); border-radius: 6px;}"
                        "QTabWidget::pane{border: none;}"
                        "QTabWidget::tab-bar {alignment: center;}"
                        "QTabBar::tab{background-color: rgb(80, 164, 186); padding: 5px 10px; border: 1px solid rgb(37, 109, 128); border-top-left-radius: 6px; border-top-right-radius: 6px;}"
                        "QTabBar::tab:hover, QTabBar::tab:selected{background-color: rgb(70, 136, 153); padding: 5px 10px; border: 1px solid rgb(37, 109, 128); border-top-left-radius: 6px; border-top-right-radius: 6px;}");
    tabs->addTab(scrollAreaFriends, "Friends");
    tabs->addTab(scrollAreaRequests, "Requests");
    tabs->addTab(scrollAreaSearch, "Search");

    hboxLayout->setAlignment(Qt::AlignLeft);
    hboxLayout->addWidget(tabs);


    setLayout(hboxLayout);
    resize(800, 500);
}

void MessegesWidget::connect() {
    QObject::connect(((MainWindow*)parent())->getServer(), SIGNAL(searchResult(QString)), SLOT(addSearchResult(QString)));
    QObject::connect(searchLine, SIGNAL(search(QString)), ((MainWindow*)parent())->getServer(), SLOT(slotSearch(QString)));
    QObject::connect(((MainWindow*)parent())->getServer(), SIGNAL(request(QString)), SLOT(addRequest(QString)));
}

void MessegesWidget::addFriend(QString login) {
    vboxFriendsArea->addWidget(new WriteMessegeWidget{this, login});
    DialogWidget* tempDialog = new DialogWidget{login};
    QObject::connect(tempDialog, SIGNAL(sendMessege(QString, QString)), ((MainWindow*)parent())->getServer(), SLOT(slotSendMessege(QString, QString)));
    hboxLayout->addWidget(tempDialog);
    mapDialogs.insert(login, tempDialog);
    tempDialog->hide();
}

void MessegesWidget::addMessege(QString dialog, bool isMy, QString text) {
    mapDialogs[dialog]->addMessege(isMy, text);
}

void MessegesWidget::showDialog() {
    const QString& stringToShow = ((QPushButton*)sender())->parent()->objectName();
    foreach (const QString& tempString, mapDialogs.keys()) {
        if (!mapDialogs.value(tempString)->isHidden()) mapDialogs.value(tempString)->hide();
        if (tempString == stringToShow) mapDialogs.value(tempString)->show();
    }
}

void MessegesWidget::addRequest(QString login) {
    RequestWidget* requestWdt = new RequestWidget{login};
    requestWdt->connect(((MainWindow*)parent())->getServer());
    vboxRequestsArea->addWidget(requestWdt);
}

void MessegesWidget::addSearchResult(QString login) {
    SearchResultWidget* searchResultWgt = new SearchResultWidget{login};
    searchResultWgt->connect(((MainWindow*)parent())->getServer());
    vboxSearchArea->addWidget(searchResultWgt);
}
