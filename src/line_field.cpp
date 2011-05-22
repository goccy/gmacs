#include <gmacs.hpp>

using namespace std;
GmacsLineField::GmacsLineField(GmacsTextField *parent) : GmacsTextField(parent)
{
	setStyleSheet("background-color:black;" "color:white");
	setMinimumSize(QSize(50, 30));
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFixedHeight(30);
	setObjectName("GmacsLineField");
	isFocus = false;
	white.setForeground(Qt::white);
	script_loader = new GmacsScriptLoader();
}

void GmacsLineField::keyPressEvent(QKeyEvent *event)
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
	case Qt::Key_Return: {
		QString filepath = readBuf();
		cout << qPrintable(filepath) << endl;
		QString buf = script_loader->loadScript(filepath);
		cout << qPrintable(buf);
		QTextDocument *document = new QTextDocument();
		document->setPlainText(buf);
		gtf->setDocument(document);
		QTextCursor cur = gtf->textCursor();
		gtf->cursor = cur;
		gtf->isFocus = true;
		gtf->grabKeyboard();
		isFocus = false;
		releaseKeyboard();
		break;
	}
	default:
		kill_buf_count = 0;
		cursor.insertText(event->text(), white);
		setTextCursor(cursor);
		break;
	}
	resetModifier();
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
	cursor.insertText("Find File:", colorFormat);
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
	return buf;
}
