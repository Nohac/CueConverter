#ifndef CUEFILES_H
#define CUEFILES_H

#include <QWidget>
#include <QFileInfo>
#include <QRegExp>
#include <QFile>
#include <QTime>

struct cueData{
    int     trackNumber;
    QTime   start;
    QTime   stop;
    QString trackName;
    QString trackPerformer;
};

class CueFiles : public QWidget
{
    Q_OBJECT
public:
    explicit CueFiles(QString s, QWidget *parent = 0);
    QList<cueData> getCueData();

    QString getAlbumName();
    QString getAlbumPerformer();
    QFileInfo *getSrcFile();

private:
    QString         albumName;
    QString         albumPerformer;

    //tta sound file
    QFileInfo       *srcFile;
    //cue file
    QFileInfo       *cfile;
    //Cue file data
    QList<cueData>  cdata;
    
    void readCueFile();
signals:
    
public slots:
    
};

#endif // CUEFILES_H
