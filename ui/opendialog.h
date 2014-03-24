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
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

class MainWindow;

 class OpenDialog : public QDialog
 {
     Q_OBJECT

 public:
     OpenDialog(QWidget *parent = 0, MainWindow* mw=0);
     void retranslateUi();

private slots:
  void accept();
  void reject();
  void open();

 private:
     QDialogButtonBox *buttonBox;
	 QWidget *widget;
	 QGridLayout *gridLayout;
	 QLabel *label;
	 QLineEdit *lineEdit;
	 QPushButton *pushButton;
	 QLabel *label_2;
	 QSpinBox *spinBox;
	 QLabel *label_4;
	 QSpinBox *spinBox_3;
	 QLabel *label_3;
	 QSpinBox *spinBox_2;
	 QLabel *label_5;
	 QComboBox *comboBox;

	 MainWindow* mainWin;
 };


