#include "searchline.h"

SearchLine::SearchLine() {
    // Memory allocate
    hboxLayout = new QHBoxLayout{};

    lblLogin = new QLabel{"Login:"};
    lineLogin = new QLineEdit{};
    lblLogin->setStyleSheet("QLabel{border: none; font-size: 18px;}");
    lineLogin->setStyleSheet("QLineEdit{background-color: rgb(99, 183, 206); font-size: 15px; padding: 4px; border: 1px solid black; border-radius: 7px;}");
    lineLogin->setMaxLength(8);
    btnSearch = new QPushButton{"Search"};
    btnSearch->setStyleSheet("QPushButton{background-color: rgb(93, 140, 152); padding: 6px 4px; font-size: 15px; border: 1px solid rgb(40, 75, 85); border-radius: 6px;}"
                            "QPushButton:hover{background-color: rgb(132, 199, 218); padding: 6px 4px; font-size: 15px; border: 1px solid rgb(40, 75, 85); border-radius: 6px;}");
    QObject::connect(btnSearch, &QPushButton::clicked, [&]() {
       if (!lineLogin->text().isEmpty()) emit search(lineLogin->text());
    });

    // Layout set up
    hboxLayout->addWidget(lblLogin);
    hboxLayout->addWidget(lineLogin);
    hboxLayout->addWidget(btnSearch);

    setLayout(hboxLayout);
    setStyleSheet("QWidget{border-bottom: 2px solid rgb(30, 78, 91); border-radius: 0px;}");
}
