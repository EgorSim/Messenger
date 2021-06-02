QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 resources_big

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clientserver.cpp \
    dialogwidget.cpp \
    loginwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    messegeswidget.cpp \
    requestwidget.cpp \
    searchline.cpp \
    searchresultwidget.cpp \
    writemessegewidget.cpp

HEADERS += \
    SignIn.h \
    SignInResponse.h \
    clientserver.h \
    dialogwidget.h \
    loginwidget.h \
    mainwindow.h \
    messegeswidget.h \
    requestwidget.h \
    searchline.h \
    searchresultwidget.h \
    writemessegewidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
