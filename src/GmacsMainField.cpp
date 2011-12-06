#include <gmacs.hpp>

GmacsMainField::GmacsMainField(QWidget *parent) : QWidget(parent)
{
	edit_area = new GmacsTextField();
	status_bar = new GmacsStatusBar();;
	layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(QMargins(0, 0, 0, 0));
	layout->addWidget(edit_area);
	layout->addWidget(status_bar);
	setLayout(layout);
}
