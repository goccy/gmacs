#include <iostream>
#include <QtGui>
#include <gmacs_class.hpp>

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

class Gmacs {
public:
	QApplication *app;
	Gmacs(int argc, char **argv);
	void start();
};

class GmacsMainWindow : public QMainWindow {
	Q_OBJECT;
public:
    GmacsMainWindow(QMainWindow *parent = 0);
	void setMainField(GmacsMainField *main);
};

class GmacsMainField : public QWidget {
	Q_OBJECT;
	QVBoxLayout *layout;
public:
	GmacsTextField *text;
	GmacsStatusBar *bar;
	GmacsLineField *line;

	GmacsMainField(QWidget *parent = 0);
	void addWidget(QWidget *widget);
	void setConnect(void);
};

class GmacsStatusBar : public QLabel {
public:
	GmacsStatusBar(QLabel *parent = 0);
};


class GmacsCompletion : public QListWidget {
public:
	QStringList completion_list;
	GmacsMainWindow *main_window;
	GmacsCompletion(QListWidget *parent = 0);
	void setCompletionList(QStringList list);
	bool existsWord(QString word);
	void addList(QString str);
	void dumpList(void);
	void open(QTextCursor *cursor, QRect *rect);
	void close(void);
	void setPosition(int x, int y);
	void keyPressEvent(QKeyEvent *event);
};

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

typedef void (GmacsKeyBind::*GmacsKeyBindFunc)(QTextCursor *);
class GmacsKeyBind : public QObject {
	Q_OBJECT;
private:
	void (GmacsKeyBind::*pressCTRLA)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLB)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLC)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLD)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLE)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLF)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLG)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLH)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLI)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLJ)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLK)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLL)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLM)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLN)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLO)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLP)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLQ)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLR)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLS)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLT)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLU)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLV)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLW)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLX)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLY)(QTextCursor *cursor);
	void (GmacsKeyBind::*pressCTRLZ)(QTextCursor *cursor);
	QString yank_buf;
	int kill_buf_count;
	int command_count;
	GmacsKeyBindFunc *key_bind_list;
	GmacsKeyBindFunc *command_bind_list;

public:
	GmacsKeyBind(void);
	void bindKeys();
	void bindCommands();
	GmacsKeyBindFunc getKeyBindFunction(QKeyEvent *event);
	void moveLineTop(QTextCursor *cursor);
	void moveLineEnd(QTextCursor *cursor);
	void moveLeft(QTextCursor *cursor);
	void moveRight(QTextCursor *cursor);
	void moveUp(QTextCursor *cursor);
	void moveDown(QTextCursor *cursor);
	void yankText(QTextCursor *cursor);
	void addYankBuffer(QString buf);
	void killText(QTextCursor *cursor);
	void deleteChar(QTextCursor *cursor);
	void startCommand(QTextCursor *cursor);
	void findFile(QTextCursor *cursor);
signals:
	void emitFindFileSignal(void);
};

class GmacsTextField : public QTextEdit {
	Q_OBJECT;
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
	GmacsScriptLoader *script_loader;

public:
	GmacsCompletion *comp;
	GmacsKeyBind *kb;
	bool isFindFileMode;
	bool isFocus;
	GmacsTextField(QTextEdit *parent = 0);
	void paintEvent(QPaintEvent *event);
	void drawCursor();
	void keyPressEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void setModifier(QKeyEvent *event);
	void resetModifier(void);
	void timerEvent(QTimerEvent *event);
signals:
	void focusToLine(void);
public slots:
	void grabFocus(void);
	void findFile(void);
	void loadText(QString filepath);
};

class GmacsLineField : public QTextEdit {
	Q_OBJECT;
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
	GmacsKeyBind *kb;
	bool isFindFileMode;
	bool isFocus;

	GmacsLineField(QTextEdit *parent = 0);
	void keyPressEvent(QKeyEvent *event);
	void paintEvent(QPaintEvent *event);
	void drawCursor();
	void setModifier(QKeyEvent *event);
	void resetModifier(void);
	void timerEvent(QTimerEvent *event);
	void findFileMode();
	QString readBuf();
	void mousePressEvent(QMouseEvent *event);
signals:
	void focusToText(void);
	void loadTextSignal(QString filepath);
public slots:
	void grabFocus(void);
};

class GmacsHighlightThread : public QThread {
public:
	GmacsTextField *gtf;
	GmacsHighlightThread(QThread *parent = 0);
	void run(void);
};
