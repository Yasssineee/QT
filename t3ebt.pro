QT += core gui sql charts network
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AlimentaireApp
TEMPLATE = app

CONFIG += c++17

SOURCES += main.cpp \
    alimentaire.cpp \
    mainwindow.cpp \
    connection.cpp

HEADERS += alimentaire.h \
    mainwindow.h \
    connection.h

FORMS += mainwindow.ui

# Add QR Code API Key
DEFINES += GOQR_API_KEY=""

# Translations
TRANSLATIONS = translations/alimentaire_fr.ts \
                translations/alimentaire_ar.ts

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
