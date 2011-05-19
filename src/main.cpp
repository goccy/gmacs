#include <iostream>
#include <QtGui>

using namespace std;

class GmacsMainWindow : public QMainWindow {
public:
    GmacsMainWindow(QMainWindow *parent = 0);
};

GmacsMainWindow::GmacsMainWindow(QMainWindow *parent) : QMainWindow(parent)
{

}

class GmacsSyntaxHighlighter {
private:
	QString text_mode;
	QStringList preserved_keyword_list;
	QColor *preserved_keyword_color;
	QStringList type_keyword_list;
	QColor *type_keyword_color;
public:
	GmacsSyntaxHighlighter();
	void highlight(QTextCursor *cursor, int width);
	void addPreservedKeywordList(QStringList list);
	void setPreservedKeywordColor(QColor *color);
	void addTypeKeywordList(QStringList list);
	void setTypeKeywordColor(QColor *color);
};

GmacsSyntaxHighlighter::GmacsSyntaxHighlighter()
{
	
}

void GmacsSyntaxHighlighter::addPreservedKeywordList(QStringList list)
{
	preserved_keyword_list += list;
}

void GmacsSyntaxHighlighter::setPreservedKeywordColor(QColor *color)
{
	preserved_keyword_color = color;
}

void GmacsSyntaxHighlighter::addTypeKeywordList(QStringList list)
{
	type_keyword_list += list;
}

void GmacsSyntaxHighlighter::setTypeKeywordColor(QColor *color)
{
	type_keyword_color = color;
}

void GmacsSyntaxHighlighter::highlight(QTextCursor *cursor, int width)
{
	QTextCharFormat defaultFormat;
	defaultFormat.setForeground(Qt::white);
	QTextBlock block = cursor->block();
	QString text = block.text();
	cout << qPrintable(text) << endl;
	QStringList words = text.split(" ");
	int offset = 0;
	int cur_pos = cursor->position();
	cout << cur_pos << endl;
	cout << block.length() << endl;
	cout << width << endl;
	//int line_count = width - block.length();
	//cout << block.lineCount() << endl;
	//cout << block.firstLineNumber() << endl;
	//cout << block.blockNumber() << endl;
	cursor->movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	for (int i = 0; i < text.size(); i++) {
		cursor->deleteChar();
	}
	QChar space = ' ';
	for (int i = 0, j = 0; i < text.size(); i++) {
		if (text[i] == space) {
			cursor->insertText(" ", defaultFormat);
		} else {
			for (int k = 0; k < preserved_keyword_list.size(); k++) {
				if (words[j] == preserved_keyword_list[k]) {
					QTextCharFormat colorFormat;
					colorFormat.setForeground(QBrush(*preserved_keyword_color));
					cursor->insertText(words[j], colorFormat);
					goto L_NEXT;
				}
			}
			cursor->insertText(words[j], defaultFormat);
		L_NEXT:;
			i += words[j].size() - 1;
			j++;
		}
	}
	cursor->movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	cur_pos = cur_pos - cursor->position();
	cursor->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, cur_pos);
	cursor->mergeCharFormat(defaultFormat);
}

class GmacsScriptLoader {
public:
	QString text_mode;
	QStringList preserved_keyword_list;
	QColor *preserved_keyword_color;
	QStringList type_keyword_list;
	QColor *type_keyword_color;
	GmacsScriptLoader(void);
};

GmacsScriptLoader::GmacsScriptLoader()
{
	//load gmacs.conf
	text_mode = "konoha";
	const char *a[16] = {"while", "for", "if", "else", "switch", "new", "class", "using", "foreach", "print", NULL};
	for (int i = 0; a[i] != NULL; i++) {
		preserved_keyword_list << a[i];
	}
	preserved_keyword_color = new QColor("#00ffff");
	const char *b[] = {"int", "float", "boolean", "String", "Map", "Array", "dyn", "Func", NULL};
	for (int i = 0; b[i] != NULL; i++) {
		type_keyword_list << b[i];
	}
	type_keyword_color = new QColor("#ff00ff");
}

class GmacsTextField : public QTextEdit {
private:
	bool isPressedCtrl;
	bool isPressedShift;
	bool isPressedAlt;
	bool isPressedCommand;
	QTextCursor cursor;
	QString yank_buf;
	int kill_buf_count;
	bool isCurVisible;
	bool isKeyPress;
	GmacsSyntaxHighlighter *sh;
public:
	GmacsTextField(QTextEdit *parent = 0);
	void paintEvent(QPaintEvent *event);
	void drawCursor();
	void keyPressEvent(QKeyEvent *event);
	void setModifier(QKeyEvent *event);
	void resetModifier(void);
	void pressA();
	void pressE();
	void pressF();
	void pressB();
	void pressN();
	void pressP();
	void pressD();
	void pressK();
	void pressY();
	void addYankBuffer(QString buf);
	void timerEvent(QTimerEvent *event);
};

GmacsTextField::GmacsTextField(QTextEdit *parent) : QTextEdit(parent)
{
	setTextColor(QColor("white"));
	setStyleSheet("background-color:black");
	setCursorWidth(0);
	cursor = textCursor();
	//QTextCharFormat plainFormat = cursor.charFormat();
    //QTextCharFormat colorFormat = plainFormat;
    //colorFormat.setForeground(Qt::red);
	//cursor.mergeCharFormat(colorFormat);
	//cursor->setStyleSheet("background-color:white");
	setTextCursor(cursor);
	const int blinkPeriod = 500;
	startTimer(blinkPeriod);
	sh = new GmacsSyntaxHighlighter();
	GmacsScriptLoader *sl = new GmacsScriptLoader();
	sh->addPreservedKeywordList(sl->preserved_keyword_list);
	sh->setPreservedKeywordColor(sl->preserved_keyword_color);
	sh->addTypeKeywordList(sl->type_keyword_list);
	sh->setTypeKeywordColor(sl->type_keyword_color);
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
	if (isKeyPress || isCurVisible) {
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
	case Qt::Key_Y:
		pressY();
		break;
	case Qt::Key_Backspace:
		kill_buf_count = 0;
		if (cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor)) {
			cursor.deleteChar();
		}
		break;
	case Qt::Key_Return:
		cursor.insertText("\n");
		break;
	default:
		kill_buf_count = 0;
		cursor.insertText(event->text());
		setTextCursor(cursor);
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

void GmacsTextField::pressA()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-A" << endl;
		cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	} else if (isPressedShift) {
		cursor.insertText("A");
	} else {
		cursor.insertText("a");
	}
	setTextCursor(cursor);
}

void GmacsTextField::pressB()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-B" << endl;
		cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
	} else if (isPressedShift) {
		cursor.insertText("B");
	} else {
		cursor.insertText("b");
	}
	setTextCursor(cursor);
}

void GmacsTextField::pressE()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-E" << endl;
		cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
	} else if (isPressedShift) {
		cursor.insertText("E");
	} else {
		cursor.insertText("e");
	}
	setTextCursor(cursor);
}

void GmacsTextField::pressF()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-F" << endl;
		cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
	} else if (isPressedShift) {
		cursor.insertText("F");
	}  else {
		cursor.insertText("f");
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
		cursor.insertText("N");
	}  else {
		cursor.insertText("n");
	}
	setTextCursor(cursor);
}

void GmacsTextField::pressP()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-P" << endl;
		cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor);
	} else if (isPressedShift) {
		cursor.insertText("P");
	}  else {
		cursor.insertText("p");
	}
	setTextCursor(cursor);
}

void GmacsTextField::pressD()
{
	kill_buf_count = 0;
	if (isPressedCtrl) {
		cout << "Ctrl-D" << endl;
		cursor.deleteChar();
	} else if (isPressedShift) {
		cursor.insertText("D");
	}  else {
		cursor.insertText("d");
	}
	setTextCursor(cursor);
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
		cursor.insertText("K");
	}  else {
		kill_buf_count = 0;
		cursor.insertText("k");
	}
	setTextCursor(cursor);
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
		cursor.insertText("Y");
	}  else {
		cursor.insertText("y");
	}
	setTextCursor(cursor);
}

int main(int argc, char **argv)
{
	QApplication *app = new QApplication(argc, argv);
	QFont font;
	font.setPointSize(18);
	font.setFixedPitch(true);
	app->setFont(font);
	GmacsMainWindow *window = new GmacsMainWindow();
	GmacsTextField *gtf = new GmacsTextField();
	//QPalette palette = gtf->palette();
	//palette.setColor(QPalette::WindowText, QColor("black"));
	window->resize(600, 600);
	window->setCentralWidget(gtf);
	window->setWindowOpacity(0.9);
	window->show();
	app->exec();
}
