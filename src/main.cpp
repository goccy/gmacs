#include <gmacs.hpp>

int main(int argc, char **argv)
{
	Gmacs gmacs(argc, argv);
	GmacsMainWindow window;
	window.show();
	gmacs.start();
}
