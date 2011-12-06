#include <gmacs.hpp>

GmacsLineNumberArea::GmacsLineNumberArea(GmacsTextField *editor) : QWidget(editor)
{
	edit_area = editor;
}

QSize GmacsLineNumberArea::sizeHint(void) const
{
	return QSize(edit_area->lineNumberAreaWidth(), 0);
}

void GmacsLineNumberArea::paintEvent(QPaintEvent *event)
{
	edit_area->lineNumberAreaPaintEvent(event);
}
