#include <gmacs.hpp>

using namespace std;
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
