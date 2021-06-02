#ifndef WRITEMESSEGEWIDGET_H
#define WRITEMESSEGEWIDGET_H

#include <QtWidgets>
#include "messegeswidget.h"

class WriteMessegeWidget : public QFrame
{
    Q_OBJECT
public:
    WriteMessegeWidget(MessegesWidget* messegesWidget, QString login);
    QString getLogin() const;

private:
    QHBoxLayout* hboxLayout;

    QLabel* lblLogin;
    QPushButton* btnWrite;

    MessegesWidget* messegesWidget;
};

#endif // WRITEMESSEGEWIDGET_H
