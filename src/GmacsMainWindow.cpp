#include <gmacs.hpp>

GmacsMainWindow::GmacsMainWindow(QWidget *parent) : QWidget(parent)
{
	resize(600, 600);
	setWindowOpacity(0.8);
	setWindowTitle("Gmacs");
	tab = new GmacsTabWidget(this);
	widget = new GmacsWidget(this);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(QMargins(0, 0, 0, 0));
	layout->addWidget(tab);
	layout->addWidget(widget);
	setLayout(layout);
	tab->addTab(widget, tr("Gmacs"));
}

void GmacsMainWindow::keyPressEvent(QKeyEvent *event)
{
	if (event->modifiers() & Qt::SHIFT &&
		event->modifiers() & Qt::CTRL) {
		int tab_max_num = tab->count();
		int cur_tab_idx = tab->currentIndex();
		//fprintf(stderr, "tab_max_num = [%d]\n", tab_max_num);
		//fprintf(stderr, "cur_tab_idx = [%d]\n", cur_tab_idx);
		switch (event->key()) {
		case Qt::Key_BracketRight:
			//switch right tab
			if (tab_max_num - 1 > cur_tab_idx) {
				tab->setCurrentIndex(cur_tab_idx+1);
			}
			break;
		case Qt::Key_BracketLeft:
			//switch left tab
			if (cur_tab_idx > 0) {
				tab->setCurrentIndex(cur_tab_idx-1);
			}
			break;
		default:
			break;
		}
	} else if (event->modifiers() & Qt::CTRL) {
		if (event->key() == Qt::Key_T) {
			addTab();
		}
	}
	QWidget::keyPressEvent(event);
}

void GmacsMainWindow::addTab(void)
{
	GmacsWidget *w = new GmacsWidget(this);
	tab->addTab(w, tr("Gmacs"));
}
