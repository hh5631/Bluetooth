QT       += core gui
QT       += network
QT       += concurrent
QT       += bluetooth
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

linux {
    # 确保使用本地Qt
    CONFIG -= app_bundle
    
    # 添加蓝牙权限
    QMAKE_LFLAGS += -Wl,--no-as-needed
}

include(MyKeyboard/MyKeyboard.pri)

INCLUDEPATH += \
               ./ \
               SpdlogManage/Spdlog/include


SOURCES += \
    Loading.cpp \
    SpdlogManage/SpdlogManage.cpp \
    main.cpp \
    slidebutton.cpp \
    widget.cpp \
    CustomMessageBox/CustomMessageBox.cpp\
    PasswordInputDlg/PasswordInputDlg.cpp \ 
    DatabaseManage/DatabaseManage.cpp \
    BluetoothWorker/BluetoothWorker.cpp


HEADERS += \
    Loading.h \
    SpdlogManage/SpdlogManage.h \
    slidebutton.h \
    widget.h \
    Myinclude.h \
    CustomMessageBox/CustomMessageBox.h\
    PasswordInputDlg/PasswordInputDlg.h \
    DatabaseManage/DatabaseManage.h \
    BluetoothWorker/BluetoothWorker.h

FORMS += \
    widget.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/root/Project/WifiDemo
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Images.qrc

DISTFILES += \
    SpdlogManage/Spdlog/include/spdlog/fmt/bundled/fmt.license.rst
