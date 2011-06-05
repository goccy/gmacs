#include <iostream>
#include <QtGui>

class GmacsTextField;
class GmacsCompletion : public QListWidget {
public:
	QStringList completion_list;
	GmacsTextField *gtf;
	GmacsCompletion(QListWidget *parent = 0);
	void setCompletionList(QStringList list);
	bool existsWord(QString word);
	void addList(QString str);
	void dumpList(void);
	void open(QTextCursor *cursor);
	void close(void);
	void setPosition(int x, int y);
	virtual void keyPressEvent(QKeyEvent *event);
};

class GmacsMainWindow : public QMainWindow {
public:
	GmacsCompletion *gc;
    GmacsMainWindow(QMainWindow *parent = 0);
};

typedef enum {
	DEF_T,
	SPACE_T,
	TYPE_T,
	VAR_T,
	FUNC_T,
	INSTANCE_T,
	PRESERVE_T,
	STRING_T,
	CONST_T,
	CPP_T,
	COMMENT_T,
	ARROW_T,
} GmacsWordType;

class GmacsToken {
public:
	QString word;
	GmacsWordType type;
	int count;
	GmacsToken(QString word, GmacsWordType type, int count);
};

#define MAX_TOKEN_SIZE 128

class GmacsTokenizer {
public:
	GmacsTokenizer(void);
	GmacsToken **split(QString text);
	void dump(GmacsToken **token);
};

class GmacsParser {
private:
	QString mode;
	QStringList preserved_keyword_list;
	QStringList type_keyword_list;
	bool isClassType;
public:
	GmacsParser(QString mode, QStringList plist, QStringList tlist);
	void parse(GmacsToken **token);
	bool searchFromType(GmacsToken *t);
	bool searchFromPreserve(GmacsToken *t);
	int getFuncNameIdx(GmacsToken **token);
	void addType(QString type_name);
};

class GmacsSyntaxHighlighter {
private:
	GmacsTokenizer *gt;
	GmacsParser *gp;
	QMutex mutex;
	QString text_mode;
	QStringList preserved_keyword_list;
	QStringList type_keyword_list;
	QColor *preserved_keyword_color;
	QColor *type_keyword_color;
	QStringList completion_list;
public:
	GmacsSyntaxHighlighter();
	void initParser(void);
	void highlight(QTextCursor *cursor, int width);
	void highlightLine(QString line, QTextCursor *cursor);
	void highlightInteractiveLine(QString line, QTextCursor *cursor);
	void highlightAll(QTextCursor *cursor);
	void addPreservedKeywordList(QStringList list);
	void setPreservedKeywordColor(QColor *color);
	void addTypeKeywordList(QStringList list);
	void setTypeKeywordColor(QColor *color);
	bool highlightKeyword(QTextCursor *cursor, QString word, QStringList list, QColor *color);
	void highlightPreserve(QTextCursor *cursor, GmacsToken *token);
	void highlightType(QTextCursor *cursor, GmacsToken *token);
	void highlightVariable(QTextCursor *cursor, GmacsToken *token);
	void highlightFunction(QTextCursor *cursor, GmacsToken *token);
	void highlightInstance(QTextCursor *cursor, GmacsToken *token);
	void highlightString(QTextCursor *cursor, GmacsToken *token);
	void highlightComment(QTextCursor *cursor, GmacsToken *token);
	void highlightDefault(QTextCursor *cursor, GmacsToken *token);
	void addCompletionList(GmacsToken **token);
	QStringList getCompletionList(void);
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
	friend class GmacsHighlightThread;
private:
	bool isHighlightAll;
	bool isPressedCtrl;
	bool isPressedShift;
	bool isPressedAlt;
	bool isPressedCommand;
	bool isCurVisible;
	bool isKeyPress;
	bool isOpenCompletionWindow;
	QTextCursor cursor;
	QString yank_buf;
	int kill_buf_count;
	GmacsSyntaxHighlighter *sh;
	int command[3];
	int command_count;
	QTextCharFormat white;

public:
	GmacsMainWindow *main_window;
	GmacsCompletion *gc;
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
	void mousePressEvent(QMouseEvent *event);
};

class GmacsHighlightThread : public QThread {
public:
	GmacsTextField *gtf;
	GmacsHighlightThread(QThread *parent = 0);
	void run(void);
};
