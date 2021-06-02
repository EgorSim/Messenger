#include "searchresultwidget.h"

SearchResultWidget::SearchResultWidget(QString login) {
    // Memory allocate
    hboxLayout = new QHBoxLayout{};

    lblLogin = new QLabel{login};
    lblLogin->setStyleSheet("QWidget{border: none; padding-left: 8px; font-size: 18px;}");
    btnMakeRequest = new QPushButton{"Make request"};
    btnMakeRequest->setStyleSheet("QPushButton{background-color: rgb(93, 140, 152); padding: 6px 4px; font-size: 15px; border: 1px solid rgb(40, 75, 85); border-radius: 6px;}"
                                  "QPushButton:hover{background-color: rgb(132, 199, 218); padding: 6px 4px; font-size: 15px; border: 1px solid rgb(40, 75, 85); border-radius: 6px;}");
    QObject::connect(btnMakeRequest, &QPushButton::clicked, [&]() {
       emit request(lblLogin->text());
    });

    // Layout set up
    hboxLayout->addWidget(lblLogin);
    hboxLayout->addWidget(btnMakeRequest);

    setLayout(hboxLayout);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setFixedHeight(50);
    setFixedWidth(220);
    setStyleSheet("QWidget{background-color: rgb(75, 145, 162); border: 1px solid rgb(40, 75, 85); border-radius: 6px;}");
}

void SearchResultWidget::connect(ClientServer *server) {
    QObject::connect(this, SIGNAL(request(QString)), server, SLOT(slotRequest(QString)));
}
