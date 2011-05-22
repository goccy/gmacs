#include <iostream>
#include <QtGui>

class GmacsMainWindow : public QMainWindow {
public:
    GmacsMainWindow(QMainWindow *parent = 0);
};

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
class GmacsScriptLoader {
public:
	QString text_mode;
	QStringList preserved_keyword_list;
	QColor *preserved_keyword_color;
	QStringList type_keyword_list;
	QColor *type_keyword_color;
	GmacsScriptLoader(void);
	QString loadScript(QString);
};

class GmacsLineField;
class GmacsTextField : public QTextEdit {
	friend class GmacsLineField;
private:
	bool isPressedCtrl;
	bool isPressedShift;
	bool isPressedAlt;
	bool isPressedCommand;
	bool isCurVisible;
	bool isKeyPress;
	QTextCursor cursor;
	QString yank_buf;
	int kill_buf_count;
	GmacsSyntaxHighlighter *sh;
	int command[3];
	int command_count;
	QTextCharFormat white;
public:
	GmacsLineField *glf;
	bool isFocus;
	GmacsTextField(QTextEdit *parent = 0);
	void paintEvent(QPaintEvent *event);
	void drawCursor();
	virtual void keyPressEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
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
	void pressX();
	void pressY();
	void addYankBuffer(QString buf);
	void timerEvent(QTimerEvent *event);
};

class GmacsLineField : public GmacsTextField {
private:
	GmacsScriptLoader *script_loader;
public:
	GmacsTextField *gtf;
	GmacsLineField(GmacsTextField *parent = 0);
	void keyPressEvent(QKeyEvent *event);
	void paintEvent(QPaintEvent *event);
	void findFileMode();
	QString readBuf();
};
