#include <gmacs.hpp>
#include <konoha1/inlinelibs.h>

#define QTC(script) script.constData()//script.toLocal8Bit().data()
#define toString(script) #script
#define LOGFILE_NAME "state_log.log"

static int selected_state_num;
static GmacsStatePixmapItem *prev_state_pixmap;
static GmacsStatePixmapItem *cur_state_pixmap;

GmacsStateScene::GmacsStateScene()
{
	prev_selecteditem = NULL;
}

void GmacsStateScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	fprintf(stderr, "GmacsStateScene:mouse pressed\n");
	QPointF p = event->scenePos();
	QGraphicsItem *item = itemAt(p.x(), p.y());
	if (item) {
		QGraphicsEffect *e = item->graphicsEffect();
		e->setEnabled(true);
		if (prev_selecteditem) {
			e = prev_selecteditem->graphicsEffect();
			e->setEnabled(false);
		}
		prev_selecteditem = item;
	}
	QGraphicsScene::mousePressEvent(event);
}

void GmacsStateScene::addItem(GmacsStatePixmapItem *i)
{
	items << i;
	QGraphicsScene::addItem(i);
}

void GmacsStateScene::clearStatePixmapItem(void)
{
	foreach (GmacsStatePixmapItem *i, items) {
		if (i->state_num > selected_state_num) {
			items.removeOne(i);
			delete i;
			i = NULL;
		}
	}
}

GmacsStatePixmapItem::GmacsStatePixmapItem(const QPixmap &pixmap, int state_num) : QGraphicsPixmapItem(pixmap)
{
	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
	effect->setColor(QColor("#00ffff"));
	effect->setOffset(0, 0);
	effect->setBlurRadius(50);
	effect->setEnabled(true);
	setGraphicsEffect(effect);
	this->state_num = state_num;
}

void GmacsStatePixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsEffect *e = graphicsEffect();
	e->setEnabled(true);
	cur_state_pixmap = this;
	selected_state_num = state_num;
}

GmacsInteractiveDesigner::GmacsInteractiveDesigner(void)
{
	scene = new GmacsStateScene();
	state_num = 0;
	prev_state_pixmap = NULL;
	selected_state_num = 0;
	scene->setSceneRect(0, 0, 250, 550);
	prev_logfilesize = getLogFileSize();
	isFirstEvaluation = true;
	startTimer(1000);
}

long long int GmacsInteractiveDesigner::getLogFileSize()
{
	struct stat status;
	long long int ret = stat(LOGFILE_NAME, &status);
	if (ret < 0) {
		ret = 0;
	} else {
		ret = (long long int)status.st_size;
	}
	return ret;
}

void GmacsInteractiveDesigner::timerEvent(QTimerEvent *event)
{
	long long int filesize = getLogFileSize();
	if (prev_logfilesize != filesize) {
		fprintf(stderr, "write to logfile\n");
		QPixmap screenshot = QPixmap::grabWidget(QApplication::activeWindow());
		//screenshot.save("screenshot.jpg");
		screenshot = screenshot.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		GmacsStatePixmapItem *pixmap = new GmacsStatePixmapItem(screenshot, state_num);
		int width_num = state_num % 2;
		int height_num = state_num / 2;
		pixmap->setPos(10 + width_num * 110, height_num * 110);
		scene->addItem(pixmap);
		if (prev_state_pixmap) {
			QGraphicsEffect *e = prev_state_pixmap->graphicsEffect();
			e->setEnabled(false);
		}
		prev_state_pixmap = pixmap;
		scene->prev_selecteditem = pixmap;
		selected_state_num = state_num;
		cur_state_pixmap = pixmap;
		state_num++;
		prev_logfilesize = filesize;
	}
}

void GmacsInteractiveDesigner::eval(const QString &script_)
{
	traverseFieldObject();
	QString script;
	QString definition;
	QString executor_exec;
	bool isResetSharedObjectFlag = false;
	if (isFirstEvaluation) {
		definition += QString("\ninclude \"define.k\";\n");
		definition += QString("EventExecutor executor = new ();\n");
	} else {
		isResetSharedObjectFlag = true;
		char buf[32];
		snprintf(buf, 32, "executor.setStateNum(%d);\n", selected_state_num);
		executor_exec += QString(buf);
		executor_exec += QString("executor.go();\n");
		state_num = (selected_state_num == 0) ? 1 : selected_state_num;
		prev_state_pixmap = cur_state_pixmap;
		scene->prev_selecteditem = cur_state_pixmap;
		scene->clearStatePixmapItem();
	}
	script = appendExecutionCode(script_, executor_exec);
	script = definition + script;
	QStringList event_func_list = getEventFunctionList(script);
	script = appendEventStateCode(script, event_func_list);
	const kcontext_t *lctx = knh_getCurrentContext();
	QString removed_script;
	if (isResetSharedObjectFlag) {
		struct kObject **fields = lctx->script->fields;
		if (fields) {
			for (int i = 0; fields[i] != NULL; i++) {
				kObject *o = fields[i];
				//TODO : needs to detect "main application object"
				if (o != NULL && QString(S_totext(o->h.cTBL->sname)) == "QWidget") {
					QWidget *w = (QWidget *)((kRawPtr *)o)->rawptr;
					delete w;
				}
			}
		}
		removed_script = script;
	} else {
		removed_script = removeGlobalScopeConstructor(script);//removeNoneModifiedScript(script_);
	}
	QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
    QByteArray scriptBuffer = utf8->fromUnicode(script);
	QByteArray removed_scriptBuffer = utf8->fromUnicode(removed_script);
	const char *rawscript = (!prev_script.isEmpty()) ? QTC(removed_scriptBuffer) : QTC(scriptBuffer);
	//fprintf(stderr, "script = [%s]\n", rawscript);
	kline_t uline = 1;
	knh_eval(lctx, rawscript, uline, NULL);
	if (isFirstEvaluation) {
		QPixmap screenshot = QPixmap::grabWidget(QApplication::activeWindow());//desktop()->winId());
		screenshot = screenshot.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		GmacsStatePixmapItem *pixmap = new GmacsStatePixmapItem(screenshot, state_num);
		int width_num = state_num % 2;
		int height_num = state_num / 2;
		pixmap->setPos(10 + width_num * 110, height_num * 110);
		scene->addItem(pixmap);
		selected_state_num = state_num;
		cur_state_pixmap = pixmap;
		if (prev_state_pixmap) {
			QGraphicsEffect *e = prev_state_pixmap->graphicsEffect();
			e->setEnabled(false);
		}
		prev_state_pixmap = pixmap;
		scene->prev_selecteditem = pixmap;
		selected_state_num = state_num;
		state_num++;
		isFirstEvaluation = false;
	}
	prev_script = script;
}

QString GmacsInteractiveDesigner::appendExecutionCode(const QString &script, const QString &exec)
{
	QStringList lines = script.split("\n");
	int line_max_num = lines.size();
	QString ret;
	QRegExp exp("\\bapp.exec()\\b");
	for (int i = 0; i < line_max_num; i++) {
		QString line = lines[i];
		int idx = exp.indexIn(line);
		if (idx >= 0) {
			//find app.exec() code
			line = exec + line;
		}
		ret += line + "\n";
	}
	return ret;
}

QString GmacsInteractiveDesigner::appendEventStateCode(const QString &script, const QStringList &event_func_list)
{
	QStringList lines = script.split("\n");
	int line_max_num = lines.size();
	QString ret;
	QList<QRegExp> patterns;
	foreach (const QString &func, event_func_list) {
		patterns << QRegExp("\\b" + func + "(?=\\()");
	}
	bool isFindFunction = false;
	for (int i = 0; i < line_max_num; i++) {
		QString line = lines[i];
		if (isFindFunction) {
			isFindFunction = false;
			continue;
		}
		int j = 0;
		foreach (QRegExp exp, patterns) {
			int idx = exp.indexIn(line);
			if (idx >= 0) {
				QRegExp args_pattern("\\b" + event_func_list[j] + "\\((.+)\\b");
				QString values;
				idx = args_pattern.indexIn(line);
				QStringList captured = args_pattern.capturedTexts();
				if (idx >= 0) {
					QStringList args = captured[1].split(",");
					foreach (QString arg, args) {
						QRegExp value_pattern("\\s?\\w+\\s+(\\w+)");
						idx = value_pattern.indexIn(arg);
						captured = value_pattern.capturedTexts();
						values += "\targs << " + captured[1] + ";\n";
					}
					//qDebug() << args;
				}
				QString state_code("\targs = [];\n" + values +
								   "\tEventState state = new (" + event_func_list[j] + ", args);\n"
								   "\texecutor.add(state);\n");
				line = line + "\n{\n" + state_code;
				isFindFunction = true;//TODO not supported "function() {" case
			}
			j++;
		}
		ret += line + "\n";
	}
	return ret;
}

QStringList GmacsInteractiveDesigner::getEventFunctionList(const QString &script)
{
	QStringList lines = script.split("\n");
	int line_max_num = lines.size();
	QStringList event_func_list;
	QRegExp exp("\\bsignalConnect(.+)\\b");
	QRegExp exp2("\\baddEvent(.+)\\b");
	qDebug() << "===============EVENT FUNCTION LISTS===============";
	for (int i = 0; i < line_max_num; i++) {
		QString line = lines[i];
		int idx = exp.indexIn(line);
		QStringList captured = exp.capturedTexts();
		if (idx >= 0) {
			QString event_func_name = captured[1].split(",")[1].trimmed();
			if (!event_func_list.contains(event_func_name)) {
				event_func_list << event_func_name;
			}
		}
		idx = exp2.indexIn(line);
		captured = exp2.capturedTexts();
		if (idx >= 0) {
			QString event_func_name = captured[1].split(",")[1].trimmed();
			if (!event_func_list.contains(event_func_name)) {
				event_func_list << event_func_name;
			}
		}
	}
	qDebug() << event_func_list;
	qDebug() << "=================================================";
	return event_func_list;
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
	//qDebug() << "ret === " + ret;
	return ret;
}

void GmacsInteractiveDesigner::traverseFieldObject(void)
{
	const kcontext_t *lctx = knh_getCurrentContext();
	struct kObject **fields = lctx->script->fields;
	if (!fields) return;
	for (int i = 0; fields[i] != NULL; i++) {
		kObject *o = fields[i];
		if (o != NULL) fprintf(stderr, "Object Type = [%s]\n", S_totext(o->h.cTBL->sname));
	}
}

GmacsInteractiveDesigner::~GmacsInteractiveDesigner(void)
{
	KONOHA_END(ctx);
	konoha_close(ctx);
}
