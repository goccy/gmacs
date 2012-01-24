#include <gmacs.hpp>

Gmacs::Gmacs(int &argc, char **argv) : QApplication(argc, argv)
{
	QFont font("monaco");
	font.setPointSize(14);
	font.setFixedPitch(true);
	font.setBold(false);
	setFont(font);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QString stylefile = "style.qss";
	if (QFile::exists(stylefile)) {
		QFile file(stylefile);
		file.open(QFile::ReadOnly);
		QString styleSheet = QLatin1String(file.readAll());
		setStyleSheet(styleSheet);
		file.close();
	}
}

void Gmacs::start()
{
	exec();
}
