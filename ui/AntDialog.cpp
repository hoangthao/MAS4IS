/*
 * opendialog.cpp
 *
 *  Created on: Oct 25, 2013
 *      Author: thao
 */



#include "AntDialog.h"
#include "mainwindow.h"

#include <QtGui/QWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QFileDialog>

 AntDialog::AntDialog(QWidget *parent, MainWindow* mw)
     : QDialog(parent)
 {
	 resize(375, 413);
	 buttonBox = new QDialogButtonBox(this);
	buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
	buttonBox->setGeometry(QRect(50, 370, 291, 32));
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
	groupBox = new QGroupBox(this);
	groupBox->setObjectName(QString::fromUtf8("groupBox"));
	groupBox->setGeometry(QRect(10, 10, 331, 161));
	ckbGPUs = new QCheckBox(groupBox);
	ckbGPUs->setObjectName(QString::fromUtf8("ckbGPUs"));
	ckbGPUs->setGeometry(QRect(240, 20, 81, 17));
	layoutWidget = new QWidget(groupBox);
	layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
	layoutWidget->setGeometry(QRect(10, 20, 211, 126));
	gridLayout = new QGridLayout(layoutWidget);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	gridLayout->setContentsMargins(0, 0, 0, 0);
	nbColony = new QSpinBox(layoutWidget);
	nbColony->setObjectName(QString::fromUtf8("nbColony"));
	nbColony->setMinimum(1);
	nbColony->setMaximum(1);
	nbColony->setValue(1);

	gridLayout->addWidget(nbColony, 0, 1, 1, 1);

	label = new QLabel(layoutWidget);
	label->setObjectName(QString::fromUtf8("label"));

	gridLayout->addWidget(label, 0, 0, 1, 1);

	label_2 = new QLabel(layoutWidget);
	label_2->setObjectName(QString::fromUtf8("label_2"));

	gridLayout->addWidget(label_2, 1, 0, 1, 1);

	nbAgent = new QSpinBox(layoutWidget);
	nbAgent->setObjectName(QString::fromUtf8("nbAgent"));
	nbAgent->setMinimum(1);
	nbAgent->setMaximum(100000000);
	nbAgent->setValue(10000);

	gridLayout->addWidget(nbAgent, 1, 1, 1, 1);

	label_3 = new QLabel(layoutWidget);
	label_3->setObjectName(QString::fromUtf8("label_3"));

	gridLayout->addWidget(label_3, 2, 0, 1, 1);

	nbIterator = new QSpinBox(layoutWidget);
	nbIterator->setObjectName(QString::fromUtf8("nbIterator"));
	nbIterator->setMinimum(1);
	nbIterator->setMaximum(100000000);
	nbIterator->setValue(50);

	gridLayout->addWidget(nbIterator, 2, 1, 1, 1);

	label_10 = new QLabel(layoutWidget);
	label_10->setObjectName(QString::fromUtf8("label_10"));

	gridLayout->addWidget(label_10, 3, 0, 1, 1);

	nbStart = new QSpinBox(layoutWidget);
	nbStart->setObjectName(QString::fromUtf8("nbStart"));
	nbStart->setMinimum(0);
	nbStart->setMaximum(1000);
	nbStart->setValue(0);

	gridLayout->addWidget(nbStart, 3, 1, 1, 1);

	label_11 = new QLabel(layoutWidget);
	label_11->setObjectName(QString::fromUtf8("label_11"));

	gridLayout->addWidget(label_11, 4, 0, 1, 1);

	nbSlide = new QSpinBox(layoutWidget);
	nbSlide->setObjectName(QString::fromUtf8("nbSlide"));
	nbSlide->setMinimum(1);
	nbSlide->setMaximum(1000);
	nbSlide->setValue(5);

	gridLayout->addWidget(nbSlide, 4, 1, 1, 1);

	ckbGPUs->raise();
	layoutWidget->raise();
	nbIterator->raise();
	label_3->raise();
	groupBox_2 = new QGroupBox(this);
	groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
	groupBox_2->setGeometry(QRect(10, 180, 331, 181));
	layoutWidget1 = new QWidget(groupBox_2);
	layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
	layoutWidget1->setGeometry(QRect(10, 20, 271, 152));
	gridLayout_2 = new QGridLayout(layoutWidget1);
	gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
	gridLayout_2->setContentsMargins(0, 0, 0, 0);
	label_4 = new QLabel(layoutWidget1);
	label_4->setObjectName(QString::fromUtf8("label_4"));

	gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

	label_5 = new QLabel(layoutWidget1);
	label_5->setObjectName(QString::fromUtf8("label_5"));

	gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

	pPheromeno = new QDoubleSpinBox(layoutWidget1);
	pPheromeno->setObjectName(QString::fromUtf8("pPheromeno"));
	pPheromeno->setValue(0.2);

	gridLayout_2->addWidget(pPheromeno, 1, 1, 1, 1);

	label_6 = new QLabel(layoutWidget1);
	label_6->setObjectName(QString::fromUtf8("label_6"));

	gridLayout_2->addWidget(label_6, 2, 0, 1, 1);

	pDim1 = new QDoubleSpinBox(layoutWidget1);
	pDim1->setObjectName(QString::fromUtf8("pDim1"));
	pDim1->setValue(1.0);

	gridLayout_2->addWidget(pDim1, 2, 1, 1, 1);

	label_7 = new QLabel(layoutWidget1);
	label_7->setObjectName(QString::fromUtf8("label_7"));

	gridLayout_2->addWidget(label_7, 3, 0, 1, 1);

	pDim2 = new QDoubleSpinBox(layoutWidget1);
	pDim2->setObjectName(QString::fromUtf8("pDim2"));
	pDim2->setValue(1.0);

	gridLayout_2->addWidget(pDim2, 3, 1, 1, 1);

	label_8 = new QLabel(layoutWidget1);
	label_8->setObjectName(QString::fromUtf8("label_8"));

	gridLayout_2->addWidget(label_8, 4, 0, 1, 1);

	pDim3 = new QDoubleSpinBox(layoutWidget1);
	pDim3->setObjectName(QString::fromUtf8("pDim3"));
	pDim3->setValue(1.0);

	gridLayout_2->addWidget(pDim3, 4, 1, 1, 1);

	label_9 = new QLabel(layoutWidget1);
	label_9->setObjectName(QString::fromUtf8("label_9"));

	gridLayout_2->addWidget(label_9, 5, 0, 1, 1);

	pDim4 = new QDoubleSpinBox(layoutWidget1);
	pDim4->setObjectName(QString::fromUtf8("pDim4"));
	pDim4->setValue(1.0);

	gridLayout_2->addWidget(pDim4, 5, 1, 1, 1);

	pHighRanking = new QDoubleSpinBox(layoutWidget1);
	pHighRanking->setObjectName(QString::fromUtf8("pHighRanking"));
	pHighRanking->setValue(1);

	gridLayout_2->addWidget(pHighRanking, 0, 1, 1, 1);


	 retranslateUi();
	 mainWin = mw;


	 connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	 connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

 }


 void AntDialog::retranslateUi()
    {
	 setWindowTitle(QApplication::translate("Dialog", "Parameters for ants", 0, QApplication::UnicodeUTF8));
	         groupBox->setTitle(QApplication::translate("Dialog", "Multi-agent system", 0, QApplication::UnicodeUTF8));
	         ckbGPUs->setText(QApplication::translate("Dialog", "using GPUs", 0, QApplication::UnicodeUTF8));
	         label->setText(QApplication::translate("Dialog", "Number colony", 0, QApplication::UnicodeUTF8));
	         label_2->setText(QApplication::translate("Dialog", "Number agent of a colony", 0, QApplication::UnicodeUTF8));
	         label_3->setText(QApplication::translate("Dialog", "Number iterator", 0, QApplication::UnicodeUTF8));
	         label_10->setText(QApplication::translate("Dialog", "Start slice", 0, QApplication::UnicodeUTF8));
	         label_11->setText(QApplication::translate("Dialog", "Number of slice", 0, QApplication::UnicodeUTF8));
	         groupBox_2->setTitle(QApplication::translate("Dialog", "Behaviors", 0, QApplication::UnicodeUTF8));
	         label_4->setText(QApplication::translate("Dialog", "High ranking", 0, QApplication::UnicodeUTF8));
	         label_5->setText(QApplication::translate("Dialog", "Evaporate pheromone", 0, QApplication::UnicodeUTF8));
	         label_6->setText(QApplication::translate("Dialog", "Direction 1", 0, QApplication::UnicodeUTF8));
	         label_7->setText(QApplication::translate("Dialog", "Direction 2", 0, QApplication::UnicodeUTF8));
	         label_8->setText(QApplication::translate("Dialog", "Direction 3", 0, QApplication::UnicodeUTF8));
	         label_9->setText(QApplication::translate("Dialog", "Direction 4", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

void AntDialog::accept(){
	mainWin->conf.infos["z_from"] = nbStart->text().toStdString().c_str();
	mainWin->conf.infos["nbSlice"] = nbSlide->text().toStdString().c_str();
	mainWin->conf.infos["gpu"] = ckbGPUs->isChecked()?"on":"off";
	mainWin->conf.infos["ant_number_colony"] = nbColony->text().toStdString().c_str();
	mainWin->conf.infos["ant_agent_per_colony"] = nbAgent->text().toStdString().c_str();
	mainWin->conf.infos["ant_iterator"] = nbIterator->text().toStdString().c_str();
	mainWin->conf.infos["high_ranking"] = pHighRanking->text().toStdString().c_str();
	mainWin->conf.infos["evaporate_peromone"] = pPheromeno->text().toStdString().c_str();
	mainWin->conf.infos["c1"] = pDim1->text().toStdString().c_str();
	mainWin->conf.infos["c2"] = pDim2->text().toStdString().c_str();
	mainWin->conf.infos["c3"] = pDim3->text().toStdString().c_str();
	mainWin->conf.infos["c4"] = pDim4->text().toStdString().c_str();
	mainWin->antImpl();
	hide();

}

void AntDialog::reject(){
	hide();
}
