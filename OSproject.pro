QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#MATERIAL-SDK
LIBS += $$PWD/libs/libcomponents.a
INCLUDEPATH += libs/include

INCLUDEPATH += \
    mapper/ \
    mapper/daoimpl/ \
    entity/ \
    service/ \
    utils/ \
    libs/

SOURCES += \
    analysewindow.cpp \
    chartform.cpp \
    daoconfigform.cpp \
    entity/buffer.cpp \
    bufferwidget.cpp \
    entity/config.cpp \
    configform.cpp \
    mapper/dao.cpp \
    main.cpp \
    mainwindow.cpp \
    entity/op_thread.cpp \
    mapper/daoimpl/configdaoimpl.cpp \
    mapper/daoimpl/messagedaoimpl.cpp \
    mapper/daoimpl/resultdaoimpl.cpp \
    progressball.cpp \
    entity/result.cpp \
    entity/message.cpp \
    utils/semaphore.cpp \

HEADERS += \
    analysewindow.h \
    chartform.h \
    daoconfigform.h \
    entity/buffer.h \
    bufferwidget.h \
    entity/config.h \
    configform.h \
    mapper/dao.h \
    mainwindow.h \
    entity/op_thread.h \
    mapper/daoimpl/configdaoimpl.h \
    mapper/daoimpl/messagedaoimpl.h \
    mapper/daoimpl/resultdaoimpl.h \
    progressball.h \
    entity/result.h \
    entity/message.h \
    utils/semaphore.h \

FORMS += \
    analysewindow.ui \
    bufferwidget.ui \
    chartform.ui \
    configform.ui \
    daoconfigform.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_ICONS = assets\icons\logo.ico
