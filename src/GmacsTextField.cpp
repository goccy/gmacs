#include <gmacs.hpp>

using namespace std;
GmacsTextField::GmacsTextField(QTextEdit *parent) : QTextEdit(parent)
{
	setStyleSheet("background-color:black;" "color:white;" "font-family: monaco;");
	setLineWrapMode(QTextEdit::NoWrap);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setCursorWidth(0);
	cursor = textCursor();
	QFontMetrics metrics(font());
	int space_size = metrics.width(' ');
	fprintf(stderr, "space size = [%d]\n", space_size);
	setTabStopWidth(space_size * 4);
	setObjectName("GmacsTextField");
	setTextCursor(cursor);
	isFocus = true;
	isHighlightAll = false;
	isOpenCompletionWindow = false;
	isFindFileMode = false;
	const int blinkPeriod = 500;
	startTimer(blinkPeriod);
	sh = new GmacsSyntaxHighlighter(this);
	c = new GmacsCompleter(this);
	GmacsScriptLoader *sl = new GmacsScriptLoader();
	white.setForeground(Qt::white);
	//comp = new GmacsCompletion();
	kb = new GmacsKeyBind();
	script_loader = new GmacsScriptLoader();
	connect(kb, SIGNAL(emitFindFileSignal()), this, SLOT(findFile()));
}

void GmacsTextField::insertCompletion(const QString& completion)
{
	fprintf(stderr, "insertCompletion!!\n");
	if (c->widget() != this) return;
	QTextCursor tc = textCursor();
	int extra = completion.length() - c->completionPrefix().length();
	tc.movePosition(QTextCursor::Left);
	tc.movePosition(QTextCursor::EndOfWord);
	tc.insertText(completion.right(extra));
	setTextCursor(tc);
}

void GmacsTextField::timerEvent(QTimerEvent *event)
{
	if (isCurVisible) {
		isCurVisible = false;
	} else {
		isCurVisible = true;
	}
	isKeyPress = false;
}

void GmacsTextField::paintEvent(QPaintEvent *event)
{
	if (isFocus && (isKeyPress || isCurVisible) ||
		isHighlightAll) {
		drawCursor();
	}
	setCursorWidth(0);
	QTextEdit::paintEvent(event);
	setCursorWidth(10);
}

void GmacsTextField::drawCursor()
{
	QRect r = cursorRect();
	r.setWidth(10);
	QPainter painter(viewport());
	painter.setOpacity(0.6);
	painter.fillRect(r, Qt::white);
}

void GmacsTextField::keyPressEvent(QKeyEvent *event)
{
	isKeyPress = true;
	if (event->modifiers() == Qt::META) {
		GmacsKeyBindFunc func = kb->getKeyBindFunction(event);
		if (func != NULL) {
			(kb->*func)(&cursor);
			setTextCursor(cursor);
		}
		kill_buf_count = 0;
		command_count = 0;
	}
	switch (event->key()) {
	case Qt::Key_Backspace:
		kill_buf_count = 0;
		if (cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor)) {
			cursor.deleteChar();
		}
		command_count = 0;
		break;
	case Qt::Key_Return:
		if (c && c->popup()->isVisible()) {
			event->ignore();
			return;
		} else {
			cursor.insertText("\n");
			command_count = 0;
		}
		break;
	case Qt::Key_Enter:
	case Qt::Key_Escape:
	case Qt::Key_Tab:
	case Qt::Key_Backtab:
		if (c && c->popup()->isVisible()) {
			event->ignore();
			return;
		}
	default:
		isOpenCompletionWindow = false;
		kill_buf_count = 0;
		cursor.insertText(event->text());
		setTextCursor(cursor);
		command_count = 0;
		break;
	}

	bool isShortcut = ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_U); // CTRL+U
	const bool ctrlOrShift = event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
	if (!c || (ctrlOrShift && event->text().isEmpty())) return;
	static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
	bool hasModifier = (event->modifiers() != Qt::NoModifier) && !ctrlOrShift;
	QString completionPrefix = textUnderCursor();
	if (!isShortcut && (hasModifier || event->text().isEmpty() ||
						completionPrefix.length() < 3 || eow.contains(event->text().right(1)))) {
		fprintf(stderr, "hide\n");
		c->popup()->hide();
		return;
	}
	qDebug() << completionPrefix;
	if (completionPrefix != c->completionPrefix()) {
		c->setCompletionPrefix(completionPrefix);
		c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
	}

	QRect cr = cursorRect();
	cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
	c->complete(cr); // popup it up!
	setTextCursor(cursor);
}

QString GmacsTextField::textUnderCursor(void) const
{
	QTextCursor tc = textCursor();
	tc.select(QTextCursor::WordUnderCursor);
	return tc.selectedText();
}

void GmacsTextField::focusInEvent(QFocusEvent *e)
{
	if (c) c->setWidget(this);
	QTextEdit::focusInEvent(e);
}

void GmacsTextField::mousePressEvent(QMouseEvent *event)
{
	(void)event;
	isFocus = true;
}

void GmacsTextField::findFile(void)
{
	isFocus = false;
	emit focusToLine();
}

void GmacsTextField::grabFocus()
{
	isFocus = true;
	setFocus(Qt::OtherFocusReason);
}

void GmacsTextField::loadText(QString filepath)
{
	//cout << qPrintable(filepath) << endl;
	QString buf = script_loader->loadScript(filepath);
	//cout << qPrintable(buf);
	setPlainText(buf);
	QTextCursor cur = textCursor();
	cursor = cur;
	QStringList words;
	QStringList list = buf.split("\n");
	QRegExp word_pattern("\\b[A-Za-z0-9_]+\\b");
	QStringList added_words;
	foreach (QString word, list) {
		int idx = word_pattern.indexIn(word);
		if (word.isEmpty()) continue;
		while (idx >= 0) {
			int length = word_pattern.matchedLength();
			QString w = word.mid(idx, length).trimmed();
			if (!added_words.contains(w) && length > 3) {
				//qDebug() << w;
				words << w;
				added_words << w;
			}
			idx = word_pattern.indexIn(word, idx + length);
		}
	}
	c->setModel(new QStringListModel(words, c));
	cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
	command_count = 0;
	command[0] = 0;
	command[1] = 0;
	command[2] = 0;
}
