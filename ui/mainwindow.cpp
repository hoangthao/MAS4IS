
#include <QtCore/QSettings>
#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>

#include <QtGui/QImage>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QDockWidget>
#include <QtGui/QSlider>
#include <QtGui/QListWidget>
#include <QtGui/QTextBrowser>

#include <QtGui/QDialog>

#include "mainwindow.h"
#include "opendialog.h"
#include "glwidget.h"
#include "SpiderDialog.h"
#include "AntDialog.h"
#include "../classic/Otsu.h"
#include "../classic/Region.h"
#include "../classic/Sobel.h"
#include "../magent/SpiderSegmentation.h"
#include "../magent/AntSegmentation.h"
#include "../magent/SpiderSegmentationGPU.h"
#include "../magent/AntSegmentationGPU.h"

MainWindow::MainWindow()
{
	setWindowTitle("Multi-agent segmentation for 3D biomedical images with GPUs - 2013");
	createMainPanel();
	createDockPanel();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    readSettings();

	conf.readFile("params.cfg");
	loadGroundTruth=false;

}

void MainWindow::createMainPanel(){

	widget = new QWidget(this);
	 gridLayout = new QGridLayout(widget);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	gridLayout->setContentsMargins(0, 0, 0, 0);
	view1 = new QGraphicsView(widget);
	view1->setObjectName(QString::fromUtf8("view1"));

	gridLayout->addWidget(view1, 0, 0, 1, 1);

	view3 = new QGraphicsView(widget);
	view3->setObjectName(QString::fromUtf8("view3"));

	gridLayout->addWidget(view3, 0, 1, 1, 1);

	sliderXY = new QSlider(widget);
	sliderXY->setObjectName(QString::fromUtf8("sliderXY"));
	sliderXY->setOrientation(Qt::Horizontal);

	gridLayout->addWidget(sliderXY, 1, 0, 1, 1);

	sliderYZ = new QSlider(widget);
	sliderYZ->setObjectName(QString::fromUtf8("sliderYZ"));
	sliderYZ->setOrientation(Qt::Horizontal);

	gridLayout->addWidget(sliderYZ, 1, 1, 1, 1);

	view2 = new QGraphicsView(widget);
	view2->setObjectName(QString::fromUtf8("view2"));

	gridLayout->addWidget(view2, 2, 0, 1, 1);

	sliderXZ = new QSlider(widget);
	sliderXZ->setObjectName(QString::fromUtf8("sliderXZ"));
	sliderXZ->setOrientation(Qt::Horizontal);

	gridLayout->addWidget(sliderXZ, 3, 0, 1, 1);

	slider3D = new QSlider(widget);
	slider3D->setObjectName(QString::fromUtf8("slider3D"));
	slider3D->setOrientation(Qt::Horizontal);

	gridLayout->addWidget(slider3D, 3, 1, 1, 1);
	setCentralWidget(widget);

	gridLayout->setContentsMargins(5,5,0,5);

	QGraphicsScene* scene = new QGraphicsScene;
	item1=new QGraphicsPixmapItem();
	scene->addItem(item1);
	view1->setScene(scene);

	QGraphicsScene* scene2 = new QGraphicsScene;
	item2 = new QGraphicsPixmapItem();
	scene2->addItem(item2);
	view2->setScene(scene2);

	QGraphicsScene* scene3 = new QGraphicsScene;
	item3 = new QGraphicsPixmapItem();
	scene3->addItem(item3);
	view3->setScene(scene3);

	vc = new GLWidget(widget);
	gridLayout->addWidget(vc, 2, 1, 1, 1);

}

void MainWindow::createDockPanel(){

	QDockWidget *dock = new QDockWidget("-=Images", this);
	//dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	imageList = new QListWidget(dock);
//	imageList->addItems(QStringList()
//			<< "./data/cpu_slice.rawb"
//			<< "./data/cpu_slice.rawb"
//			<< "./data/cpu_slice.rawb");
	dock->setWidget(imageList);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	dock->setFixedHeight(150);
	dock->setContentsMargins(0,5,5,0);

	QDockWidget *dock2 = new QDockWidget("-=Informations", this);
	infos = new QTextBrowser(dock2);
	dock2->setWidget(infos);
	addDockWidget(Qt::RightDockWidgetArea, dock2);
	dock2->setContentsMargins(0,0,5,0);

	connect(imageList, SIGNAL(itemSelectionChanged()), this, SLOT(selectImage()));

}

void MainWindow::open()
{
	loadGroundTruth=false;
	QWidget* w = new QWidget(this);
	OpenDialog* od = new OpenDialog(w,this);
	od->show();
}

void MainWindow::openGT()
{
	loadGroundTruth=true;
	QWidget* w = new QWidget(this);
	OpenDialog* od = new OpenDialog(w,this);
	od->show();
}

void MainWindow::openSpiderDialog()
{
	QWidget* w = new QWidget(this);
	SpiderDialog* od = new SpiderDialog(w,this);
	od->show();

}

void MainWindow::openAntDialog()
{
	QWidget* w = new QWidget(this);
	AntDialog* od = new AntDialog(w,this);
	od->show();
}

bool MainWindow::save()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	 if (!fileName.isEmpty()){
		 Environment *env = list.at(imageList->currentRow())->env;
		 env->writeRawb(fileName.toStdString().c_str());
	 }
    return true;
}

void MainWindow::close()
{
	if (imageList->currentRow()>-1){
		ItemWrap* item = list.at(imageList->currentRow());
		delete item;
		list.takeAt(imageList->currentRow());
		imageList->takeItem(imageList->currentRow());
	}
	if (imageList->count()==0) {
		saveAct->setDisabled(true);
		closeAct->setDisabled(true);
		otsuAct->setDisabled(true);
		sobelAct->setDisabled(true);
		regionAct->setDisabled(true);
		spiderAct->setDisabled(true);
		antAct->setDisabled(true);
		statisticAct->setDisabled(true);

		sliderXY->setDisabled(true);
		sliderXZ->setDisabled(true);
		sliderYZ->setDisabled(true);
		slider3D->setDisabled(true);
	}
}


void MainWindow::about()
{
   QMessageBox::about(this, tr("About"),
            tr("This application is the internship project of master 1 program at LaBRI - 2013\n"
            		"Author: Hoang Thao NGUYEN\n"
            		"Contact: hoangthao.ng@gmail.com"));
}
void MainWindow::createActions()
{
    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
    saveAct->setDisabled(true);

    closeAct = new QAction(QIcon(":/images/stop.png"), tr("&Close..."), this);
    closeAct->setShortcuts(QKeySequence::Close);
    closeAct->setStatusTip(tr("Close an existing file"));
	connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));
	closeAct->setDisabled(true);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    otsuAct = new QAction(tr("&Otsu multi-level"), this);
    otsuAct->setStatusTip(tr("Otsu multi-level"));
	connect(otsuAct, SIGNAL(triggered()), this, SLOT(otsuImpl()));
	otsuAct->setDisabled(true);

	regionAct = new QAction(tr("&Region growing"), this);
	regionAct->setStatusTip(tr("Region growing"));
	connect(regionAct, SIGNAL(triggered()), this, SLOT(regionImpl()));
	regionAct->setDisabled(true);

	sobelAct = new QAction(tr("So&bel operator"), this);
	sobelAct->setStatusTip(tr("Sobel operator"));
	connect(sobelAct, SIGNAL(triggered()), this, SLOT(sobelImpl()));
	sobelAct->setDisabled(true);

	spiderAct = new QAction(tr("S&pider method"), this);
	spiderAct->setStatusTip(tr("Spider method"));
	connect(spiderAct, SIGNAL(triggered()), this, SLOT(openSpiderDialog()));
	spiderAct->setDisabled(true);

	antAct = new QAction(tr("&Ant method"), this);
	antAct->setStatusTip(tr("Ant method"));
	connect(antAct, SIGNAL(triggered()), this, SLOT(openAntDialog()));
	antAct->setDisabled(true);

	statisticAct = new QAction(tr("Stat&istic"), this);
	statisticAct->setStatusTip(tr("Statistic"));
	connect(statisticAct, SIGNAL(triggered()), this, SLOT(openGT()));
	statisticAct->setDisabled(true);

    connect(sliderXY, SIGNAL(valueChanged(int)), this, SLOT(setValueSlideXY(int)));
    connect(sliderXZ, SIGNAL(valueChanged(int)), this, SLOT(setValueSlideXZ(int)));
    connect(sliderYZ, SIGNAL(valueChanged(int)), this, SLOT(setValueSlideYZ(int)));

    sliderXY->setDisabled(true);
    sliderXZ->setDisabled(true);
    sliderYZ->setDisabled(true);
    slider3D->setDisabled(true);

}
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(closeAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    algMenu = menuBar()->addMenu(tr("&Algorithms"));
    algMenu->addAction(otsuAct);
    algMenu->addAction(regionAct);
    algMenu->addAction(sobelAct);
    algMenu->addSeparator();
    algMenu->addAction(spiderAct);
    algMenu->addAction(antAct);
    algMenu->addSeparator();
    algMenu->addAction(statisticAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(closeAct);

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings("MAS_QT", "Application MAS_QT");
    QPoint pos = settings.value("pos", QPoint(100, 100)).toPoint();
    QSize size = settings.value("size", QSize(800, 600)).toSize();
    resize(size);
    move(pos);
}
void MainWindow::writeSettings()
{
	QSettings settings("MAS_QT", "Application MAS_QT");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

void MainWindow::statisticGeneral(Environment *result, Environment *gt, const char* fn){

	ofstream file;
	file.open(fn);
	if (!file){
		cerr << "Cannot save the files statistic for spider algorithm"  << endl;
		return;
	}

	int size = result->x*result->y*result->z;
	file << "number voxels: " << size << endl;

//	for (int i=0;i<10;i++){
//		file << "region " << i << endl << "\t" ;
//		for (int j=0;j<size;j++){
//			if (gt->voxels[j]==i){
//				file << j << "(" << (int) result->voxels[j] << "),";
//			}
//		}
//		file << endl;
//	}

	file << "histogram :" << endl;
	result->computeHistogram();
	for (int i=0;i<result->grayScale;i++){
		file << i << "(" << result->histogram[i] << ") ";
	}
	file << "\nend histogram :" << endl;


//	map<int, map <int, int> > stat;
//
//	for (int i=0;i<10;i++){
//		map <int, int> rs;
//		stat[i] = rs;
//	}
//	for (int j=0;j<size;j++){
//		stat[gt->voxels[j]][result->voxels[j]]++;
//	}
//
//	for (int i=0;i<10;i++){
//		map <int, int> rs = stat[i];
//		file << "region " << i << endl;
//		for(map<int, int>::iterator iter=rs.begin(); iter!=rs.end(); ++iter)	{
//			file << "\t(value=" <<iter->first << ") : " << iter->second << " voxels" <<endl;
//		}
//	}

	file.close();
}

void MainWindow::statistics(int _x, int _y, int _z, const QString &fileName)
{

	QString saveName = QFileDialog::getSaveFileName(this);
	if (saveName.isEmpty()) return;

	Environment *env = new Environment();
	env->createEnvironment(fileName.toStdString().c_str(), _x,_y,_z, 256);

	ItemWrap *item=list.at(imageList->currentRow());

	clock_t t;
	___clock(t);

	if (item->seg!=0){
		item->seg->statistic(item->env,env,saveName.toStdString().c_str());
	}else{
		statisticGeneral(item->env,env,saveName.toStdString().c_str());
	}
	___clock(t,"Time statistic is ");

	free(env);
	statusBar()->showMessage(tr("Statistics"), 2000);
}

void MainWindow::loadFile(int _x, int _y, int _z, const QString &fileName)
{
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    if (loadGroundTruth) {
    	statistics(_x, _y, _z, fileName);
    	return;
    }

    ItemWrap *item=new ItemWrap();
    item->name = fileName;
    item->env = new Environment();
    clock_t t;
    ___clock(t);
    item->env->createEnvironment(fileName.toStdString().c_str(), _x,_y,_z, 256);
    item->infos.append(QString("Width = %1 \n").arg(_x));
    item->infos.append(QString("Height = %1 \n").arg(_y));
    item->infos.append(QString("Deep = %1 \n").arg(_z));

    item->infos.append("-------------\n");
    item->infos.append(QString("It takes %1 miliseconds time!").arg((float)(clock()-t)/CLOCKS_PER_SEC));

    //item->env->computeHistogram();
    //item->env->writeHistogram();

	list << item;

	statusBar()->showMessage(tr("File loaded"), 2000);

	sliderXY->setDisabled(false);
	sliderXZ->setDisabled(false);
	sliderYZ->setDisabled(false);

	imageList->addItem(item->name);

	imageList->setCurrentRow(list.size()-1);

	otsuAct->setDisabled(false);
	regionAct->setDisabled(false);
	sobelAct->setDisabled(false);
	spiderAct->setDisabled(false);
	antAct->setDisabled(false);
	statisticAct->setDisabled(false);

	saveAct->setDisabled(false);
	closeAct->setDisabled(false);

}

void MainWindow::viewSlideXY(int slide){
	Environment *env = list.at(imageList->currentRow())->env;

	QImage image1(env->x,env->y,QImage::Format_RGB16);
	int idx=env->x*env->y*slide;
	int gray=0;
	byte aa[env->x*env->y];
	int idxAA=0;
	for (int j=0;j<env->y;j++){
		for (int i=0;i<env->x;i++){
		   gray = env->voxels[idx++];
		   image1.setPixel(i, j, qRgb(gray, gray, gray));
		   aa[idxAA++]=gray;
		}
	}
	item1->setPixmap(QPixmap::fromImage(image1));

	//process OpenGL
	vc->checkImageXY = new GLubyte[256*256*3];
	vc->_height = 256;//item->env->y;
	vc->_width = 256;//item->env->x;
	vc->zz=(float)slide/256 + (float)(256-env->z)/(2*256);

	idx=0;
	byte value=0;
	idxAA=0;
	int x1=(256-env->x)/2;
	int x2=x1+env->x;
	int y1=(256-env->y)/2;
	int y2=y1+env->y;

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			if (j>=x1 && j <x2 && i>=y1 && i <y2){
				value = aa[idxAA++];
			}else{
				value = 0;
			}
			vc->checkImageXY[idx++]= (GLubyte) value;
			vc->checkImageXY[idx++]= (GLubyte) value;
			vc->checkImageXY[idx++]= (GLubyte) value;
		}
	}
	vc->makeObjectXY();
	vc->updateGL();
}

void MainWindow::viewSlideXZ(int slide){
	Environment *env = list.at(imageList->currentRow())->env;

	QImage image1(env->x,env->z,QImage::Format_RGB16);
	int idx=env->x*slide;
	int gray=0;
	byte aa[env->x*env->z];
	int idxAA=0;
	for (int j=0;j<env->z;j++){
		for (int i=0;i<env->x;i++){
		   gray = env->voxels[idx++];
		   image1.setPixel(i, j, qRgb(gray, gray, gray));
		   aa[idxAA++]=gray;
		}
		idx += env->x*env->y - env->x;
	}

	item2->setPixmap(QPixmap::fromImage(image1));

	//process OpenGL
	vc->checkImageXZ = new GLubyte[256*256*3];
	vc->_height = 256;//item->env->y;
	vc->_width = 256;//item->env->x;
	vc->yy=(float)slide/256 + (float)(256-env->y)/(2*256);
	idx=0;
	byte value=0;
	int x1=(256-env->x)/2;
	int x2=x1+env->x;
	int y1=(256-env->z)/2;
	int y2=y1+env->z;
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			if (j>=x1 && j <x2 && i>=y1 && i <y2){
				value = aa[--idxAA];
			}else{
				value = 0;
			}
			vc->checkImageXZ[idx++]= (GLubyte) value;
			vc->checkImageXZ[idx++]= (GLubyte) value;
			vc->checkImageXZ[idx++]= (GLubyte) value;
		}
	}
	vc->makeObjectXZ();
	vc->updateGL();
}

void MainWindow::viewSlideYZ(int slide){
	Environment *env = list.at(imageList->currentRow())->env;

	QImage image1(env->y,env->z,QImage::Format_RGB16);
	int idx=slide;
	int gray=0;
	byte aa[env->z*env->y];
	int idxAA=0;
	for (int j=0;j<env->z;j++){
		for (int i=0;i<env->y;i++){
		   gray = env->voxels[idx];
		   image1.setPixel(i, j, qRgb(gray, gray, gray));
		   aa[idxAA++]=gray;
		   idx += env->x;
		}

	}
	item3->setPixmap(QPixmap::fromImage(image1));

	//process OpenGL
	vc->checkImageYZ = new GLubyte[256*256*3];
	vc->_height = 256;//item->env->y;
	vc->_width = 256;//item->env->x;
	vc->xx=(float)slide/256 + (float)(256-env->x)/(2*256);;
	idx=0;
	byte value=0;
	int x1=(256-env->y)/2;
	int x2=x1+env->y;
	int y1=(256-env->z)/2;
	int y2=y1+env->z;
	//idxAA=0;
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			if (j>=x1 && j <x2 && i>=y1 && i <y2){
				//value = env->voxels[start++];
				value = aa[--idxAA];
			}else{
				value = 0;
			}
			vc->checkImageYZ[idx++]= (GLubyte) value;
			vc->checkImageYZ[idx++]= (GLubyte) value;
			vc->checkImageYZ[idx++]= (GLubyte) value;
		}
	}
	vc->makeObjectYZ();
	vc->updateGL();
}

void MainWindow::setValueSlideXY(int value){
	viewSlideXY(value);
}

void MainWindow::setValueSlideXZ(int value){
	viewSlideXZ(value);
}

void MainWindow::setValueSlideYZ(int value){
	viewSlideYZ(value);
}

bool MainWindow::saveFile(const QString &fileName)
{
	QMessageBox::warning(this, tr("Save file"),fileName);
    return true;
}

void MainWindow::selectImage(){

	int crrItem = imageList->currentRow();
	ItemWrap *item = list.at(crrItem);
	Environment *env = item->env;

	sliderXY->setRange(0, env->z-1);
	sliderXZ->setRange(0, env->y-1);
	sliderYZ->setRange(0, env->x-1);

	sliderXY->setValue(env->z/2);
	sliderXZ->setValue(env->y/2);
	sliderYZ->setValue(env->x/2);

	viewSlideXY(env->z/2);
	viewSlideXZ(env->y/2);
	viewSlideYZ(env->x/2);

	infos->setText(item->infos);
}

void MainWindow::otsuImpl(){
	cout << "otsu" << endl;

	Environment *env = list.at(imageList->currentRow())->env;

	ItemWrap *item=new ItemWrap();
	item->name = QString("result otsu");
	item->env = new Environment();

	cout << "===========>threshold_segmentation" << endl;
	clock_t t;
	___clock(t);
	item->seg = new Otsu(env,&conf);
	item->seg->excute(item->env);
	___clock(t,"Time otsu is ");

	item->infos.append(QString("Width = %1 \n").arg(item->env->x));
	item->infos.append(QString("Height = %1 \n").arg(item->env->y));
	item->infos.append(QString("Deep = %1 \n").arg(item->env->z));
	item->infos.append("-------------\n");
	item->infos.append("Parameters:\n");
	item->infos.append(QString("Level threshold = %1\n").arg(conf.infos["number_level_threshold"].c_str()));
	item->infos.append("-------------\n");
	item->infos.append(QString("It takes %1 miliseconds time!").arg((float)(clock()-t)/CLOCKS_PER_SEC));

	list << item;
	imageList->addItem(item->name);
	imageList->setCurrentRow(list.size()-1);

}

void MainWindow::regionImpl(){
	cout << "region" << endl;

	Environment *env = list.at(imageList->currentRow())->env;

	ItemWrap *item=new ItemWrap();
	item->name = QString("result region");
	item->env = new Environment();

	cout << "===========>region_segmentation" << endl;
	clock_t t;
	___clock(t);
	item->seg = new RegionGW(env,&conf);
	item->seg->excute(item->env);
	___clock(t,"Time region is ");

	item->infos.append(QString("Width = %1 \n").arg(item->env->x));
	item->infos.append(QString("Height = %1 \n").arg(item->env->y));
	item->infos.append(QString("Deep = %1 \n").arg(item->env->z));
	item->infos.append("-------------\n");
	item->infos.append("Parameters:\n");
	item->infos.append(QString("Range = %1\n").arg(conf.infos["region_threshold"].c_str()));
	item->infos.append("-------------\n");
	item->infos.append(QString("It takes %1 miliseconds time!").arg((float)(clock()-t)/CLOCKS_PER_SEC));

	list << item;
	imageList->addItem(item->name);
	imageList->setCurrentRow(list.size()-1);

}

void MainWindow::sobelImpl(){
	cout << "sobel" << endl;

	Environment *env = list.at(imageList->currentRow())->env;

	ItemWrap *item=new ItemWrap();
	item->name = QString("result sobel");
	item->env = new Environment();

	cout << "===========>sobel_segmentation" << endl;
	clock_t t;
	___clock(t);
	item->seg = new Sobel(env,&conf);
	item->seg->excute(item->env);
	___clock(t,"Time sobel is ");

	item->infos.append(QString("Width = %1 \n").arg(item->env->x));
		item->infos.append(QString("Height = %1 \n").arg(item->env->y));
		item->infos.append(QString("Deep = %1 \n").arg(item->env->z));
		item->infos.append("-------------\n");
	item->infos.append("Parameters:\n");
	item->infos.append(QString("Sobel dimension = %1\n").arg(conf.infos["sobel_dimension"].c_str()));
	item->infos.append("-------------\n");
	item->infos.append(QString("It takes %1 miliseconds time!").arg((float)(clock()-t)/CLOCKS_PER_SEC));

	list << item;
	imageList->addItem(item->name);
	imageList->setCurrentRow(list.size()-1);

}

void MainWindow::spiderImpl(){
	cout << "spider" << endl;

	Environment *env1 = list.at(imageList->currentRow())->env;

	Environment *env = new Environment();
	env1->copyEnvironment(env,
			atoi(conf.infos["z_from"].c_str()),
			atoi(conf.infos["nbSlice"].c_str()));

	ItemWrap *item=new ItemWrap();
	item->name = QString("result spider");
	item->env = new Environment();

	cout << "===========>spider_segmentation" << endl;
	clock_t t;
	___clock(t);
	if (conf.infos["gpu"]=="on"){
		item->seg = new SpiderSegmentationGPU(&conf,env);
	}else{
		item->seg = new SpiderSegmentation(&conf,env);
	}

	item->seg->excute(item->env);
	___clock(t,"Time spider is ");

	item->infos.append(QString("Width = %1 \n").arg(item->env->x));
	item->infos.append(QString("Height = %1 \n").arg(item->env->y));
	item->infos.append(QString("Deep = %1 \n").arg(item->env->z));
	item->infos.append("-------------\n");
	if (conf.infos["gpu"]=="on"){
		item->infos.append("... run on gpu \n");
	}else{
		item->infos.append("... run on cpu \n");
	}
	item->infos.append("Parameters:\n");
	item->infos.append(QString("Number of colony = %1 \n").arg(conf.infos["spider_number_colony"].c_str()));
	item->infos.append(QString("Number of agent (1 colony) = %1 \n").arg(conf.infos["spider_agent_per_colony"].c_str()));
	item->infos.append(QString("Number of iterator = %1 \n").arg(conf.infos["spider_iterator"].c_str()));
	item->infos.append(QString("Max number dragline for 1 voxel = %1 \n").arg(conf.infos["spider_nbDl"].c_str()));
	item->infos.append(QString("Back probability = %1 \n").arg(conf.infos["backprobability"].c_str()));
	item->infos.append(QString("Attract self = %1 \n").arg(conf.infos["pAttractSelf"].c_str()));
	item->infos.append(QString("Attract others = %1 \n").arg(conf.infos["pAttractOther"].c_str()));
	item->infos.append(QString("Saturation = %1 \n").arg(conf.infos["pSaturation"].c_str()));
	item->infos.append(QString("Default weight = %1 \n").arg(conf.infos["constantWeight"].c_str()));
	item->infos.append("-------------\n");
	item->infos.append(QString("It takes %1 miliseconds time!").arg((float)(clock()-t)/CLOCKS_PER_SEC));

	list << item;
	imageList->addItem(item->name);
	imageList->setCurrentRow(list.size()-1);


}

void MainWindow::antImpl(){
	cout << "ant" << endl;

	Environment *env1 = list.at(imageList->currentRow())->env;

	Environment *env = new Environment();
	env1->copyEnvironment(env,
			atoi(conf.infos["z_from"].c_str()),
			atoi(conf.infos["nbSlice"].c_str()));

	ItemWrap *item=new ItemWrap();
	item->name = QString("result ant");
	item->env = new Environment();

	cout << "===========>ant_segmentation" << endl;
	clock_t t;
	___clock(t);
	if (conf.infos["gpu"]=="on"){
		item->seg = new AntSegmentationGPU(&conf,env);
	}else{
		item->seg = new AntSegmentation(&conf,env);
	}
	item->seg->excute(item->env);
	___clock(t,"Time ant is ");

	item->infos.append(QString("Width = %1 \n").arg(item->env->x));
	item->infos.append(QString("Height = %1 \n").arg(item->env->y));
	item->infos.append(QString("Deep = %1 \n").arg(item->env->z));
	item->infos.append("-------------\n");
	if (conf.infos["gpu"]=="on"){
		item->infos.append("... run on gpu \n");
	}else{
		item->infos.append("... run on cpu \n");
	}
	item->infos.append("Parameters:\n");
	item->infos.append(QString("Number of colony = %1 \n").arg(conf.infos["ant_number_colony"].c_str()));
	item->infos.append(QString("Number of agent (1 colony) = %1 \n").arg(conf.infos["ant_agent_per_colony"].c_str()));
	item->infos.append(QString("Number of iterator = %1 \n").arg(conf.infos["ant_iterator"].c_str()));
	item->infos.append(QString("High ranking = %1 \n").arg(conf.infos["high_ranking"].c_str()));
	item->infos.append(QString("Evaporate pheromone = %1 \n").arg(conf.infos["evaporate_peromone"].c_str()));
	item->infos.append(QString("Dimension 1 = %1 \n").arg(conf.infos["c1"].c_str()));
	item->infos.append(QString("Dimension 2 = %1 \n").arg(conf.infos["c2"].c_str()));
	item->infos.append(QString("Dimension 3 = %1 \n").arg(conf.infos["c3"].c_str()));
	item->infos.append(QString("Dimension 4 = %1 \n").arg(conf.infos["c4"].c_str()));
	item->infos.append("-------------\n");
	item->infos.append(QString("It takes %1 miliseconds time!").arg((float)(clock()-t)/CLOCKS_PER_SEC));

	list << item;
	imageList->addItem(item->name);
	imageList->setCurrentRow(list.size()-1);

}

