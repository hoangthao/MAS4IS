//#include "core/misc.h"
//#include "core/Config.h"
//#include "core/Environment.h"
//#include "core/Segmentation.h"
//#include "magent/SpiderSegmentation.h"
//#include "magent/SpiderSegmentationGPU.h"
//
//
//clock_t t;
//
//
//int main(int argc, char *argv[]) {
//
//	Config* conf = new Config();
//	Environment* env = new Environment();
//
//	conf->readFile("params.cfg");
//	env->createEnvironment(conf->infos["file_name"].c_str(),
//			atoi(conf->infos["x_dimension"].c_str()),
//			atoi(conf->infos["y_dimension"].c_str()),
//			atoi(conf->infos["z_dimension"].c_str()),
//			atoi(conf->infos["gray_scale"].c_str()));
//	___clock(t);
//	SpiderSegmentationGPU* seg = new SpiderSegmentationGPU(conf,env);
//	Environment* result = new Environment();
//	seg->excute(result);
//	result->writeRawb("image.rawb");
//	___clock(t,"Time collapse is ");
//
//	free(conf);
//	free(env);
//	free(result);
//    return 0;
//}

#include <QtGui/QApplication>
#include <QtOpenGL/QGLFormat>

#include "ui/mainwindow.h"

#include <iostream>

int main(int argc, char *argv[]) {

	QApplication app(argc, argv);

	if (!QGLFormat::hasOpenGL()) {
		std::cerr << "This system has no OpenGL support" << std::endl;
		return 1;
	}

	app.setOrganizationName("MAS_QT");
	app.setApplicationName("Application MAS_QT");
	MainWindow mainWin;
	mainWin.show();

    return app.exec();
}

