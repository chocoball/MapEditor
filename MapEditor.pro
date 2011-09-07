#-------------------------------------------------
#
# Project created by QtCreator 2011-08-11T23:49:36
#
#-------------------------------------------------

QT			+=	core gui xml

TARGET		=	MapEditor
TEMPLATE	=	app


SOURCES		+=	src/main.cpp				\
				src/mainwindow.cpp			\
				src/csetting.cpp			\
				src/ceditdata.cpp			\
				src/cimagelabel.cpp			\
				src/cmaplabel.cpp			\
				src/cgridlabel.cpp			\
				src/csavefile.cpp			\
				src/cscrollareaimage.cpp	\
				src/clistmodelmap.cpp		\
				src/clistmodeltreasure.cpp	\
				src/clistmodelpoint.cpp		\
				src/ccomboboxdelegate.cpp	\
				src/command.cpp

HEADERS		+=	src/mainwindow.h			\
				src/csetting.h				\
				src/ceditdata.h				\
				src/include.h				\
				src/cimagelabel.h			\
				src/cmaplabel.h				\
				src/cgridlabel.h			\
				src/csavefile.h				\
				src/cscrollareaimage.h		\
				src/clistmodelmap.h			\
				src/clistmodeltreasure.h	\
				src/clistmodelpoint.h		\
				src/ccomboboxdelegate.h		\
				src/command.h

FORMS		+=	mainwindow.ui


