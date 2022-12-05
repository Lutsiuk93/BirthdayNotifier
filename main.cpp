#include "BirthdayNotifierWindow.h"
#include <QtWidgets/QApplication>
#include "BirthdayNotifierModel.h"
#include "qsettings.h"
#include "qdir.h"

int main(int argc, char* argv[])
{
	QApplication application(argc, argv);
	QDir::setCurrent(qApp->applicationDirPath());
	BirthdayNotifierModel model = {};
	BirthdayNotifierWindow window(&model);
	model.LoadFromFile();
	
	if (QString(argv[1]) != QString("inBackground"))	
		window.show();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Add this application to windows autorun
#ifdef Q_OS_WIN32
	QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
	settings.setValue("BirthdayNotifier", QDir::toNativeSeparators(application.applicationFilePath()) + " inBackground");
	settings.sync();
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return application.exec();
}
