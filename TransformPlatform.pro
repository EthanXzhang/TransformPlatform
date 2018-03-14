#-------------------------------------------------
#
# Project created by QtCreator 2018-02-27T08:42:52
#
#-------------------------------------------------

QT       += core gui sql multimedia multimediawidgets avwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TransformPlatform
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    mainwindow.cpp \
    movieinfo.cpp \
    projectiondialog.cpp

HEADERS += \
        mainwindow.h \
    movieinfo.h \
    projectiondialog.h

FORMS += \
        mainwindow.ui \
    projectiondialog.ui

RESOURCES += \
    resource.qrc
INCLUDEPATH += include\libavcodec \
            include\libavdevice \
            include\libavfilter \
            include\libavformat \
            include\libavutil   \
            include\libpostproc \
            include\libswresample   \
            include\libswscale  \
LIBS += lib\avcodec.lib \
        lib\avdevice.lib    \
        lib\avfilter.lib    \
        lib\avformat.lib    \
        lib\avutil.lib  \
        lib\postproc.lib    \
        lib\swresample.lib  \
        lib\swscale.lib \
LIBS +=kernel32.lib \
    user32.lib \
    gdi32.lib \
    winspool.lib \
    comdlg32.lib \
    advapi32.lib \
    shell32.lib \
    ole32.lib \
    oleaut32.lib \
    uuid.lib \
    odbc32.lib \
    odbccp32.lib \
    strmiids.lib \
