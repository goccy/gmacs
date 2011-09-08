#include <gmacs.hpp>

using namespace std;

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
	main->setConnect();
	window->setMainField(main);
	window->show();
	gmacs->start();
}
