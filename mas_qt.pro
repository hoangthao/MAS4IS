######################################################################
# Automatically generated by qmake (2.01a) Thu Oct 24 18:20:00 2013
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v5.0/include"
LIBS += -L"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v5.0/lib/Win32" -lOpenCL

#INCLUDEPATH = 
#LIBS += 
				

QT           += opengl
CONFIG       += console

# Input

HEADERS       = ui/mainwindow.h \
				ui/opendialog.h \
				ui/glwidget.h \
				core/misc.h \
				core/Config.h \
				core/Environment.h \
				core/Segmentation.h \
				classic/ClassicSegmentation.h \
				classic/Otsu.h \
				classic/Region.h \
				classic/Sobel.h \ 
				ui/SpiderDialog.h \
				ui/AntDialog.h \
				core/Info.h \
				magent/Agent.h \
				magent/MAS.h \
				magent/AgentSegmentation.h \
				magent/SpiderInfo.h \				
				magent/Spider.h \
				magent/SpiderSegmentation.h \
				magent/AntInfo.h \				
				magent/Ant.h \
				magent/AntSegmentation.h \
				core/misc_ext.h \
				magent/SpiderSegmentationGPU.h \
				magent/AntSegmentationGPU.h
				
SOURCES       = Main.cpp \
                ui/mainwindow.cpp \
                ui/opendialog.cpp \
                ui/glwidget.cpp \
                core/misc.cpp \
                core/Config.cpp \
                core/Environment.cpp \
				classic/Otsu.cpp \
				classic/Region.cpp \
				classic/Sobel.cpp \
				ui/SpiderDialog.cpp \
				ui/AntDialog.cpp \
				magent/Spider.cpp \
				magent/SpiderSegmentation.cpp \
				magent/Ant.cpp \
				magent/AntSegmentation.cpp \
				magent/SpiderSegmentationGPU.cpp \
				magent/AntSegmentationGPU.cpp

RESOURCES     = mas_qt.qrc