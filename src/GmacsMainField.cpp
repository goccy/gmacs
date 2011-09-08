#include <gmacs.hpp>

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
		text = (GmacsTextField *)w;
	} else if (name == "GmacsLineField") {
		line = (GmacsLineField *)w;
	} else if (name == "GmacsStatusBar") {
		bar = (GmacsStatusBar *)w;
	}
}

void GmacsMainField::setConnect()
{
	connect(text, SIGNAL(focusToLine()), line, SLOT(grabFocus()));
	connect(line, SIGNAL(focusToText()), text, SLOT(grabFocus()));
	connect(line, SIGNAL(loadTextSignal(QString)), text, SLOT(loadText(QString)));
}
