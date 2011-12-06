#include <gmacs.hpp>

GmacsCompleter::GmacsCompleter(QPlainTextEdit *editor) : QCompleter()
{
	//setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	//setCaseSensitivity(Qt::CaseInsensitive);
	setWrapAround(false);
	setMaxVisibleItems(20);
	setCompletionMode(QCompleter::PopupCompletion);
	setWidget(editor);
	QObject::connect(this, SIGNAL(activated(QString)),
					 editor, SLOT(insertCompletion(QString)));
}

/*
GmacsCompletion::GmacsCompletion(QListWidget *parent) : QListWidget(parent)
{
	setWindowFlags(Qt::Popup);
	QFont font("monaco");
	font.setPointSize(14);
	font.setFixedPitch(true);
	font.setBold(false);
	setFont(font);
	setMidLineWidth(1);
	resize(QSize(200, 150));
	setWindowOpacity(0.9);
	qDebug() << "completion!!";
}

void GmacsCompletion::setCompletionList(QStringList l)
{
	for (int i = 0; i < l.size(); i++) {
		if (!existsWord(l[i])) {
			completion_list << l[i];
		}
	}
}

bool GmacsCompletion::existsWord(QString word)
{
	int size = completion_list.size();
	for (int i = 0; i < size; i++) {
		if (completion_list[i] == word) {
			return true;
		}
	}
	return false;
}

void GmacsCompletion::addList(QString str)
{
	(void)str;
}

void GmacsCompletion::dumpList(void)
{
	int size = completion_list.size();
	for (int i = 0; i < size; i++) {
		qDebug() << completion_list[i];
	}
}

void GmacsCompletion::open(QTextCursor *cursor, QRect *rect)
{
	QPoint window_pos = main_window->pos();
	setPosition(window_pos.x() + rect->left(), window_pos.y() + rect->bottom() + 30);
	int pos = cursor->position();
	cursor->movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
	QString word = cursor->selectedText();
	qDebug() << "word = " << word;
	if (word == "") return;
	int size = completion_list.size();
	for (int i = 0; i < size; i++) {
		if (completion_list[i].contains(QRegExp("^" + word))) {
			qDebug() << "add item = " << completion_list[i];
			addItem(completion_list[i]);
		}
	}
	if (count() == 0) {
		cursor->movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
		pos -= cursor->position();
		cursor->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, pos);
		return;
	}
	activateWindow();
	setCurrentRow(0);
	//list->setFocus(Qt::PopupFocusReason);
	cursor->movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	pos -= cursor->position();
	cursor->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, pos);
	this->show();
}

void GmacsCompletion::setPosition(int x, int y)
{
	move(x, y);
}

void GmacsCompletion::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Up: {
		int r = currentRow();
		if (r > 0) {
			setCurrentRow(r-1);
		}
		break;
	}
	case Qt::Key_Down: {
		int r = currentRow();
		setCurrentRow(r+1);
		break;
	}
	default:
		close();
		break;
	}
}

void GmacsCompletion::close(void)
{
	int max_num = count();
	qDebug() << "max_num = " << max_num;
	for (int i = 0; i < max_num; i++) {
		takeItem(i);
	}
	qDebug() << "after count = " << count();
	releaseKeyboard();
	//gtf->grabKeyboard();
	this->hide();
}
*/
