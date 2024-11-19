QT       += core gui axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core/main.cpp \
    core/packet/sendProxy.cpp \
    core/packet/wsaSendProxy.cpp \
    core/roco_window/rocoWindow.cpp

HEADERS += \
    core/hook_configure/hookConfigure.h \
    core/packet/sendProxy.h \
    core/packet/wsaSendProxy.h \
    core/roco_window/rocoWindow.h \
    core/atomic_queue/*.h \
    core/utils/bytes.h

FORMS += \
    ui/rocoWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -lWs2_32
win32: LIBS += -L$$PWD/libs/minhook/bin/ -lMinHook.x64

INCLUDEPATH += $$PWD/libs/minhook/include
DEPENDPATH += $$PWD/libs/minhook/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/minhook/bin/MinHook.x64.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/minhook/bin/libMinHook.x64.a

DISTFILES += \
    .gitignore
