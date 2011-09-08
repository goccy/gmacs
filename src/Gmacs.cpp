#include <gmacs.hpp>

Gmacs::Gmacs(int argc, char **argv)
{
	QFont font("monaco");
	font.setPointSize(14);
	font.setFixedPitch(true);
	font.setBold(false);
	app = new QApplication(argc, argv);
	app->setFont(font);
}

void Gmacs::start()
{
	app->exec();
}
