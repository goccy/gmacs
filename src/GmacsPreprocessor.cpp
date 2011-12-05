#include <gmacs.hpp>


GmacsPreprocessor::GmacsPreprocessor(void)
{

}

void GmacsPreprocessor::setMode(const QString &mode)
{

}

void GmacsPreprocessor::setDocument(const QString &document_)
{
	document = document_;
}

void GmacsPreprocessor::start(void)
{
	//class T
	//struct T
	//typedef struct {...} T;
	//typedef enum {...} T;
	//typedef type(*function_ptr_type)(...);
	QRegExp class_expression("\\bclass(\\s+[A-Za-z0-9_]+)\\b");
	QRegExp struct_expression("\\bstruct(\\s+[A-Za-z0-9_]+)\\b");
	QRegExp typedef_expression("\\btypedef(\\s+struct)(\\s+[A-Za-z0-9_]+)\\b");
	QStringList list = document.split("\n");
	analyze(list, class_expression);
	analyze(list, struct_expression);
	analyze(list, typedef_expression);
}

void GmacsPreprocessor::analyze(const QStringList &list, const QRegExp &expression)
{
	int size = list.size();
	for (int i = 0; i < size; i++) {
		QString word = list[i];
		int idx = expression.indexIn(word);
		if (word.isEmpty()) continue;
		while (idx >= 0) {
			int length = expression.matchedLength();
			QString w = word.mid(idx, length).trimmed();
			if (!added_words.contains(w)) {
				QStringList texts = expression.capturedTexts();
				if (texts.size() > 1) {
					qDebug() << texts;
					QString type_name = texts[1].trimmed();
					if (type_name == "struct" &&
						texts.size() > 2 && !texts[2].isEmpty()) {
						type_name = texts[2].trimmed();
					}
					qDebug() << type_name;
					added_words << type_name;
					//traverseTypeObject(list, i);
				}
			}
			idx = expression.indexIn(word, idx + length);
		}
	}
}

void GmacsPreprocessor::traverseTypeObject(const QStringList &list, int start_idx)
{
	(void)list;
	(void)start_idx;
	/*
	int size = list.size();
	int brace_cont = 0;
	QRegExp left_brace("{");
	QRegExp right_brace("}");
	for (int i = start_idx; i < size; i++) {
		QString line = list[start_idx];
		int idx = left_brace.indexIn(line);
		if (word.isEmpty()) continue;
		while (idx >= 0) {
			int length = expression.matchedLength();
			QString w = word.mid(idx, length).trimmed();
			if (!added_words.contains(w)) {
				QStringList texts = expression.capturedTexts();
				if (texts.size() > 1) {
					QString type_name = texts[1].trimmed();
					qDebug() << type_name;
					added_words << type_name;
					traverseTypeObject(list, i);
				}
			}
			idx = expression.indexIn(word, idx + length);
		}
	}
	*/
}
