#include <gmacs.hpp>

using namespace std;
GmacsTextField::GmacsTextField(QTextEdit *parent) : QTextEdit(parent)
{
	setStyleSheet("background-color:black;" "color:white;" "font-family: monaco");
	setLineWrapMode(QTextEdit::NoWrap);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setCursorWidth(0);
	cursor = textCursor();
	setTabStopWidth(4);
	setObjectName("GmacsTextField");
	setTextCursor(cursor);
	isFocus = true;
	isHighlightAll = false;
	isOpenCompletionWindow = false;
	isFindFileMode = false;
	const int blinkPeriod = 500;
	startTimer(blinkPeriod);
	sh = new GmacsSyntaxHighlighter();
	GmacsScriptLoader *sl = new GmacsScriptLoader();
	white.setForeground(Qt::white);
	sh->addPreservedKeywordList(sl->preserved_keyword_list);
	sh->setPreservedKeywordColor(sl->preserved_keyword_color);
	sh->addTypeKeywordList(sl->type_keyword_list);
	sh->setTypeKeywordColor(sl->type_keyword_color);
	sh->initParser();
	comp = new GmacsCompletion();
	kb = new GmacsKeyBind();
	script_loader = new GmacsScriptLoader();
	connect(kb, SIGNAL(emitFindFileSignal()), this, SLOT(findFile()));
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
	setModifier(event);
	if (isPressedCtrl) {
		GmacsKeyBindFunc func = kb->getKeyBindFunction(event);
		if (func != NULL) {
			(kb->*func)(&cursor);
			setTextCursor(cursor);
		}
		kill_buf_count = 0;
		command_count = 0;
	}
	switch (event->key()) {
	case Qt::Key_Up:
		if (isOpenCompletionWindow) {
			releaseKeyboard();
			comp->grabKeyboard();
		}
		break;
	case Qt::Key_Down:
		if (isOpenCompletionWindow) {
			releaseKeyboard();
			comp->grabKeyboard();
			int r = comp->currentRow();
			comp->setCurrentRow(r+1);
		}
		break;
	case Qt::Key_Backspace:
		kill_buf_count = 0;
		if (cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor)) {
			cursor.deleteChar();
		}
		command_count = 0;
		break;
	case Qt::Key_Return:
		cursor.insertText("\n");
		command_count = 0;
		break;
	default:
		comp->close();
		isOpenCompletionWindow = false;
		kill_buf_count = 0;
		cursor.insertText(event->text());
		setTextCursor(cursor);
		command_count = 0;
		//qDebug() << "main_window_pos = " << main_window->pos();
		if (comp->completion_list.size() != 0) {
			//QRect rect = cursorRect(cursor);
			//QPoint pos = main_window->pos();
			//gc->setPosition(pos.x() + rect.left(), pos.y() + rect.bottom() + 30);
			comp->open(&cursor, &cursorRect(cursor));
			isOpenCompletionWindow = true;
		}
		break;
	}
	resetModifier();
	sh->highlight(&cursor, width());
	setTextCursor(cursor);
}

void GmacsTextField::setModifier(QKeyEvent *event)
{
	switch (event->modifiers()) {
	case Qt::CTRL:
		//OSX is command
		isPressedCommand = true;
		break;
	case Qt::SHIFT:
		isPressedShift = true;
		break;
	case Qt::ALT:
		isPressedAlt = true;
		break;
	case Qt::META:
		//OSX is ctrl
		isPressedCtrl = true;
		break;
	default:
		break;
	}
}

void GmacsTextField::resetModifier()
{
	isPressedCtrl = false;
	isPressedShift = false;
	isPressedAlt = false;
	isPressedCommand = false;
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
	cout << qPrintable(filepath) << endl;
	QString buf = script_loader->loadScript(filepath);
	cout << qPrintable(buf);
	QTextDocument *document = new QTextDocument();
	document->setPlainText(buf);
	setDocument(document);
	QTextCursor cur = textCursor();
	cursor = cur;
	sh->highlightAll(&cursor);
	QStringList list = sh->getCompletionList();
	comp->setCompletionList(list);
	comp->dumpList();
	//GmacsHighlightThread *th = new GmacsHighlightThread();
	//th->gtf = gtf;
	//th->start();
	cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
	command_count = 0;
	command[0] = 0;
	command[1] = 0;
	command[2] = 0;
}
