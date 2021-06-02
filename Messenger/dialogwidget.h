#ifndef DIALOGWIDGET_H
#define DIALOGWIDGET_H

#include <QtWidgets>
#include <QFrame>

class DialogWidget : public QFrame
{
    Q_OBJECT
public:
    DialogWidget(QString login);

    void addMessege(bool isMy, QString messege);
private:
    QVBoxLayout* vboxLayout;

    QScrollArea* scrollAreaMesseges;
    QWidget* wgtMesseges;

    QVBoxLayout* innerVboxLayout;

    QWidget* wgtSend;
    QHBoxLayout* hboxLayout;
    QLineEdit* lineMessege;
    QPushButton* btnSend;

    QString login;

signals:
    void sendMessege(QString to, QString text);
private slots:
    void scrollToBot();
    void showMessege();

};

#endif // DIALOGWIDGET_H
