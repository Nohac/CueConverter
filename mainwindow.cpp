#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStringListModel>
#include <QMimeData>
#include <QFileDialog>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dropView = new DropListView();
    outputDir.setPath(outputDir.homePath());
    ui->lineOutput->setText(outputDir.path());
    bitrate = ui->spinBitrate->value();
    ffmpegPath.setFile("ffmpeg");

    ui->splitter->insertWidget(0,dropView);

    connect(dropView,SIGNAL(dropFiles(QDropEvent*)),this,SLOT(dropFiles(QDropEvent*)));
    connect(dropView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(removeCue(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dropFiles(QDropEvent *e)
{
    qDebug()<<e->mimeData()->urls();
    foreach(QUrl u, e->mimeData()->urls()){
        files.append(new CueFiles(u.toLocalFile()));
    }
    QStringList strList;
    jobammount = 0;
    foreach(CueFiles* f, files){
        jobammount+=f->getCueData().length();
        strList.append(f->getAlbumName());
    }
    ui->progressBar->setMaximum(jobammount);
    ui->progressBar->setValue(0);
    dropView->setModel(new QStringListModel(strList));
}

void MainWindow::removeCue(QModelIndex e)
{
    qDebug()<<files.at(e.row())->getAlbumName();
    jobammount-=files.at(e.row())->getCueData().length();
    files.removeAt(e.row());
    dropView->model()->removeRow(e.row());
}

void MainWindow::finished(int s)
{
    curThreads--;
    if (converting == true){
        jobsdone++;
        ui->progressBar->setValue(jobsdone);
    }
    if (jobsdone==jobammount){
        setOutput("DONE!");
        stopConvert();
        return;
    }

    assignJobs();
}

void MainWindow::on_pushButton_clicked()
{
    QString tmp = QFileDialog::getExistingDirectory();
    if (!tmp.isEmpty()){
        outputDir.setPath(tmp);
        ui->lineOutput->setText(outputDir.path());
    }
}

void MainWindow::on_spinBitrate_valueChanged(int arg1)
{
    bitrate = arg1;
}

void MainWindow::on_pushStartConvert_clicked()
{
    if (converting == false && files.length()>0){
        converting = true;
        ui->pushStartConvert->setText("Stop!");
        dropView->setEnabled(false);
        assignJobs();
        return;
    }else{
        stopConvert();
    }
}

QString MainWindow::convert(int p, int s)
{
    if (converting == true){
        qDebug()<<"Prog: "<<p<<" sub: "<<s;

        QStringList args;
        CueFiles *cfiles    = files.value(p);
        QString srcTta      = cfiles->getSrcFile()->filePath();

        QDir ttaPath(cfiles->getSrcFile()->path());
        QString albumPath = ttaPath.dirName();
        ttaPath.cdUp();
        QString circlePath = ttaPath.dirName();

        QDir circleDir(outputDir.path() + "/" + circlePath);
        if (!circleDir.exists())
            circleDir.mkdir(circleDir.path());

        QDir newPath(circleDir.path()+"/"+albumPath);
        if (!newPath.exists())
            newPath.mkdir(newPath.path());

        cueData data        = cfiles->getCueData().value(s);
        QString start       = data.start.toString("hh:mm:ss.00");

        QString duration    = data.stop.toString("hh:mm:ss.00");

        args << ffmpegPath.filePath();
        if (!data.start.minute()==0 && !data.start.second()==0)
            args    << "-ss" << start;

        args    << "-i" << QString("\"%1\"").arg(srcTta);

        //Metadata tags
        args    << "-metadata" << QString("album=\"%1\"").arg(files.value(p)->getAlbumName());
        args    << "-metadata" << QString("author=\"%1\"").arg(files.value(p)->getAlbumPerformer());
        args    << "-metadata" << QString("artist=\"%1\"").arg(files.value(p)->getAlbumPerformer());
        args    << "-metadata" << QString("track=\"%1\"").arg(QString::number(data.trackNumber));
        args    << "-metadata" << QString("title=\"%1\"").arg(data.trackName);

        if (!data.stop.isNull())
            args    << "-t" << duration;

        args    << "-acodec" << "libmp3lame"
                << "-ab " << QString("%1k").arg(QString::number(bitrate))
                << QString("\"%1/%2.mp3\"").arg(newPath.path()).arg(data.trackName)
                << ((ui->checkOverwrite->checkState()==Qt::Checked)?"-y":"-n");

        QString strout;
        foreach(QString s,args){
            strout.append(s).append(" ");

        }
        setOutput(strout);
        return strout;
    }
}

void MainWindow::stopConvert()
{
    ui->pushStartConvert->setText("Start convertion!");
    progress = 0;
    subprogress = 0;
    converting = false;
    dropView->setEnabled(true);
    curThreads = 0;
    jobsdone = 0;
    //ui->progressBar->setValue(0);
}

void MainWindow::setOutput(QString s)
{
    ui->outputView->append(s);
}

void MainWindow::assignJobs()
{
    if (converting == true){
        for (int i = curThreads; i < threads; i++){
            if (progress < files.length()){
                QProcess *pro = new QProcess();
                pro->start(convert(progress,subprogress));
                connect(pro,SIGNAL(finished(int)),this,SLOT(finished(int)));
                curThreads++;
            }
            if (progress<files.length()){
                if (subprogress<files.value(progress)->getCueData().length()-1)
                    subprogress++;
                else{
                    subprogress = 0;
                    progress++;
                }
            }

        }
    }
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    threads = arg1;
}

void MainWindow::on_pushButton_2_clicked()
{
    QString tmp = QFileDialog::getOpenFileName();
    if (!tmp.isEmpty()){
        ffmpegPath.setFile(tmp);
        ui->lineFfmpeg->setText(ffmpegPath.filePath());
    }
}
