#-------------------------------------------------
#
# Project created by QtCreator 2017-11-22T09:10:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Longdistracker
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
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
    screen.cpp \
    camera.cpp \
    webcamera.cpp \
    threadcapture.cpp \
    guidecamera.cpp \
    imagingcamera.cpp \
    selectserialportdialog.cpp \
    exception.cpp

HEADERS += \
        mainwindow.h \
    screen.h \
    camera.h \
    webcamera.h \
    threadcapture.h \
    guidecamera.h \
    astronomy.h \
    imagingcamera.h \
    selectserialportdialog.h \
    exception.h

FORMS += \
        mainwindow.ui \
    selectserialportdialog.ui

INCLUDEPATH+= $$(OPENCV_INC_PATH) \
              $$quote($$(PYLON_INC_PATH))\
              $$(SISODIR5)\include\
              $$(SISODIR5)\SDKExamples\include\
              $$(VC14_INC_PATH)\
              $$(WINDOWSKITS_7_INC_PATH)\
              $$(WINDOWSKITS_10_INC_PATH)



LIBS += -L$$(OPENCV_LIB_PATH)  \
        -L$$quote($$(PYLON_LIB_PATH))\
        -L$$(SISODIR5)\lib\visualc\
        -L$$(WINDOWSKITS_7_LIB_PATH)\
        -L$$(WINDOWSKITS_10_LIB_PATH)\
        -L$$(VC14_LIB_PATH)\
        -lopencv_world330d  \
        -lGCBase_MD_VC120_v3_0_Basler_pylon_v5_0\
        -lclsersis\
        -lfglib5
        
