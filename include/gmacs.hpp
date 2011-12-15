#include <iostream>
#include <QtGui>
#include <gmacs_class.hpp>
#define QSTRING_TO_CHAR(str) str.toLocal8Bit().data()

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

class Gmacs : public QApplication {
public:
	Gmacs(int argc, char **argv);
	void start(void);
};

#define K_INTERNAL
#include <konoha1.h>
class GmacsInteractiveDesigner {
public:
	kcontext_t *ctx;
	QString prev_script;
	QStringList global_decls;

	GmacsInteractiveDesigner(void);
	void eval(const QString &script);
	void traverseFieldObject(void);
	QString removeNoneModifiedScript(const QString &script);
	QString removeGlobalScopeConstructor(const QString &script);
	~GmacsInteractiveDesigner(void);
};

class GmacsMainWindow : public QWidget {
	Q_OBJECT;
public:
	GmacsTabWidget *tab;
	GmacsWidget *widget;
    GmacsMainWindow(QWidget *parent = 0);
	void addTab(void);
	void keyPressEvent(QKeyEvent *event);
};

class GmacsWidget : public QWidget {
	Q_OBJECT;
public:
	GmacsMainField *main_field;
	GmacsLineField *command_area;
	GmacsWidget(QWidget *parent);
};

class GmacsTabWidget : public QTabWidget {
public:
	GmacsTabWidget(QWidget *parent);
};

class GmacsMainField : public QWidget {
	Q_OBJECT;
	QVBoxLayout *layout;
public:
	GmacsTextField *edit_area;
	GmacsStatusBar *status_bar;

	GmacsMainField(QWidget *parent = 0);
};

class GmacsLineNumberArea : public QWidget {
public:
	GmacsTextField *edit_area;
	GmacsLineNumberArea(GmacsTextField *parent);
	QSize sizeHint(void) const;
	void paintEvent(QPaintEvent *event);
};

class GmacsStatusBar : public QLabel {
public:
	GmacsStatusBar(QLabel *parent = 0);
};

class GmacsCompleter : public QCompleter {
public:
	GmacsCompleter(QPlainTextEdit *editor);
};

typedef struct GmacsTypeObject {
	QString type_name;
	QStringList fields;
	QStringList private_method_list;
	QStringList public_method_list;
} GmacsTypeObject;

#include <clang-c/Index.h>

class GmacsPreprocessor {
private:
	QString document;
	QList<GmacsTypeObject> typeobjs;
	QList<char *> *include_dirs;
	int include_dirs_size;
	CXIndex idx;
	CXTranslationUnit unit;
	CXUnsavedFile *unsaved_file;
	QString *filename;
public:
	QStringList added_words;

	GmacsPreprocessor(void);
	~GmacsPreprocessor(void);
	void compile(const QString &filename, const QTextCursor &cursor);
	void codeCompletion(const QTextCursor &cursor);
	void setMode(const QString &mode);
	void setDocument(const QString &document);
	void start(void);
	void analyze(const QStringList &list, const QRegExp &expression);
	QStringList getTypeObjectLines(const QStringList &list, int start_idx);
	void traverseTypeObject(const QStringList &list, int start_idx);
};

/*
class GmacsCompleter : public QListWidget {
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
*/

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

/*
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
*/

struct HighlightingRule	{
	QRegExp pattern;
	QTextCharFormat format;
};

class GmacsSyntaxHighlighter : public QSyntaxHighlighter {
	Q_OBJECT;
private:
	QVector<HighlightingRule> highlightingRules;
	QRegExp commentStartExpression;
	QRegExp commentEndExpression;
	QTextCharFormat keywordFormat;
	QTextCharFormat typeFormat;
	QTextCharFormat valueFormat;
	QTextCharFormat classFormat;
	QTextCharFormat operatorFormat;
	QTextCharFormat tabFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat functionFormat;
public:
	GmacsSyntaxHighlighter(QTextDocument *parent = 0);
	void addTypes(const QStringList &types);
protected:
	void highlightBlock(const QString &text);
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
	void clearCommand(QTextCursor *cursor);
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

class GmacsTextField : public QPlainTextEdit {//public QTextEdit {
	Q_OBJECT;
	friend class GmacsLineField;
	friend class GmacsHighlightThread;
private:
	bool isHighlightAll;
	bool isCurVisible;
	bool isKeyPress;
	bool isOpenCompletionWindow;
	QTextCursor cursor;
	QString yank_buf;
	int kill_buf_count;
	GmacsSyntaxHighlighter *sh;
	GmacsCompleter *c;
	GmacsLineNumberArea *line_number_area;
	int command[3];
	int command_count;
	QTextCharFormat white;
	GmacsScriptLoader *script_loader;
	GmacsInteractiveDesigner *designer;
	GmacsPreprocessor *gpp;
public:
	GmacsKeyBind *kb;
	bool isFindFileMode;
	bool isFocus;
	GmacsTextField(QPlainTextEdit *parent = 0);
	bool isInstance(const QString &prefix);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth(void);
	void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
	void drawCursor();
	void keyPressEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void timerEvent(QTimerEvent *event);
	void focusInEvent(QFocusEvent *event);
	QString textUnderCursor(void) const;
signals:
	void focusToLine(void);
public slots:
	void grabFocus(void);
	void findFile(void);
	void loadText(QString filepath);
	void insertCompletion(const QString &completion);
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);
};

class GmacsLineField : public QPlainTextEdit {
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
	GmacsCompleter *c;
	bool isFindFileMode;
	bool isFocus;

	GmacsLineField(QPlainTextEdit *parent = 0);
	void keyPressEvent(QKeyEvent *event);
	void focusInEvent(QFocusEvent *event);
	QString textUnderCursor(void) const;
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
	void insertCompletion(const QString &completion);
};

class GmacsHighlightThread : public QThread {
public:
	GmacsTextField *gtf;
	GmacsHighlightThread(QThread *parent = 0);
	void run(void);
};
