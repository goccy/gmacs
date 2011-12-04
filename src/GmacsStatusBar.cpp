#include <gmacs.hpp>

GmacsStatusBar::GmacsStatusBar(QLabel *parent) : QLabel(parent)
{
	setStyleSheet("background: qradialgradient(cx:0.5, cy:0.5, radius:1, fx:0.5, fy:0.5, stop:0 white, stop:1 #1e90ff);");
	//setStyleSheet("background-color:#1e90ff");
	setFixedHeight(20);
}
