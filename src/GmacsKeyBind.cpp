#include <gmacs.hpp>

enum {
	CTRLA = 0, CTRLB, CTRLC, CTRLD, CTRLE,
	CTRLF, CTRLG, CTRLH, CTRLI, CTRLJ, CTRLK,
	CTRLL, CTRLM, CTRLN, CTRLO, CTRLP, CTRLQ,
	CTRLR, CTRLS, CTRLT, CTRLU, CTRLV, CTRLW,
	CTRLX, CTRLY, CTRLZ,
};

#define CTRL(key) pressCTRL ## key
#define BIND_KEY(key, func) key_bind_list[key] = func;
#define BIND_COMMAND(command, func) command_bind_list[command] = func

GmacsKeyBind::GmacsKeyBind(void)
{
	//TODO load from Script
	static GmacsKeyBindFunc key_bind_list_[] = {
		CTRL(A), CTRL(B), CTRL(C), CTRL(D), CTRL(E),
		CTRL(F), CTRL(G), CTRL(H), CTRL(I), CTRL(J),
		CTRL(K), CTRL(L), CTRL(M), CTRL(N), CTRL(O),
		CTRL(P), CTRL(Q), CTRL(R), CTRL(S), CTRL(T),
		CTRL(U), CTRL(V), CTRL(W), CTRL(X), CTRL(Y),
		CTRL(Z), NULL,
	};

	static GmacsKeyBindFunc command_bind_list_[] = {
		CTRL(A), CTRL(B), CTRL(C), CTRL(D), CTRL(E),
		CTRL(F), CTRL(G), CTRL(H), CTRL(I), CTRL(J),
		CTRL(K), CTRL(L), CTRL(M), CTRL(N), CTRL(O),
		CTRL(P), CTRL(Q), CTRL(R), CTRL(S), CTRL(T),
		CTRL(U), CTRL(V), CTRL(W), CTRL(X), CTRL(Y),
		CTRL(Z), NULL,
	};
	key_bind_list = key_bind_list_;
	command_bind_list = command_bind_list_;
	bindKeys();
	bindCommands();
}

void GmacsKeyBind::bindKeys(void)
{
	BIND_KEY(CTRLA, &GmacsKeyBind::moveLineTop);
	BIND_KEY(CTRLB, &GmacsKeyBind::moveLeft);
	BIND_KEY(CTRLC, NULL);
	BIND_KEY(CTRLD, &GmacsKeyBind::deleteChar);
	BIND_KEY(CTRLE, &GmacsKeyBind::moveLineEnd);
	BIND_KEY(CTRLF, &GmacsKeyBind::moveRight);
	BIND_KEY(CTRLG, &GmacsKeyBind::clearCommand);
	BIND_KEY(CTRLH, NULL);
	BIND_KEY(CTRLI, NULL);
	BIND_KEY(CTRLJ, NULL);
	BIND_KEY(CTRLK, &GmacsKeyBind::killText);
	BIND_KEY(CTRLL, NULL);
	BIND_KEY(CTRLM, NULL);
	BIND_KEY(CTRLN, &GmacsKeyBind::moveDown);
	BIND_KEY(CTRLO, NULL);
	BIND_KEY(CTRLP, &GmacsKeyBind::moveUp);
	BIND_KEY(CTRLQ, NULL);
	BIND_KEY(CTRLR, NULL);
	BIND_KEY(CTRLS, NULL);
	BIND_KEY(CTRLT, NULL);
	BIND_KEY(CTRLU, NULL);
	BIND_KEY(CTRLV, NULL);
	BIND_KEY(CTRLW, NULL);
	BIND_KEY(CTRLX, &GmacsKeyBind::startCommand);
	BIND_KEY(CTRLY, &GmacsKeyBind::yankText);
	BIND_KEY(CTRLZ, NULL);
}

void GmacsKeyBind::bindCommands(void)
{
	BIND_COMMAND(CTRLA, NULL);
	BIND_COMMAND(CTRLB, NULL);
	BIND_COMMAND(CTRLC, NULL);
	BIND_COMMAND(CTRLD, &GmacsKeyBind::toggleStateDock);//GmacsOriginalCommand
	BIND_COMMAND(CTRLE, NULL);
	BIND_COMMAND(CTRLF, &GmacsKeyBind::findFile);
	BIND_COMMAND(CTRLG, &GmacsKeyBind::clearCommand);
	BIND_COMMAND(CTRLH, NULL);
	BIND_COMMAND(CTRLI, NULL);
	BIND_COMMAND(CTRLJ, NULL);
	BIND_COMMAND(CTRLK, NULL);
	BIND_COMMAND(CTRLL, NULL);
	BIND_COMMAND(CTRLM, NULL);
	BIND_COMMAND(CTRLN, NULL);
	BIND_COMMAND(CTRLO, NULL);
	BIND_COMMAND(CTRLP, NULL);
	BIND_COMMAND(CTRLQ, NULL);
	BIND_COMMAND(CTRLR, NULL);
	BIND_COMMAND(CTRLS, NULL);
	BIND_COMMAND(CTRLT, NULL);
	BIND_COMMAND(CTRLU, NULL);
	BIND_COMMAND(CTRLV, NULL);
	BIND_COMMAND(CTRLW, NULL);
	BIND_COMMAND(CTRLX, NULL);
	BIND_COMMAND(CTRLY, NULL);
	BIND_COMMAND(CTRLZ, NULL);
}

GmacsKeyBindFunc GmacsKeyBind::getKeyBindFunction(QKeyEvent *event)
{
	GmacsKeyBindFunc func = NULL;
	int idx = event->key() - Qt::Key_A;
	int max = Qt::Key_Z - Qt::Key_A;
	if (0 <= idx && idx < max) {
		if (command_count == 0) {
			func = key_bind_list[idx];
		} else if (command_count == 1) {
			func = command_bind_list[idx];
		} else {
			command_count = 0;
		}
	}
	return func;
}

void GmacsKeyBind::toggleStateDock(QTextCursor *cursor)
{
	(void)cursor;
	emit emitToggleStateDockSignal();
}

void GmacsKeyBind::clearCommand(QTextCursor *cursor)
{
	kill_buf_count = 0;
	command_count = 0;
}

void GmacsKeyBind::moveLineTop(QTextCursor *cursor)
{
	kill_buf_count = 0;
	command_count = 0;
	cursor->movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
}

void GmacsKeyBind::moveLineEnd(QTextCursor *cursor)
{
	kill_buf_count = 0;
	command_count = 0;
	cursor->movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
}

void GmacsKeyBind::moveLeft(QTextCursor *cursor)
{
	kill_buf_count = 0;
	command_count = 0;
	cursor->movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
}

void GmacsKeyBind::moveRight(QTextCursor *cursor)
{
	kill_buf_count = 0;
	command_count = 0;
	cursor->movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
}

void GmacsKeyBind::moveUp(QTextCursor *cursor)
{
	kill_buf_count = 0;
	command_count = 0;
	cursor->movePosition(QTextCursor::Up, QTextCursor::MoveAnchor);
}

void GmacsKeyBind::moveDown(QTextCursor *cursor)
{
	kill_buf_count = 0;
	command_count = 0;
	cursor->movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
}

void GmacsKeyBind::yankText(QTextCursor *cursor)
{
	QTextCharFormat colorFormat = cursor->charFormat();
	colorFormat.setForeground(Qt::white);
	cursor->insertText(yank_buf, colorFormat);
	QTextCharFormat defaultFormat;
	defaultFormat.setForeground(Qt::white);
	cursor->mergeCharFormat(defaultFormat);
	command_count = 0;
}

void GmacsKeyBind::addYankBuffer(QString buf)
{
	if (kill_buf_count == 0) {
		yank_buf = buf;
	} else {
		yank_buf += "\n" + buf;
	}
}

void GmacsKeyBind::killText(QTextCursor *cursor)
{
	if (cursor->movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor)) {
		QString text = cursor->selectedText();
		addYankBuffer(text);
		kill_buf_count++;
		cursor->removeSelectedText();
	} else {
		//line end
		kill_buf_count++;
		cursor->deleteChar();
	}
	command_count = 0;
}

void GmacsKeyBind::deleteChar(QTextCursor *cursor)
{
	(void)cursor;
	cursor->deleteChar();
}

void GmacsKeyBind::startCommand(QTextCursor *cursor)
{
	(void)cursor;
	command_count++;
}

void GmacsKeyBind::findFile(QTextCursor *cursor)
{
	(void)cursor;
	emit emitFindFileSignal();
	command_count = 0;
}
