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
	//if (isOpenCompletionWindow) {
		//qDebug() << "hello";
		//cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
	//}
	//cout << "=========== key press event =======" << endl;
	//cout << event->key() << endl;
	//cout << event->modifiers() << endl;
	isKeyPress = true;
	setModifier(event);
	switch (event->key()) {
	case Qt::Key_A:
		pressA();
		break;
	case Qt::Key_E:
		pressE();
		break;
	case Qt::Key_F:
		pressF();
		break;
	case Qt::Key_B:
		pressB();
		break;
	case Qt::Key_N:
		pressN();
		break;
	case Qt::Key_P:
		pressP();
		break;
	case Qt::Key_D:
		pressD();
		break;
	case Qt::Key_K:
		pressK();
		break;
	case Qt::Key_X:
		pressX();
		break;
	case Qt::Key_Y:
		pressY();
		break;
	case Qt::Key_Up:
		if (isOpenCompletionWindow) {
			releaseKeyboard();
			gc->grabKeyboard();
		}
		break;
	case Qt::Key_Down:
		if (isOpenCompletionWindow) {
			releaseKeyboard();
			gc->grabKeyboard();
			int r = gc->currentRow();
			gc->setCurrentRow(r+1);
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
		gc->close();
		isOpenCompletionWindow = false;
		kill_buf_count = 0;
		cursor.insertText(event->text());
		setTextCursor(cursor);
		command_count = 0;
		qDebug() << "main_window_pos = " << main_window->pos();
		if (gc->completion_list.size() != 0) {
			QRect rect = cursorRect(cursor);
			QPoint pos = main_window->pos();
			gc->setPosition(pos.x() + rect.left(), pos.y() + rect.bottom() + 30);
			gc->open(&cursor);
			isOpenCompletionWindow = true;
		}
		break;
	}
	resetModifier();
	//setOverwriteMode(true);
	sh->highlight(&cursor, width());
	setTextCursor(cursor);
	//setOverwriteMode(false);
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

void GmacsTextField::pressA()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-A" << endl;
		cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	} else if (isPressedShift) {
		cursor.insertText("A", white);
	} else {
		cursor.insertText("a", white);
	}
	setTextCursor(cursor);
	command_count = 0;
}

void GmacsTextField::pressB()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-B" << endl;
		cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
	} else if (isPressedShift) {
		cursor.insertText("B", white);
	} else {
		cursor.insertText("b", white);
	}
	setTextCursor(cursor);
	command_count = 0;
}

void GmacsTextField::pressE()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-E" << endl;
		cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
	} else if (isPressedShift) {
		cursor.insertText("E", white);
	} else {
		cursor.insertText("e", white);
	}
	setTextCursor(cursor);
	command_count = 0;
}

void GmacsTextField::pressF()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-F" << endl;
		cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
		if (command[0] == Qt::Key_X) {
			command[command_count] = Qt::Key_F;
			cout << "C-X C-F" << endl;
			isFocus = false;
			releaseKeyboard();
			glf->isFocus = true;
			glf->grabKeyboard();
			glf->findFileMode();
		}
		command_count = 0;
	} else if (isPressedShift) {
		cursor.insertText("F", white);
	}  else {
		cursor.insertText("f", white);
	}
	setTextCursor(cursor);
}

void GmacsTextField::pressX()
{
	if (isPressedCtrl) {
		cout << "Ctrl-X" << endl;
		if (command_count == 0) {
			command[0] = Qt::Key_X;
			command_count++;
		}
	} else if (isPressedShift) {
		cursor.insertText("X", white);
	}  else {
		cursor.insertText("x", white);
	}
	setTextCursor(cursor);
}

void GmacsTextField::pressN()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-N" << endl;
		cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
	} else if (isPressedShift) {
		cursor.insertText("N", white);
	}  else {
		cursor.insertText("n", white);
	}
	setTextCursor(cursor);
	command_count = 0;
}

void GmacsTextField::pressP()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-P" << endl;
		cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor);
	} else if (isPressedShift) {
		cursor.insertText("P", white);
	}  else {
		cursor.insertText("p", white);
	}
	setTextCursor(cursor);
	command_count = 0;
}

void GmacsTextField::pressD()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-D" << endl;
		cursor.deleteChar();
	} else if (isPressedShift) {
		cursor.insertText("D", white);
	}  else {
		cursor.insertText("d", white);
	}
	setTextCursor(cursor);
	command_count = 0;
}

void GmacsTextField::pressK()
{
	if (isPressedCtrl) {
		cout << "Ctrl-K" << endl;
		if (cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor)) {
			QString text = cursor.selectedText();
			addYankBuffer(text);
			kill_buf_count++;
			cout << qPrintable(text) << endl;
			cursor.removeSelectedText();
		} else {
			//line end
			kill_buf_count++;
			cursor.deleteChar();
		}
	} else if (isPressedShift) {
		kill_buf_count = 0;
		cursor.insertText("K", white);
	}  else {
		kill_buf_count = 0;
		cursor.insertText("k", white);
	}
	setTextCursor(cursor);
	command_count = 0;
}

void GmacsTextField::addYankBuffer(QString buf)
{
	if (kill_buf_count == 0) {
		yank_buf = buf;
	} else {
		yank_buf += "\n" + buf;
	}
}

void GmacsTextField::pressY()
{
	if (isPressedCtrl) {
		cout << "Ctrl-Y" << endl;
		QTextCharFormat colorFormat = cursor.charFormat();
		//colorFormat.setForeground(Qt::red);
		colorFormat.setForeground(Qt::white);
		//cursor.mergeCharFormat(colorFormat);
		cursor.insertText(yank_buf, colorFormat);
		QTextCharFormat defaultFormat;
		defaultFormat.setForeground(Qt::white);
		cursor.mergeCharFormat(defaultFormat);
	} else if (isPressedShift) {
		cursor.insertText("Y", white);
	}  else {
		cursor.insertText("y", white);
	}
	setTextCursor(cursor);
	command_count = 0;
}

void GmacsTextField::mousePressEvent(QMouseEvent *event)
{
	isFocus = true;
	grabKeyboard();
	glf->isFocus = false;
	glf->releaseKeyboard();
}
