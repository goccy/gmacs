#include <gmacs.hpp>

GmacsTabWidget::GmacsTabWidget(QWidget *parent) : QTabWidget(parent)
{
	setTabShape(QTabWidget::Triangular);
	setMovable(true);
	//setTabClosable(true);
	//setDocumentMode(true);
}
