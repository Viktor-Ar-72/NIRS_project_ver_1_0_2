QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog_sql_delete.cpp \
    dialog_sql_insert.cpp \
    dialog_sql_update.cpp \
    main.cpp \
    loginwindow.cpp \
    sql_window_main.cpp

HEADERS += \
    dialog_sql_delete.h \
    dialog_sql_insert.h \
    dialog_sql_update.h \
    loginwindow.h \
    sql_window_main.h

FORMS += \
    dialog_sql_delete.ui \
    dialog_sql_insert.ui \
    dialog_sql_update.ui \
    loginwindow.ui \
    sql_window_main.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
