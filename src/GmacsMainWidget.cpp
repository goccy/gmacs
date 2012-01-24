#include <gmacs.hpp>

#define GOTO_NEXT_TAB() tab->setCurrentIndex(cur_tab_idx+1)
#define GOTO_PREV_TAB() tab->setCurrentIndex(cur_tab_idx-1)
#define GOTO_LAST_TAB() tab->setCurrentIndex(tab->count()-1)

GmacsMainWidget::GmacsMainWidget(QWidget *parent) : QWidget(parent)
{
	const char *argv[] = {"konoha", "hoge.k"};
	konoha_ginit(0, argv);
	konoha_t konoha = konoha_open();
	CTX ctx = (CTX)konoha;
	KONOHA_BEGIN(ctx);
	resize(600, 600);
	setWindowOpacity(0.8);
	//setWindowTitle("Gmacs");
	tab = new GmacsTabWidget(this);
	widget = new GmacsWidget(this);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(QMargins(0, 0, 0, 0));
	layout->addWidget(tab);
	layout->addWidget(widget);
	setLayout(layout);
	tab->addTab(widget, tr("tab1"));
}

void GmacsMainWidget::keyPressEvent(QKeyEvent *event)
{
	if (event->modifiers() & Qt::SHIFT &&
		event->modifiers() & Qt::CTRL) {
		int tab_max_num = tab->count();
		int cur_tab_idx = tab->currentIndex();
		switch (event->key()) {
		case Qt::Key_BraceRight:
			if (tab_max_num - 1 > cur_tab_idx) GOTO_NEXT_TAB();
			break;
		case Qt::Key_BraceLeft:
			if (cur_tab_idx > 0) GOTO_PREV_TAB();
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

void GmacsMainWidget::addTab(void)
{
	GmacsWidget *w = new GmacsWidget(this);
	int index = tab->currentIndex();
	char buf[8];
	snprintf(buf, 8, "tab%d", index+2);
	tab->addTab(w, tr(buf));
	GOTO_LAST_TAB();
}
