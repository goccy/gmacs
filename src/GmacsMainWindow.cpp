#include <gmacs.hpp>

GmacsStateViewer::GmacsStateViewer(QGraphicsScene *scene) : QGraphicsView(scene)
{
}

GmacsMainWindow::GmacsMainWindow(QWidget *parent) : QMainWindow(parent)
{
	GmacsMainWidget *mainwidget = new GmacsMainWidget();
	setCentralWidget(mainwidget);
	GmacsTextField *edit_area = mainwidget->widget->main_field->edit_area;
	GmacsInteractiveDesigner *designer = edit_area->designer;
	GmacsKeyBind *kb = edit_area->kb;
	GmacsStateViewer *viewer = new GmacsStateViewer(designer->scene);
	dock = new QDockWidget(this, Qt::Drawer);
	dock->setWidget(viewer);
	dock->hide();
	addDockWidget(Qt::RightDockWidgetArea, dock);
	resize(600, 600);
	connect(kb, SIGNAL(emitToggleStateDockSignal()), this, SLOT(toggleDockWidget()));
}

void GmacsMainWindow::toggleDockWidget(void)
{
	if (dock->isVisible()) {
		dock->hide();
	} else {
		dock->show();
	}
}
