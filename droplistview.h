#ifndef DROPLISTVIEW_H
#define DROPLISTVIEW_H

#include <QListView>
#include <QDragMoveEvent>

class DropListView : public QListView
{
    Q_OBJECT
public:
    explicit DropListView(QWidget *parent = 0);
    
signals:
    void dropFiles(QDropEvent *e);
    
public slots:

protected:
    void dragMoveEvent(QDragMoveEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
};

#endif // DROPLISTVIEW_H
