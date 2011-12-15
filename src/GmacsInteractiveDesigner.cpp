#include <gmacs.hpp>
#include <konoha1/inlinelibs.h>

#define QTC(script) script.toLocal8Bit().data()

GmacsInteractiveDesigner::GmacsInteractiveDesigner(void)
{
	const char *argv[] = {"konoha", "hoge.k"};
	konoha_ginit(0, argv);
	konoha_t konoha = konoha_open();
	CTX ctx = (CTX)konoha;
	KONOHA_BEGIN(ctx);
	knh_loadScriptPackageList(ctx, "konoha.i?");
	this->ctx = (kcontext_t *)ctx;
}

void GmacsInteractiveDesigner::eval(const QString &script_)
{
	//traverseFieldObject();
	QString removed_script = removeGlobalScopeConstructor(script_);//removeNoneModifiedScript(script_);
	const kcontext_t *lctx = knh_getCurrentContext();
	const char *script = (!prev_script.isEmpty()) ? QTC(removed_script) : QTC(script_);
	kline_t uline = 1;
	knh_eval(lctx, script, uline, NULL);
	prev_script = script_;
}

QString GmacsInteractiveDesigner::removeNoneModifiedScript(const QString &script)
{
	if (prev_script.isEmpty()) return "";
	QString ret = "";
	QStringList added_list;
	QStringList cur_lines = script.split("\n");
	QStringList prev_lines = prev_script.split("\n");
	int prev_size = prev_lines.size();
	int cur_size = cur_lines.size();
	for (int i = 0; i < cur_size; i++) {
		for (int j = 0; j < prev_size; j++) {
			if (cur_lines[i] == prev_lines[j]) {// && !added_list.contains(cur_lines[i])) {
				added_list << cur_lines[i];
			}
		}
	}
	//qDebug() << added_list;
	for (int i = 0; i < cur_size; i++) {
		if (!added_list.contains(cur_lines[i])) {
			ret += cur_lines[i];
		}
	}
	//qDebug() << "ret === " + ret;
	return ret;
}

QString GmacsInteractiveDesigner::removeGlobalScopeConstructor(const QString &script)
{
	//if (prev_script.isEmpty()) return "";
	QString ret = "";
	QStringList added_list;
	QStringList cur_lines = script.split("\n");
	//QStringList prev_lines = prev_script.split("\n");
	//int prev_size = prev_lines.size();
	QRegExp exp("\\bnew\\b");
	QRegExp left_brace("\\b{\\b");
	QRegExp right_brace("\\b}\\b");
	int cur_size = cur_lines.size();
	int indent = 0;
	for (int i = 0; i < cur_size; i++) {
		if (cur_lines[i].contains("{")) {
			fprintf(stderr, "indent++");
			indent++;
		}
		if (cur_lines[i].contains("}")) {
			fprintf(stderr, "indent--");
			indent--;
		}
		int idx = exp.indexIn(cur_lines[i]);
		if (idx < 0) {
			ret += cur_lines[i] + "\n";
		} else if (indent > 0) {
			//not global scope line;
			ret += cur_lines[i] + "\n";
		} else {
			//fprintf(stderr, "indent = [%d]\n", indent);
			if (!global_decls.contains(cur_lines[i])) {
				ret += cur_lines[i] + "\n";
				global_decls << cur_lines[i];
			}
		}
		//for (int j = 0; j < prev_size; j++) {
		//if (cur_lines[i] == prev_lines[j]) {// && !added_list.contains(cur_lines[i])) {
		//added_list << cur_lines[i];
		//}
		//}
	}
	//qDebug() << added_list;
//	for (int i = 0; i < cur_size; i++) {
//		if (!added_list.contains(cur_lines[i])) {
//			ret += cur_lines[i];
//		}
//	}
	qDebug() << "ret === " + ret;
	return ret;
}

void GmacsInteractiveDesigner::traverseFieldObject(void)
{
	const kcontext_t *lctx = knh_getCurrentContext();
	struct kObject **fields = lctx->script->fields;
	if (!fields) return;
	for (int i = 0; fields[i] != NULL; i++) {
		kObject *o = fields[i];
		fprintf(stderr, "Object Type = [%s]\n", S_totext(o->h.cTBL->sname));
	}
}

GmacsInteractiveDesigner::~GmacsInteractiveDesigner(void)
{
	KONOHA_END(ctx);
	konoha_close(ctx);
}
