#include <gmacs.hpp>

static inline CXTranslationUnit CLANG_PARSE(const CXIndex &idx, const char *filename, char **args, int args_size, CXUnsavedFile *unsaved_file)
{
	int unsaved_file_num = 0;
	if (unsaved_file) unsaved_file_num = 1;
	return clang_parseTranslationUnit(idx, filename, args, args_size,
									  unsaved_file, unsaved_file_num,
									  clang_defaultEditingTranslationUnitOptions()|
									  CXTranslationUnit_PrecompiledPreamble|
									  CXTranslationUnit_CXXPrecompiledPreamble|
									  CXTranslationUnit_CacheCompletionResults);
}

#define CLANG_REPARSE(tu, unsaved_file) {								\
		if (unsaved_file) {												\
			clang_reparseTranslationUnit(tu, 1, unsaved_file, clang_defaultReparseOptions(tu));	\
		} else {														\
			clang_reparseTranslationUnit(tu, 0, 0, clang_defaultReparseOptions(tu)); \
		}																\
	}

#define CLANG_CODE_COMPLETION(unit, filename, line, column) {		\
		clang_codeCompleteAt(unit, filename, line, column, 0, 0, 0)	\
			}

GmacsPreprocessor::GmacsPreprocessor(void)
{
	const char *dirs[] = {"-I/usr/include/", "-I/usr/local/include/",
						  "-I/usr/local/Trolltech/Qt-4.7.3/include/",
						  "-I/usr/local/Trolltech/Qt-4.7.3/include/QtGui/",
						  "-I/opt/include", "-I/opt/local/include/",
						  "-I./", "-I./include/"};
	int size = sizeof(dirs) / sizeof(char *);
	include_dirs = new QList<char *>();
	for (int i = 0; i < size; i++) {
		int length = strlen(dirs[i]);
		char *dir = (char *)malloc(length + 1);
		strncpy(dir, dirs[i], length + 1);
		include_dirs->append(dir);
	}
	include_dirs_size = size;
}

GmacsPreprocessor::~GmacsPreprocessor(void)
{
	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(idx);
}

void GmacsPreprocessor::compile(const QString &filename, const QTextCursor &cursor)
{
	if (filename.isEmpty()) return;
	idx = clang_createIndex(1, 0);
	if (!idx) fprintf(stderr, "ERROR: createIndex failed\n");
	int clang_arg_size = include_dirs_size;
	char *clang_arg_ptr[clang_arg_size];
	for (int i = 0; i < clang_arg_size; i++) {
		clang_arg_ptr[i] = include_dirs->at(i);
		//fprintf(stderr, "dirs = [%s]\n", clang_arg_ptr[i]);
	}
	//fprintf(stderr, "filename = [%s]\n", filename);
	QString document = cursor.document()->toPlainText();
	unsaved_file = new CXUnsavedFile();
	//unsaved_file->Filename = filename;
	//unsaved_file->Contents = document.toLocal8Bit().data();
	//unsaved_file->Length = document.size();
	const char *name = filename.toLocal8Bit().data();
	unit = CLANG_PARSE(idx, name, clang_arg_ptr, clang_arg_size, NULL);//unsaved_file);
	if (!unit) {
		fprintf(stderr, "ERROR parseTranslationUnit failed\n");
	} else {
		CLANG_REPARSE(unit, NULL);//unsaved_file);
	}
}

void GmacsPreprocessor::codeCompletion(const QTextCursor &cursor)
{
	int line = cursor.blockNumber();
	int column = cursor.columnNumber();
	//const char *name = filename->toLocal8Bit().data();
	const char *name = clang_getCString(clang_getTranslationUnitSpelling(unit));
	//qDebug() << cursor.document()->toPlainText();
	//qDebug() << cursor.document()->toPlainText().size();
	QString document = cursor.document()->toPlainText();
	QString text = cursor.block().text();
	QRegExp exp("\t");
	int tab_count = text.count(exp);
	//unsaved_file->Filename = name;
	//unsaved_file->Contents = document.toLocal8Bit().data();
	//unsaved_file->Length = document.size();
	if (!unit) return;
	CLANG_REPARSE(unit, NULL);//unsaved_file);
	fprintf(stderr, "line = [%d], column = [%d]\n", line+1, column + tab_count);
	fprintf(stderr, "name = [%s]\n", name);
	CXCodeCompleteResults *res = CLANG_CODE_COMPLETION(unit, name, line+1, column + tab_count * 2);
	if (!res) fprintf(stderr, "ERROR: could not complete\n");
	for (size_t i = 0; i < clang_codeCompleteGetNumDiagnostics(res); i++) {
		const CXDiagnostic &diag = clang_codeCompleteGetDiagnostic(res, i);
		const CXString &s = clang_getDiagnosticSpelling(diag);
		fprintf(stderr, "%s\n", clang_getCString(s));
	}
	unsigned num_results = res->NumResults;
	fprintf(stderr, "num_results = [%d]\n");
    for (unsigned i = 0; i < num_results; i++) {
        const CXCompletionString& str = res->Results[i].CompletionString;
		unsigned chunks = clang_getNumCompletionChunks(str);
        for (unsigned j = 0; j < chunks; j++) {
            const CXString& out = clang_getCompletionChunkText(str, j);
            std::cout << clang_getCString(out) << " ";
            if (clang_getCompletionChunkKind(str, j) != CXCompletionChunk_TypedText)
                continue;
        }
        std::cout << std::endl;
    }
	clang_disposeCodeCompleteResults(res);
}

void GmacsPreprocessor::setMode(const QString &mode)
{
	(void)mode;
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
					QStringList property = getTypeObjectLines(list, i);
					//traverseTypeObject(list, i);
				}
			}
			idx = expression.indexIn(word, idx + length);
		}
	}
}

QStringList GmacsPreprocessor::getTypeObjectLines(const QStringList &list, int start_idx)
{
	int size = list.size();
	int brace_cont = 0;
	QRegExp left_brace("\\b{\\b");
	QRegExp right_brace("\\b}\\b");
	QStringList ret;
	for (int i = start_idx; i < size; i++) {
		QString line = list[i];
		//qDebug() << line;
		int idx = left_brace.indexIn(line);
		if (idx >= 0) brace_cont++;
		idx = right_brace.indexIn(line);
		if (idx >= 0) brace_cont--;
		ret << line + "\n";
		if (brace_cont <= 0) break;
	}
	//qDebug() << "===============";
	//qDebug() << ret;
	return ret;
}
