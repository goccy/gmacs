#include <gmacs.hpp>

GmacsParser::GmacsParser(QString m, QStringList plist, QStringList tlist)
{
	mode = m;
	preserved_keyword_list = plist;
	type_keyword_list = tlist;
	isClassType = false;
}

bool GmacsParser::searchFromType(GmacsToken *t)
{
	QString word = t->word;
	GmacsWordType type = t->type;
	if (type == STRING_T) return false;
	QStringList list = type_keyword_list;
	for (int i = 0; i < list.size(); i++) {
		QString key = list[i];
		QRegExp r("\\b" + key + "\\b");
		if (word.contains(r)) {
			return true;
		}
	}
	return false;
}

bool GmacsParser::searchFromPreserve(GmacsToken *t)
{
	QString word = t->word;
	GmacsWordType type = t->type;
	if (type == STRING_T) return false;
	QStringList list = preserved_keyword_list;
	for (int i = 0; i < list.size(); i++) {
		QString key = list[i];
		QRegExp r("\\b" + key + "\\b");
		if (word.contains(r)) {
			qDebug() << "key = " << key;
			if (key == "class") {
				isClassType = true;
			}
			return true;
		}
	}
	return false;
}

int GmacsParser::getFuncNameIdx(GmacsToken **token)
{
	for (int i = 0; token[i] != NULL; i++) {
		if (token[i]->type == TYPE_T) {
			i++;
			if (token[i] != NULL && token[i]->type == SPACE_T) i++;
			//<<==== function name
			int ret = i;
			i++;
			if (token[i] != NULL && token[i]->type == SPACE_T) i++;
			if (token[i] != NULL && token[i]->word == "(") {
				return ret;
			}
		}
	}
	return -1;
}

void GmacsParser::parse(GmacsToken **token)
{
	bool isType = false;
	for (int i = 0; token[i] != NULL; i++) {
		if (searchFromType(token[i])) {
			token[i]->type = TYPE_T;
			isType = true;
		} else if (searchFromPreserve(token[i])) {
			token[i]->type = PRESERVE_T;
			if (isClassType == true) {
				if (token[i + 1] != NULL && token[i + 1]->type == SPACE_T &&
					token[i + 2] != NULL && token[i + 2]->word.contains(QRegExp("^\\w*$"))) {
					addType(token[i + 2]->word);
				} else if (token[i + 1] != NULL && token[i + 1]->type != SPACE_T &&
						   token[i + 1]->word.contains(QRegExp("^\\w*$"))) {
					addType(token[i + 1]->word);
				}
			}
			isClassType = false;
		} else if (token[i]->type == DEF_T && isType &&
				   token[i]->word.contains(QRegExp("^\\w*$"))) {
			token[i]->type = VAR_T;
			isType = false;
		}
	}
	int name_idx = getFuncNameIdx(token);
	if (name_idx > 0) {
		token[name_idx]->type = FUNC_T;
	}
}

void GmacsParser::addType(QString type_name)
{
	qDebug() << type_name;
	type_keyword_list << type_name;
}
