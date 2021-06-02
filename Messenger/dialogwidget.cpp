#include "dialogwidget.h"
#include <QDebug>

DialogWidget::DialogWidget(QString login) : login(login) {
    // Memory allocate
    vboxLayout = new QVBoxLayout{};

    scrollAreaMesseges = new QScrollArea{};
    scrollAreaMesseges->setStyleSheet("QWidget{border: none;}"
                                      "QScrollBar{width: 8px; border: 1px solid rgb(40, 75, 85); border-radius: 2px; background: rgb(67, 160, 185);}"
                                      "QScrollBar::add-line:vertical{width: 0px; height: 0px;}"
                                      "QScrollBar::sub-line:vertical{width: 0px; height: 0px;}"
                                      "QScrollBar::handle:vertical{background: rgb(67, 160, 185);}"
                                      "QScrollBar:vertical{background-color: rgb(67, 160, 185);}");
    scrollAreaMesseges->setWidgetResizable(true);
    wgtMesseges = new QWidget{};
    innerVboxLayout = new QVBoxLayout{};
    innerVboxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    wgtSend = new QWidget{};
    wgtSend->setStyleSheet("QWidget{border: none; border-top: 1px solid rgb(40, 75, 85); border-radius: 0px;}");
    hboxLayout = new QHBoxLayout{};
    lineMessege = new QLineEdit{};
    lineMessege->setStyleSheet("QLineEdit{background-color: rgb(99, 183, 206); font-size: 15px; padding: 4px; border: 1px solid black; border-radius: 7px;}");
    QObject::connect(lineMessege, SIGNAL(returnPressed()), SLOT(showMessege()));
    btnSend = new QPushButton{"Send"};
    btnSend->setStyleSheet("QPushButton{background-color: rgb(93, 140, 152); padding: 6px 4px; font-size: 15px; border: 1px solid rgb(40, 75, 85); border-radius: 6px;}"
                           "QPushButton:hover{background-color: rgb(132, 199, 218); padding: 6px 4px; font-size: 15px; border: 1px solid rgb(40, 75, 85); border-radius: 6px;}");
    QObject::connect(btnSend, SIGNAL(clicked()), SLOT(showMessege()));

    // Layout set up
    scrollAreaMesseges->setWidget(wgtMesseges);
    wgtMesseges->setLayout(innerVboxLayout);

    hboxLayout->addWidget(lineMessege);
    hboxLayout->addWidget(btnSend);
    wgtSend->setLayout(hboxLayout);

    vboxLayout->addWidget(scrollAreaMesseges);
    vboxLayout->addWidget(wgtSend);

    setLayout(vboxLayout);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setStyleSheet("QWidget{border: 1px solid rgb(37, 109, 128); border-radius: 6px;}");
    QObject::connect(scrollAreaMesseges->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), SLOT(scrollToBot()));
}

void DialogWidget::addMessege(bool isMy, QString messege) {
    QHBoxLayout* tempLayout = new QHBoxLayout{};
    if (isMy) tempLayout->setAlignment(Qt::AlignRight);
    else tempLayout->setAlignment(Qt::AlignLeft);

    QLabel* tempLabel = new QLabel{messege};
    tempLabel->setStyleSheet(QString{"background-color: rgb(93, 140, 152); border: 1px solid black; border-radius: 10px; font-size: 17px; padding: 6px;"});
    tempLabel->setWordWrap(true);
    tempLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    tempLabel->setMaximumWidth(300);
    tempLabel->setAlignment(Qt::AlignCenter);

    QWidget* tempWidget = new QWidget{};
    tempWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    tempWidget->setMinimumWidth(440);
    tempWidget->setLayout(tempLayout);

    if (isMy) tempLayout->addWidget(tempLabel, Qt::AlignRight);
    else tempLayout->addWidget(tempLabel, Qt::AlignLeft);

    innerVboxLayout->addWidget(tempWidget);
}

void DialogWidget::scrollToBot() {
    scrollAreaMesseges->verticalScrollBar()->setValue(scrollAreaMesseges->verticalScrollBar()->maximum());
}

void DialogWidget::showMessege() {
    if (lineMessege->text().isEmpty()) return;

    emit sendMessege(login, lineMessege->text());

    addMessege(true, lineMessege->text());
    lineMessege->setText(QString{});
}

