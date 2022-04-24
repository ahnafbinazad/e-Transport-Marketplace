QT       += core gui sql widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    customerprofile.cpp \
    driverprofile.cpp \
    feedback.cpp \
    invoiceviewer.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    customerprofile.h \
    driverprofile.h \
    feedback.h \
    invoiceviewer.h \
    mainwindow.h

FORMS += \
    customerprofile.ui \
    driverprofile.ui \
    feedback.ui \
    invoiceviewer.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
