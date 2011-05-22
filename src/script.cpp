#include <gmacs.hpp>

GmacsScriptLoader::GmacsScriptLoader()
{
	//load gmacs.conf
	text_mode = "konoha";
	const char *a[16] = {"while", "for", "if", "else", "switch", "new", "class", "using", "foreach", "print", NULL};
	for (int i = 0; a[i] != NULL; i++) {
		preserved_keyword_list << a[i];
	}
	preserved_keyword_color = new QColor("#00ffff");
	const char *b[] = {"int", "float", "boolean", "String", "Map", "Array", "dyn", "Func", NULL};
	for (int i = 0; b[i] != NULL; i++) {
		type_keyword_list << b[i];
	}
	type_keyword_color = new QColor("#ff00ff");
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
