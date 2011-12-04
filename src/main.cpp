#include <gmacs.hpp>

int main(int argc, char **argv)
{
	Gmacs gmacs(argc, argv);
	GmacsMainWindow window;
	GmacsMainField main;
	GmacsTextField edit_area;
	GmacsStatusBar status;
	GmacsLineField command_area;
	main.addWidget(&edit_area);
	main.addWidget(&status);
	main.addWidget(&command_area);
	main.setConnect();
	window.setMainField(&main);
	window.show();
	gmacs.start();
}
