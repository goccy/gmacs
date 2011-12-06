#include <gmacs.hpp>

GmacsWidget::GmacsWidget(QWidget *parent) : QWidget(parent)
{
	main_field = new GmacsMainField();
	command_area = new GmacsLineField();
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(QMargins(0, 0, 0, 0));
	layout->addWidget(main_field);
	layout->addWidget(command_area);
	setLayout(layout);
	GmacsTextField *edit_area = main_field->edit_area;
	connect(edit_area, SIGNAL(focusToLine()), command_area, SLOT(grabFocus()));
	connect(command_area, SIGNAL(focusToText()), edit_area, SLOT(grabFocus()));
	connect(command_area, SIGNAL(loadTextSignal(QString)), edit_area, SLOT(loadText(QString)));
}
