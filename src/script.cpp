#include <gmacs.hpp>

GmacsScriptLoader::GmacsScriptLoader()
{
	//load gmacs.conf
	text_mode = "konoha";
	const char *a[32] = {"while", "for", "if", "else", "switch", "goto", "static", "OUT", "EOL",
						 "case", "default", "break", "continue", "new", "class", "using", "include",
						 "foreach", "print", "return", NULL};
	for (int i = 0; a[i] != NULL; i++) {
		preserved_keyword_list << a[i];
	}
	preserved_keyword_color = new QColor("#40e0d0");
	const char *b[] = {"void", "int", "Int", "float", "Float", "boolean", "Boolean", "String", "Map", "char", "dynamic",
					   "Array", "dyn", "InputStream", "OutputStream", "Func", NULL};
	for (int i = 0; b[i] != NULL; i++) {
		type_keyword_list << b[i];
	}
	type_keyword_color = new QColor("#ffaadd");
}

QString GmacsScriptLoader::loadScript(QString filepath)
{
	QString script = "";
	QFile textFile(filepath);
	if (textFile.open(QIODevice::ReadWrite)) { 
		QTextStream input(&textFile);
		script = input.readAll();
		textFile.close();
	}
	return script;
}
