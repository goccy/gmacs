#include <gmacs.hpp>

//GmacsSyntaxHighlighter::GmacsSyntaxHighlighter(QTextDocument *parent)
GmacsSyntaxHighlighter::GmacsSyntaxHighlighter(QTextEdit *parent)
	: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	const char *keywords[] = {
		"while", "for", "if", "else", "switch",
		"goto", "static", "OUT", "EOL", "assert",
		"assure", "namespace", "link", "format",
		"yield", "case", "default", "break", "do",
		"try", "catch", "throw", "function", "extends",
		"continue", "new", "class", "using", "include",
		"foreach", "print", "return", "const",
		"enum", "explicit", "friend", "inline", "operator",
		"private", "public", "protected", "signals", "slots",
		"struct", "template", "typedef", "typename", "union",
		"virtual", "volatile",
		NULL
	};
	keywordFormat.setForeground(QColor("#40e0d0"));
	QStringList keywordPatterns;
	for (int i = 0; keywords[i] != NULL; i++) {
		keywordPatterns << "\\b" + QString(keywords[i]) + "\\b";
	}

	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

	const char *types[] = {
		"char", "double", "float", "int", "long", "signed", "unsigned",
		"short", "void", "bool", "string", "Array", "Float", "boolean",
		"Boolean", "Map", "dynamic", "InputStream", "OutputStream",
		"Func", "Int",
		NULL
	};
	typeFormat.setForeground(QColor("#ffaadd"));
	QStringList typePatterns;
	for (int i = 0; types[i] != NULL; i++) {
		typePatterns << "\\b" + QString(types[i]) + "\\b";
	}

	QStringList valuePatterns;
	for (int i = 0; types[i] != NULL; i++) {
		valuePatterns << "\\b" + QString(types[i]) + "(\\s+[&*A-Za-z0-9_]+)\\b";
	}

	valueFormat.setForeground(QColor("#00ffff"));
	foreach (const QString &pattern, valuePatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = valueFormat;
		highlightingRules.append(rule);
	}

	foreach (const QString &pattern, typePatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = typeFormat;
		highlightingRules.append(rule);
	}

	const char *operators[] = {
		"&", "\\*", NULL
	};

	operatorFormat.setForeground(QColor("white"));
	QStringList operatorPatterns;
	for (int i = 0; operators[i] != NULL; i++) {
		operatorPatterns << QString(operators[i]);
	}

	foreach (const QString &pattern, operatorPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = operatorFormat;
		highlightingRules.append(rule);
	}
	//classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(QColor("#ff69b4"));//Qt::darkMagenta);
	rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);

	//functionFormat.setFontItalic(true);
	functionFormat.setForeground(QColor("#ff8c00"));
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);

	singleLineCommentFormat.setForeground(QColor("#808080"));
	rule.pattern = QRegExp("//[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(QColor("#808080"));

	quotationFormat.setForeground(QColor("#00ff7f"));
	rule.pattern = QRegExp("\"[^\"]*\"");
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");
}

void GmacsSyntaxHighlighter::highlightBlock(const QString &text)
{
	foreach (const HighlightingRule &rule, highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = commentStartExpression.indexIn(text);

	while (startIndex >= 0) {
		int endIndex = commentEndExpression.indexIn(text, startIndex);
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} else {
			commentLength = endIndex - startIndex
				+ commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
	}
}

/*
using namespace std;
GmacsSyntaxHighlighter::GmacsSyntaxHighlighter()
{
	gt = new GmacsTokenizer();
}

void GmacsSyntaxHighlighter::initParser(void)
{
	gp = new GmacsParser("konoha", preserved_keyword_list, type_keyword_list);
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

bool GmacsSyntaxHighlighter::highlightKeyword(QTextCursor *cursor, QString word, QStringList list, QColor *color)
{
	QTextCharFormat defaultFormat;
	defaultFormat.setForeground(Qt::white);
	for (int i = 0; i < list.size(); i++) {
		QString key = list[i];
		QRegExp r("\\b" + key + "\\b");
		if (word.contains(r)) {
			int idx = word.indexOf(r, 0);
			QTextCharFormat colorFormat;
			colorFormat.setForeground(QBrush(*color));
			cursor->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, idx);
			for (int j = 0; j < key.size(); j++) {
				cursor->deleteChar();
			}
			cursor->insertText(QString(key), colorFormat);
			cursor->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, word.size() - (key.size() + idx));
			return true;
		}
	}
	return false;
}

void GmacsSyntaxHighlighter::highlight(QTextCursor *cursor, int width)
{
	QTextBlock block = cursor->block();
	QString text = block.text();
	highlightInteractiveLine(text, cursor);
}

void GmacsSyntaxHighlighter::highlightPreserve(QTextCursor *cursor, GmacsToken *token)
{
	cout << "preserved" << endl;
	QString pre = token->word;
	int size = pre.size();
	for (int i = 0; i < size; i++) {
		cursor->deleteChar();
	}
	QTextCharFormat colorFormat;
	colorFormat.setForeground(QBrush(QColor("#40e0d0")));
	cursor->insertText(QString(pre), colorFormat);
}

void GmacsSyntaxHighlighter::highlightType(QTextCursor *cursor, GmacsToken *token)
{
	cout << "type" << endl;
	QString type = token->word;
	int size = type.size();
	for (int i = 0; i < size; i++) {
		cursor->deleteChar();
	}
	QTextCharFormat colorFormat;
	colorFormat.setForeground(QBrush(QColor("#ffaadd")));
	cursor->insertText(QString(type), colorFormat);
}

void GmacsSyntaxHighlighter::highlightVariable(QTextCursor *cursor, GmacsToken *token)
{
	cout << "var" << endl;
	QString var = token->word;
	int size = var.size();
	for (int i = 0; i < size; i++) {
		cursor->deleteChar();
	}
	QTextCharFormat colorFormat;
	colorFormat.setForeground(QBrush(QColor("#00ffff")));
	cursor->insertText(QString(var), colorFormat);
}

void GmacsSyntaxHighlighter::highlightFunction(QTextCursor *cursor, GmacsToken *token)
{
	cout << "func" << endl;
	QString func = token->word;
	int size = func.size();
	for (int i = 0; i < size; i++) {
		cursor->deleteChar();
	}
	QTextCharFormat colorFormat;
	colorFormat.setForeground(QBrush(QColor("#ff8c00")));
	cursor->insertText(QString(func), colorFormat);
}

void GmacsSyntaxHighlighter::highlightInstance(QTextCursor *cursor, GmacsToken *token)
{
	cout << "instance" << endl;
	QString inst = token->word;
	int size = inst.size();
	for (int i = 0; i < size; i++) {
		cursor->deleteChar();
	}
	QTextCharFormat colorFormat;
	colorFormat.setForeground(QBrush(QColor("#ff9999")));
	cursor->insertText(QString(inst), colorFormat);
}

void GmacsSyntaxHighlighter::highlightString(QTextCursor *cursor, GmacsToken *token)
{
	cout << "string" << endl;
	QString str = token->word;
	int size = str.size();
	qDebug() << str;
	for (int i = 0; i < size; i++) {
		cursor->deleteChar();
	}
	QTextCharFormat colorFormat;
	colorFormat.setForeground(QBrush(QColor("#00ff7f")));
	cursor->insertText(QString(str), colorFormat);
}

void GmacsSyntaxHighlighter::highlightComment(QTextCursor *cursor, GmacsToken *token)
{
	cout << "comment" << endl;
	QString com = token->word;
	GmacsWordType type = token->type;
	if (type == SPACE_T) {
		cursor->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, token->count);
		return;
	}
	int size = com.size();
	qDebug() << com;
	for (int i = 0; i < size; i++) {
		cursor->deleteChar();
	}
	QTextCharFormat colorFormat;
	colorFormat.setForeground(QBrush(QColor("#626063")));
	cursor->insertText(QString(com), colorFormat);
}

void GmacsSyntaxHighlighter::highlightDefault(QTextCursor *cursor, GmacsToken *token)
{
	QString def = token->word;
	int size = def.size();
	QTextCharFormat defaultFormat;
	defaultFormat.setForeground(QBrush(Qt::white));
	for (int i = 0; i < size; i++) {
		cursor->deleteChar();
	}
	cursor->insertText(def, defaultFormat);
}

void GmacsSyntaxHighlighter::highlightLine(QString text, QTextCursor *cursor)
{
	QTextCharFormat defaultFormat;
	defaultFormat.setForeground(Qt::white);
	GmacsToken **token = gt->split(text);
	gt->dump(token);
	gp->parse(token);
	addCompletionList(token);
	int offset = 0;
	int cur_pos = cursor->position();
	cursor->movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	for (int i = 0; token[i] != NULL; i++) {
		switch (token[i]->type) {
		case DEF_T:
			cout << "def" << endl;
			cursor->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, token[i]->word.size());
			break;
		case SPACE_T:
			cout << "space" << endl;
			cursor->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, token[i]->count);
			break;
		case TYPE_T:
			highlightType(cursor, token[i]);
			break;
		case VAR_T:
			highlightVariable(cursor, token[i]);
			break;
		case FUNC_T:
			highlightFunction(cursor, token[i]);
			break;
		case INSTANCE_T:
			highlightInstance(cursor, token[i]);
			break;
		case STRING_T:
			highlightString(cursor, token[i]);
			break;
		case PRESERVE_T:
			highlightPreserve(cursor, token[i]);
			break;
		case CONST_T:
			break;
		case CPP_T:
			break;
		case COMMENT_T:
			for (; token[i] != NULL; i++) {
				highlightComment(cursor, token[i]);
			}
			break;
		case ARROW_T:
			highlightInstance(cursor, token[i]);
			break;
		default:
			break;
		}
	}
	cursor->insertText(" ");
	cursor->deletePreviousChar();
	cursor->movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	cur_pos = cur_pos - cursor->position();
	cursor->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, cur_pos);
}

void GmacsSyntaxHighlighter::highlightInteractiveLine(QString text, QTextCursor *cursor)
{
	QTextCharFormat defaultFormat;
	defaultFormat.setForeground(Qt::white);
	GmacsToken **token = gt->split(text);
	gt->dump(token);
	gp->parse(token);
	int offset = 0;
	int cur_pos = cursor->position();
	cursor->movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	for (int i = 0; token[i] != NULL; i++) {
		switch (token[i]->type) {
		case DEF_T:
			cout << "def" << endl;
			highlightDefault(cursor, token[i]);
			break;
		case SPACE_T:
			cout << "space" << endl;
			cursor->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, token[i]->count);
			break;
		case TYPE_T:
			highlightType(cursor, token[i]);
			break;
		case VAR_T:
			highlightVariable(cursor, token[i]);
			break;
		case FUNC_T:
			highlightFunction(cursor, token[i]);
			break;
		case INSTANCE_T:
			highlightInstance(cursor, token[i]);
			break;
		case STRING_T:
			highlightString(cursor, token[i]);
			break;
		case PRESERVE_T:
			highlightPreserve(cursor, token[i]);
			break;
		case CONST_T:
			break;
		case CPP_T:
			break;
		case COMMENT_T:
			for (; token[i] != NULL; i++) {
				highlightComment(cursor, token[i]);
			}
			break;
		case ARROW_T:
			highlightInstance(cursor, token[i]);
			break;
		default:
			break;
		}
	}
	cursor->insertText(" ");
	cursor->deletePreviousChar();
	cursor->movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	cur_pos = cur_pos - cursor->position();
	cursor->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, cur_pos);
}

void GmacsSyntaxHighlighter::highlightAll(QTextCursor *cursor)
{
	cursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
	QTextBlock block = cursor->block();
	while (block.isValid()) {
		QString text = block.text();
		//qDebug() << text << endl;
		highlightLine(text, cursor);
		cursor->movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
		block = block.next();
	}
	cout << "end of highlight" << endl;
}

void GmacsSyntaxHighlighter::addCompletionList(GmacsToken **token)
{
	for (int i = 0; token[i] != NULL; i++) {
		GmacsWordType type = token[i]->type;
		QString word = token[i]->word;
		if (type != SPACE_T && word != "" &&
			word.contains(QRegExp("^\\w*$"))) {
			completion_list << word;
		}
	}
}

QStringList GmacsSyntaxHighlighter::getCompletionList(void)
{
	return completion_list;
}
*/
