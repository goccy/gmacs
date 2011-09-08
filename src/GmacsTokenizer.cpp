#include <gmacs.hpp>
using namespace std;

GmacsTokenizer::GmacsTokenizer(void)
{

}

GmacsToken::GmacsToken(QString w, GmacsWordType t, int c)
{
	word = w;
	type = t;
	count = c;
}

GmacsToken **GmacsTokenizer::split(QString text)
{
	int i = 0;
	QStringList list;
	QString word;
	qDebug() << "text = " << text;
	GmacsToken **token = (GmacsToken **)malloc(sizeof(GmacsToken *) * MAX_TOKEN_SIZE);
	memset(token, 0, sizeof(GmacsToken *) * MAX_TOKEN_SIZE);
	int t_cnt = 0;
	while (i < text.size()) {
		if (text[i] == '"') {
			if (word != "") {
				list << word;
				GmacsToken *gt = new GmacsToken(word, DEF_T, 0);
				token[t_cnt] = gt;
				t_cnt++;
			}
			word = "";
			i++;
			for (; text[i] != '"' && i < text.size(); i++) {
				word += QString(text[i]);
			}
			if (i == text.size()) {
				GmacsToken *gt = new GmacsToken("\"" + word, STRING_T, 0);
				token[t_cnt] = gt;
				t_cnt++;
				return token;
			}
			GmacsToken *gt = new GmacsToken("\"" + word + "\"", STRING_T, 0);
			token[t_cnt] = gt;
			t_cnt++;
			list << "\"" + word + "\"";
			word = "";
			goto L_END;
		} else if (text[i] == ' ') {
			GmacsToken *gt = new GmacsToken(word, DEF_T, 0);
			token[t_cnt] = gt;
			t_cnt++;
			list << word;
			word = "";
			int space_cnt = 0;
			for (; text[i] == ' ' && i < text.size(); i++) {
				space_cnt++;
			}
			gt = new GmacsToken("", SPACE_T, space_cnt);
			token[t_cnt] = gt;
			t_cnt++;
			i--;
			qDebug() << "space count is " << space_cnt;
			goto L_END;
		} else if (text[i] == '/') {
			if (word != "") {
				list << word;
				GmacsToken *gt = new GmacsToken(word, DEF_T, 0);
				token[t_cnt] = gt;
				t_cnt++;
			}
			if (text[i + 1] == '/' || text[i + 1] == '*') {
				i++;
				list << "/" + text[i];
				GmacsToken *gt = new GmacsToken("/" + text[i], COMMENT_T, 0);
				token[t_cnt] = gt;
				t_cnt++;
			} else {
				list << "/";
				GmacsToken *gt = new GmacsToken("/", DEF_T, 0);
				token[t_cnt] = gt;
				t_cnt++;
			}
			word = "";
			goto L_END;
		} else if (text[i] == '-') {
			if (text[i + 1] == '>') {
				if (word != "") {
					list << word;
					GmacsToken *gt = new GmacsToken(word, ARROW_T, 0);
					token[t_cnt] = gt;
					t_cnt++;
				}
				word = "";
				i++;
				list << "->";
				GmacsToken *gt = new GmacsToken("->", DEF_T, 0);
				token[t_cnt] = gt;
				t_cnt++;
			} else if (QString(text[i + 1]) == "[0-9]") {
				if (word != "") {
					list << word;
					GmacsToken *gt = new GmacsToken(word, DEF_T, 0);
					token[t_cnt] = gt;
					t_cnt++;
				}
				word = "";
				i++;
				for (; QString(text[i]) == "[^0-9.]" && i < text.size(); i++) {
					word += QString(text[i]);
				}
				i--;
				GmacsToken *gt = new GmacsToken("-" + word, DEF_T, 0);
				token[t_cnt] = gt;
				t_cnt++;
				list << "-" + word;
			}
			word = "";
			goto L_END;
		} else if (text[i] == '(' || text[i] == ')' ||
				   text[i] == '*' || text[i] == '+' ||
				   text[i] == ':' || text[i] == ',' || text[i] == ';') {
			if (word != "") {
				list << word;
				GmacsToken *gt = new GmacsToken(word, DEF_T, 0);
				token[t_cnt] = gt;
				t_cnt++;
			}
			list << QString(text[i]);
			GmacsToken *gt = new GmacsToken(QString(text[i]), DEF_T, 0);
			token[t_cnt] = gt;
			t_cnt++;
			word = "";
			goto L_END;
		} else if (text[i] == '.') {
			if (word != "") {
				list << word;
				GmacsToken *gt = new GmacsToken(word, INSTANCE_T, 0);
				token[t_cnt] = gt;
				t_cnt++;
			}
			list << ".";
			GmacsToken *gt = new GmacsToken(".", DEF_T, 0);
			token[t_cnt] = gt;
			t_cnt++;
			word = "";
			goto L_END;
		}
		word += QString(text[i]);
	L_END:;
		i++;
	}
	list << word;
	GmacsToken *gt = new GmacsToken(word, DEF_T, 0);
	token[t_cnt] = gt;
	t_cnt++;
	token[t_cnt] = NULL;
	for (int i = 0; i < list.size(); i++) {
		qDebug() << "list["<<i<<"] = " << list[i];
	}
	return token;
}

void GmacsTokenizer::dump(GmacsToken **token)
{
	for (int i = 0; token[i] != NULL; i++) {
		GmacsToken *t = token[i];
		qDebug() << t->word << t->type << t->count;
	}
}
