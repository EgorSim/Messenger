#ifndef SEARCHLINE_H
#define SEARCHLINE_H

#include <QtWidgets>
#include <QFrame>

class SearchLine : public QFrame
{
    Q_OBJECT
public:
    SearchLine();

private:
    QHBoxLayout* hboxLayout;

    QLabel* lblLogin;
    QLineEdit* lineLogin;
    QPushButton* btnSearch;

signals:
    void search(QString login);
};

#endif // SEARCHLINE_H
