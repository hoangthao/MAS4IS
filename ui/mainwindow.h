
#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QToolBar>
#include <QtGui/QCloseEvent>
#include <QtGui/QGraphicsView>
#include <QtGui/QSlider>
#include <QtGui/QListWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QGridLayout>
#include <QtGui/QProgressDialog>


#include "../core/misc.h"
#include "../core/Environment.h"
#include "../core/Segmentation.h"
#include "../core/Config.h"
#include "glwidget.h"

class ItemWrap{
public:
	Environment *env;
	QString name;
	QString infos;
	//int type;
	Segmentation *seg;
	ItemWrap(){
		env =0;
//		type=0;
		seg = 0;
	}
	~ItemWrap(){
		free(env);
	}
};

class MainWindow: public QMainWindow {
Q_OBJECT

public:
	MainWindow();
	void loadFile(int x, int y, int z, const QString &fileName);
	bool saveFile(const QString &fileName);

	QList<ItemWrap*> list;
//	int crrItem;
	Config conf;

	bool loadGroundTruth;
	void statistics(int x, int y, int z, const QString &fileName);

	void statisticGeneral(Environment *result, Environment *gt, const char* fn);

	void spiderImpl();
	void antImpl();

 private slots:
     void open();
     void openGT();
     void openSpiderDialog();
     void openAntDialog();
     bool save();
     void about();
     void close();
     void setValueSlideXY(int value);
     void setValueSlideXZ(int value);
     void setValueSlideYZ(int value);
     void selectImage();
     void otsuImpl();
     void regionImpl();
     void sobelImpl();

 private:
     void createActions();
     void createMenus();
     void createToolBars();
     void createStatusBar();
     void createMainPanel();
     void createDockPanel();
     void viewSlideXY(int i);
     void viewSlideXZ(int i);
     void viewSlideYZ(int i);
     void readSettings();
     void writeSettings();


     QMenu *fileMenu;
     QMenu *helpMenu;
     QToolBar *fileToolBar;
     QAction *openAct;
     QAction *saveAct;
     QAction *closeAct;
     QAction *exitAct;
     QAction *aboutAct;

     QMenu *algMenu;
     QAction *otsuAct;
     QAction *regionAct;
     QAction *sobelAct;
     QAction *spiderAct;
     QAction *antAct;

     QAction *statisticAct;


     QListWidget*    imageList;
     QTextBrowser*    infos;

     QWidget *widget;
	QGridLayout *gridLayout;
	QGraphicsView *view1;
	QGraphicsView *view3;
	QSlider *sliderXY;
	QSlider *sliderYZ;
	QGraphicsView *view2;
	QSlider *sliderXZ;
	QSlider *slider3D;

	QGraphicsPixmapItem *item1,*item2,*item3;

	GLWidget *vc;


 };


#endif
