#include <gmacs.hpp>

using namespace std;

GmacsMainWindow::GmacsMainWindow(QMainWindow *parent) : QMainWindow(parent)
{
	resize(600, 600);
	setWindowOpacity(0.9);
	setWindowTitle("Gmacs");
}

class GmacsMainField : public QWidget {
	QVBoxLayout *layout;
	GmacsTextField *gtf;
	GmacsLineField *glf;
public:
	GmacsMainField(QWidget *parent = 0);
	void addWidget(QWidget *widget);
	void setReference();
};

GmacsMainField::GmacsMainField(QWidget *parent) : QWidget(parent)
{
	layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(QMargins(0, 0, 0, 0));
	setLayout(layout);
}

void GmacsMainField::addWidget(QWidget *w)
{
	layout->addWidget(w);
	QString name = w->objectName();
	if (name == "GmacsTextField") {
		gtf = (GmacsTextField *)w;
	} else if (name == "GmacsLineField") {
		glf = (GmacsLineField *)w;
	}
}

void GmacsMainField::setReference()
{
	gtf->glf = glf;
	glf->gtf = gtf;
}

class GmacsStatusBar : public QLabel {
public:
	GmacsStatusBar(QLabel *parent = 0);

};

GmacsStatusBar::GmacsStatusBar(QLabel *parent) : QLabel(parent)
{
	setStyleSheet("background-color:#1e90ff");
	setFixedHeight(20);
}

class Gmacs {
public:
	QApplication *app;
	Gmacs(int argc, char **argv);
	void start();
};

Gmacs::Gmacs(int argc, char **argv)
{
	QFont font;
	font.setPointSize(18);
	font.setFixedPitch(true);
	font.setBold(false);
	app = new QApplication(argc, argv);
	app->setFont(font);
}

void Gmacs::start()
{
	app->exec();
}

int main(int argc, char **argv)
{
	Gmacs *gmacs = new Gmacs(argc, argv);
	GmacsMainWindow *window = new GmacsMainWindow();
	GmacsMainField *main = new GmacsMainField();
	GmacsTextField *gtf = new GmacsTextField();
	GmacsStatusBar *sb = new GmacsStatusBar();
	GmacsLineField *glf = new GmacsLineField();
	main->addWidget(gtf);
	main->addWidget(sb);
	main->addWidget(glf);
	main->setReference();
	window->setCentralWidget(main);
	window->show();
	gmacs->start();
}
