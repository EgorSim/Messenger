#include "writemessegewidget.h"
#include "messegeswidget.h"

WriteMessegeWidget::WriteMessegeWidget(MessegesWidget* messegesWidget, QString login) : messegesWidget(messegesWidget) {
    // Memory allocate
    hboxLayout = new QHBoxLayout{};

    lblLogin = new QLabel{login};
    lblLogin->setStyleSheet("QWidget{border: none; padding-left: 8px; font-size: 18px;}");
    btnWrite = new QPushButton{"Write"};
    btnWrite->setStyleSheet("QPushButton{background-color: rgb(93, 140, 152); padding: 6px 4px; font-size: 15px; border: 1px solid rgb(40, 75, 85); border-radius: 6px;}"
                            "QPushButton:hover{background-color: rgb(132, 199, 218); padding: 6px 4px; font-size: 15px; border: 1px solid rgb(40, 75, 85); border-radius: 6px;}");
    QObject::connect(btnWrite, SIGNAL(clicked()), messegesWidget, SLOT(showDialog()));

    // Layout set up
    hboxLayout->addWidget(lblLogin);
    hboxLayout->addWidget(btnWrite);

    setLayout(hboxLayout);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setFixedHeight(50);
    setFixedWidth(220);
    setObjectName(login);
    setStyleSheet("QWidget{background-color: rgb(75, 145, 162); border: 1px solid rgb(40, 75, 85); border-radius: 6px;}");
}

QString WriteMessegeWidget::getLogin() const {
    return lblLogin->text();
}
