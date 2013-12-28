#include "droplistview.h"
#include <QMimeData>

DropListView::DropListView(QWidget *parent) :
    QListView(parent)
{
    setAcceptDrops(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void DropListView::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->mimeData()->hasUrls())
        e->acceptProposedAction();
}

void DropListView::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
        e->acceptProposedAction();
}

void DropListView::dropEvent(QDropEvent *e)
{
    emit dropFiles(e);
}
