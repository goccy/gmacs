#include <gmacs.hpp>

GmacsStatusBar::GmacsStatusBar(QLabel *parent) : QLabel(parent)
{
	setStyleSheet("background-color:#1e90ff");
	setFixedHeight(20);
}
