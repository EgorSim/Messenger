#include <QCoreApplication>

#include "messengerserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MessengerServer server{1111};

    return a.exec();
}
