#include <gmacs.hpp>

GmacsMainWindow::GmacsMainWindow(QMainWindow *parent) : QMainWindow(parent)
{
	resize(600, 600);
	setWindowOpacity(0.8);
	setWindowTitle("Gmacs");
}

void GmacsMainWindow::setMainField(GmacsMainField *main)
{
	GmacsCompletion *comp = main->text->comp;
	comp->main_window = this;//for window pos
	setCentralWidget(main);
}
