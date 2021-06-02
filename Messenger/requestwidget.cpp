#include "requestwidget.h"

RequestWidget::RequestWidget(QString login) {
    // Memory allocate
    hboxLayout = new QHBoxLayout{};

    lblLogin = new QLabel{login};
    lblLogin->setStyleSheet("QWidget{border: none; padding-left: 5px; font-size: 18px;}");
    btnAccept = new QPushButton{"Accept"};
    btnAccept->setStyleSheet("QPushButton{background-color: rgb(93, 140, 152); padding: 6px 4px; font-size: 15px; border: 1px solid rgb(40, 75, 85); border-radius: 6px;}"
                             "QPushButton:hover{background-color: rgb(132, 199, 218); padding: 6px 4px; font-size: 15px; border: 1px solid rgb(40, 75, 85); border-radius: 6px;}");
    QObject::connect(btnAccept, &QPushButton::clicked, [&]() {
       emit requestResponse(lblLogin->text(), true);
       QObject::deleteLater();
    });
    btnReject = new QPushButton{"Reject"};
    btnReject->setStyleSheet("QPushButton{background-color: rgb(93, 140, 152); padding: 6px 4px; font-size: 15px; border: 1px solid rgb(40, 75, 85); border-radius: 6px;}"
                             "QPushButton:hover{background-color: rgb(132, 199, 218); padding: 6px 4px; font-size: 15px; border: 1px solid rgb(40, 75, 85); border-radius: 6px;}");
    QObject::connect(btnReject, &QPushButton::clicked, [&]() {
       emit requestResponse(lblLogin->text(), false);
       QObject::deleteLater();
    });

    // Layout set up
    hboxLayout->addWidget(lblLogin);
    hboxLayout->addWidget(btnAccept);
    hboxLayout->addWidget(btnReject);

    setLayout(hboxLayout);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setFixedHeight(50);
    setFixedWidth(220);
    setStyleSheet("QWidget{background-color: rgb(75, 145, 162); border: 1px solid rgb(40, 75, 85); border-radius: 6px;}");
}

void RequestWidget::connect(ClientServer *server) {
    QObject::connect(this, SIGNAL(requestResponse(QString, bool)), server, SLOT(slotRequestResponse(QString, bool)));
}
