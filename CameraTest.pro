QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algorithm.cpp \
    grabthread.cpp \
    main.cpp \
    mvcamera.cpp \
    widget.cpp

HEADERS += \
    algorithm.h \
    grabthread.h \
    mvcamera.h \
    widget.h

FORMS += \
    widget.ui



#LIBS += -L$$PWD/../Libraries/win64 -lMvCameraControl
#INCLUDEPATH += $$PWD/../Includes/

INCLUDEPATH += D:/MVS/MVS/Development/Includes
LIBS += D:/MVS/MVS/Development/Libraries/win64/MvCameraControl.lib

#INCLUDEPATH += $$shell_path("$$(OPENCV_ROOT)/build/include")
#LIBS += D:/OpenCV/opencv/build/x64/vc15/lib/opencv_world451.lib
##LIBS += -L$$shell_path("$$(OPENCV_ROOT)/build/x64/vc15/lib") -lopencv_world451
#QMAKE_POST_LINK += D:/project/OpenCV/bin/vc15/bin/opencv_world341.dll \
#                   D:/project/OpenCV/bin/vc15/bin/opencv_ffmpeg341_64.dll\
 #                  D:/project/OpenCV/bin/vc15/bin/opencv_world341d.dll


#INCLUDEPATH += D:/OpenCV/opencv/Qt_Build/install/include
#LIBS += D:/OpenCV/opencv/Qt_Build/lib/libopencv_*.a

#INCLUDEPATH += D:/project/OpenCV/include
#LIBS += D:/project/OpenCV/bin/vc14/lib/opencv_world341d.lib

INCLUDEPATH += D:/OpenCV/opencv/build/include
LIBS += D:/OpenCV/opencv/build/x64/vc15/lib/opencv_world451d.lib

