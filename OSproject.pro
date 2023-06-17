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
    libs/ \
    form/

SOURCES += \
    chartdatachangedtrend.cpp \
    chartstat.cpp \
    entity/buffer.cpp \
    entity/config.cpp \
    entity/message.cpp \
    entity/op_thread.cpp \
    entity/result.cpp \
    form/analysewindow.cpp \
    form/chartform.cpp \
    form/configform.cpp \
    form/daoconfigform.cpp \
    form/ledlabel.cpp \
    form/loginform.cpp \
    form/mainwindow.cpp \
    form/progressball.cpp \
    main.cpp \
    mapper/dao.cpp \
    mapper/daoimpl/configdaoimpl.cpp \
    mapper/daoimpl/messagedaoimpl.cpp \
    mapper/daoimpl/resultdaoimpl.cpp \
    statgatherer.cpp \
    utils/betterchartview.cpp \
    utils/semaphore.cpp


HEADERS += \
    chartdatachangedtrend.h \
    chartstat.h \
    entity/buffer.h \
    entity/config.h \
    entity/message.h \
    entity/op_thread.h \
    entity/result.h \
    form/analysewindow.h \
    form/chartform.h \
    form/configform.h \
    form/daoconfigform.h \
    form/ledlabel.h \
    form/loginform.h \
    form/mainwindow.h \
    form/progressball.h \
    mapper/dao.h \
    mapper/daoimpl/configdaoimpl.h \
    mapper/daoimpl/messagedaoimpl.h \
    mapper/daoimpl/resultdaoimpl.h \
    statgatherer.h \
    utils/betterchartview.h \
    utils/semaphore.h \
    utils/utils.hpp


FORMS += \
    form/analysewindow.ui \
    form/chartform.ui \
    form/configform.ui \
    form/daoconfigform.ui \
    form/ledlabel.ui \
    form/loginform.ui \
    form/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += res.qrc

RC_ICONS = assets\icons\logo.ico
