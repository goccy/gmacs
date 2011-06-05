#include <gmacs.hpp>

using namespace std;

GmacsLineField::GmacsLineField(GmacsTextField *parent) : GmacsTextField(parent)
{
	setStyleSheet("background-color:black;" "color:white;" "font-family: monaco");
	setMinimumSize(QSize(50, 30));
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFixedHeight(30);
	setObjectName("GmacsLineField");
	isFocus = false;
	isPressedCtrl = false;
	isPressedShift = false;
	isPressedAlt = false;
	isPressedCommand = false;
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
		gtf->sh->highlightAll(&gtf->cursor);
		QStringList list = gtf->sh->getCompletionList();
		gtf->gc->setCompletionList(list);
		gtf->gc->dumpList();
		//GmacsHighlightThread *th = new GmacsHighlightThread();
		//th->gtf = gtf;
		//th->start();
		gtf->cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
		gtf->command_count = 0;
		gtf->command[0] = 0;
		gtf->command[1] = 0;
		gtf->command[2] = 0;
		QString text = cursor.block().text();
		cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
		for (int i = 0; i < text.size(); i++) {
			cursor.deleteChar();
		}
		isFocus = false;
		releaseKeyboard();
		gtf->isFocus = true;
		gtf->grabKeyboard();
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
	grabKeyboard();
	gtf->isFocus = false;
	gtf->releaseKeyboard();
}

GmacsHighlightThread::GmacsHighlightThread(QThread *parent) : QThread(parent)
{
	
}

void GmacsHighlightThread::run()
{
	gtf->sh->highlightAll(&gtf->cursor);
}
