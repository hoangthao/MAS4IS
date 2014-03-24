/*
 * opendialog.h
 *
 *  Created on: Oct 25, 2013
 *      Author: thao
 */


#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

class MainWindow;

 class SpiderDialog : public QDialog
 {
     Q_OBJECT

 public:
     SpiderDialog(QWidget *parent = 0, MainWindow* mw=0);
     void retranslateUi();

private slots:
  void accept();
  void reject();

 private:

	 MainWindow* mainWin;
	 QDialogButtonBox *buttonBox;
	     QGroupBox *groupBox;
	     QCheckBox *ckbGPUs;
	     QWidget *layoutWidget;
	     QGridLayout *gridLayout;
	     QLabel *label;
	     QSpinBox *nbColony;
	     QLabel *label_2;
	     QSpinBox *nbAgent;
	     QLabel *label_3;
	     QSpinBox *nbIterator;
	     QSpinBox *nbStart;
	     QLabel *label_10;
	     QLabel *label_11;
	     QSpinBox *nbSlide;
	     QGroupBox *groupBox_2;
	     QWidget *layoutWidget1;
	     QGridLayout *gridLayout_2;
	     QLabel *label_4;
	     QSpinBox *nbDragline;
	     QLabel *label_5;
	     QDoubleSpinBox *pBack;
	     QLabel *label_6;
	     QDoubleSpinBox *pSelf;
	     QLabel *label_7;
	     QDoubleSpinBox *pOthers;
	     QLabel *label_8;
	     QDoubleSpinBox *pSaturation;
	     QLabel *label_9;
	     QDoubleSpinBox *pWeight;

 };


