/*
 * opendialog.cpp
 *
 *  Created on: Oct 25, 2013
 *      Author: thao
 */



#include "opendialog.h"
#include "mainwindow.h"

#include <QtGui/QWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QFileDialog>

 OpenDialog::OpenDialog(QWidget *parent, MainWindow* mw)
     : QDialog(parent)
 {
	 resize(386, 139);
	 buttonBox = new QDialogButtonBox(this);
	 buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
	 buttonBox->setGeometry(QRect(30, 100, 341, 32));
	 buttonBox->setOrientation(Qt::Horizontal);
	 buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
	 widget = new QWidget(this);
	 widget->setObjectName(QString::fromUtf8("widget"));
	 widget->setGeometry(QRect(10, 10, 371, 77));
	 gridLayout = new QGridLayout(widget);
	 gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	 gridLayout->setContentsMargins(0, 0, 0, 0);
	 label = new QLabel(widget);
	 label->setObjectName(QString::fromUtf8("label"));

	 gridLayout->addWidget(label, 0, 0, 1, 1);

	 lineEdit = new QLineEdit(widget);
	 lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

	 gridLayout->addWidget(lineEdit, 0, 1, 1, 3);

	 pushButton = new QPushButton(widget);
	 pushButton->setObjectName(QString::fromUtf8("pushButton"));

	 gridLayout->addWidget(pushButton, 0, 4, 1, 1);

	 label_2 = new QLabel(widget);
	 label_2->setObjectName(QString::fromUtf8("label_2"));

	 gridLayout->addWidget(label_2, 1, 0, 1, 1);

	 spinBox = new QSpinBox(widget);
	 spinBox->setObjectName(QString::fromUtf8("spinBox"));
	 spinBox->setMaximum(1000);
	 spinBox->setValue(181);

	 gridLayout->addWidget(spinBox, 1, 1, 1, 1);

	 label_4 = new QLabel(widget);
	 label_4->setObjectName(QString::fromUtf8("label_4"));

	 gridLayout->addWidget(label_4, 1, 2, 1, 1);

	 spinBox_3 = new QSpinBox(widget);
	 spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));
	 spinBox_3->setMaximum(1000);
	 spinBox_3->setValue(181);

	 gridLayout->addWidget(spinBox_3, 1, 3, 1, 1);

	 label_3 = new QLabel(widget);
	 label_3->setObjectName(QString::fromUtf8("label_3"));

	 gridLayout->addWidget(label_3, 2, 0, 1, 1);

	 spinBox_2 = new QSpinBox(widget);
	 spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
	 spinBox_2->setMaximum(1000);
	 spinBox_2->setValue(217);

	 gridLayout->addWidget(spinBox_2, 2, 1, 1, 1);

	 label_5 = new QLabel(widget);
	 label_5->setObjectName(QString::fromUtf8("label_5"));

	 gridLayout->addWidget(label_5, 2, 2, 1, 1);

	 comboBox = new QComboBox(widget);
	 comboBox->setObjectName(QString::fromUtf8("comboBox"));
	comboBox->addItems(QStringList()
				<< "Gray scale (255)");

	 gridLayout->addWidget(comboBox, 2, 3, 1, 1);

	 retranslateUi();
	 mainWin = mw;

	 connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	 connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	 connect(pushButton, SIGNAL(clicked()), this, SLOT(open()));


 }

 void OpenDialog::open(){
	 QString fileName = QFileDialog::getOpenFileName(this);
	 lineEdit->setText(fileName);
 }

 void OpenDialog::retranslateUi()
    {
        setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog", "File name", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Dialog", "Browser...", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Dialog", "Width (x)", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Dialog", "Deep (z)", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Dialog", "Height (y)", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Dialog", "Type", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

void OpenDialog::accept(){
	mainWin->loadFile(spinBox->value(),spinBox_2->value(),spinBox_3->value(),lineEdit->text());
	hide();

}

void OpenDialog::reject(){
	hide();
}
