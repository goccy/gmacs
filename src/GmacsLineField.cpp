#include <gmacs.hpp>

using namespace std;

GmacsLineField::GmacsLineField(QTextEdit *parent) : QTextEdit(parent)
{
	setStyleSheet("background-color:black;" "color:white;" "font-family: monaco");
	setMinimumSize(QSize(50, 30));
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFixedHeight(30);
	setCursorWidth(0);
	cursor = textCursor();
	setTextCursor(cursor);
	setObjectName("GmacsLineField");
	isFocus = false;
	isPressedCtrl = false;
	isPressedShift = false;
	isPressedAlt = false;
	isPressedCommand = false;
	white.setForeground(Qt::white);
	c = new GmacsCompleter(this);
	QFileSystemModel *model = new QFileSystemModel(c);
	model->setRootPath(QDir::currentPath());
	c->setModel(model);
	kb = new GmacsKeyBind();
	const int blinkPeriod = 500;
	startTimer(blinkPeriod);
}

void GmacsLineField::insertCompletion(const QString& completion)
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

void GmacsLineField::keyPressEvent(QKeyEvent *event)
{
	//cout << "=========== key press event =======" << endl;
	//cout << event->key() << endl;
	//cout << event->modifiers() << endl;
	isKeyPress = true;
	if (event->modifiers() == Qt::META) {
		GmacsKeyBindFunc func = kb->getKeyBindFunction(event);
		if (func != NULL) {
			fprintf(stderr, "CTRL+key\n");
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
		break;
	case Qt::Key_Return: {
		QString filepath = readBuf();
		isFocus = false;
		QString text = cursor.block().text();
		cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
		for (int i = 0; i < text.size(); i++) {
			cursor.deleteChar();
		}
		emit focusToText();
		emit loadTextSignal(filepath);
		break;
	}
	default:
		kill_buf_count = 0;
		cursor.insertText(event->text(), white);
		setTextCursor(cursor);
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
}

QString GmacsLineField::textUnderCursor(void) const
{
	QTextCursor tc = textCursor();
	tc.select(QTextCursor::WordUnderCursor);
	return tc.selectedText();
}

void GmacsLineField::focusInEvent(QFocusEvent *e)
{
	if (c) c->setWidget(this);
	QTextEdit::focusInEvent(e);
}

void GmacsLineField::paintEvent(QPaintEvent *event)
{
	if (isFocus && (isKeyPress || isCurVisible)) {
		drawCursor();
	}
	setCursorWidth(0);
	QTextEdit::paintEvent(event);
	setCursorWidth(10);
}

void GmacsLineField::findFileMode()
{
	QTextCharFormat colorFormat;
	colorFormat.setForeground(QBrush(QColor("#00ffff")));
	cursor.insertText("Find File: ", colorFormat);
}

QString GmacsLineField::readBuf()
{
	QTextBlock block = cursor.block();
	QString buf = block.text();
	cout << qPrintable(buf) << endl;
	QStringList bufs = buf.split(":");
	if (bufs.size() != 0) {
		buf = bufs[1];
	}
	buf = buf.trimmed();
	return buf;
}

void GmacsLineField::mousePressEvent(QMouseEvent *event)
{
	isFocus = true;
	(void)event;
}

void GmacsLineField::setModifier(QKeyEvent *event)
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

void GmacsLineField::timerEvent(QTimerEvent *event)
{
	(void)event;
	if (isCurVisible) {
		isCurVisible = false;
	} else {
		isCurVisible = true;
	}
	isKeyPress = false;
}

void GmacsLineField::drawCursor()
{
	QRect r = cursorRect();
	r.setWidth(10);
	QPainter painter(viewport());
	painter.setOpacity(0.6);
	painter.fillRect(r, Qt::white);
}

void GmacsLineField::resetModifier()
{
	isPressedCtrl = false;
	isPressedShift = false;
	isPressedAlt = false;
	isPressedCommand = false;
}

void GmacsLineField::grabFocus()
{
	isFocus = true;
	findFileMode();
	setFocus(Qt::OtherFocusReason);
}

GmacsHighlightThread::GmacsHighlightThread(QThread *parent) : QThread(parent)
{

}

void GmacsHighlightThread::run()
{
	//gtf->sh->highlightAll(&gtf->cursor);
}
