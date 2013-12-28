#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDropEvent>
#include <QDir>
#include <QProcess>
#include "droplistview.h"
#include "cuefiles.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void dropFiles(QDropEvent* e);
    void removeCue(QModelIndex e);
    void finished(int s);

private slots:
    void on_pushButton_clicked();
    void on_spinBitrate_valueChanged(int arg1);
    void on_pushStartConvert_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    DropListView *dropView;
    QList<CueFiles*> files;

    QFileInfo ffmpegPath;

    QDir    outputDir;
    int     bitrate;
    int     threads         = 1;
    int     curThreads      = 0;

    int     progress        = 0;
    int     subprogress     = 0;
    int     jobammount;
    int     jobsdone        = 0;
    bool    converting      = false;

    QString convert(int p, int s);
    void stopConvert();
    void setOutput(QString s);
    void assignJobs();
};

#endif // MAINWINDOW_H
