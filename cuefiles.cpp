#include "cuefiles.h"
#include "nregexp.h"
#include <QDebug>

CueFiles::CueFiles(QString s, QWidget *parent) :
    QWidget(parent)
{
    cfile = new QFileInfo(s);
    readCueFile();
}

QList<cueData> CueFiles::getCueData()
{
    return cdata;
}

QString CueFiles::getAlbumName()
{
    return albumName;
}

QString CueFiles::getAlbumPerformer()
{
    return albumPerformer;
}

QFileInfo *CueFiles::getSrcFile()
{
    return srcFile;
}

void CueFiles::readCueFile()
{

    //Read cue file
    QString str;
    QFile cueFile(cfile->filePath());
    if (!cueFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!cueFile.atEnd()) {
        str.append(cueFile.readLine());
    }
    cueFile.close();

    //Setup regex
    NRegExp regTitle("TITLE\\s*\"([^\"]+)\".+"),
            regPerformer("PERFORMER\\s*\"([^\"]+)\".+"),
            regFile("FILE\\s*\"([^\"]+)\".+");

    //Run pregmatch on regex and get the global album info
    albumName       = regTitle.pregMatch(str).at(0);
    albumPerformer  = regPerformer.pregMatch(str).at(0);
    srcFile = new QFileInfo(cfile->path()+"/"+regFile.pregMatch(str).at(0));

    qDebug()<<srcFile->filePath();

    //Read the rest of the track info and add the to the track list
    NRegExp reg("(TRACK.+)");
    QStringList list;
    list = reg.pregMatch(str);

    for (int i = 0; i < list.length(); i++){
        qDebug()<<i;
        QString val = list.value(i);
        NRegExp trackTitle("TITLE\\s*\"([^\"]+)\""),
                trackPerformer("PERFORMER\\s*\"([^\"]+)\""),
                trackTime("INDEX\\s*\\d+\\s*([\\d:]+)\\s*");

        QStringList tmpTime = trackTime.pregMatch(val);
        QStringList tmpTitle = trackTitle.pregMatch(val);
        QStringList tmpPerf = trackPerformer.pregMatch(val);

        for (int j = 0; j < tmpTitle.length(); j++){
            cueData d;
            d.trackNumber = j;
            qDebug()<<"Tracknum: "<<d.trackNumber;
            d.trackName = tmpTitle.value(j);
            d.trackPerformer = tmpPerf.value(j);
            QStringList time = tmpTime.value(j).split(":");
            int seconds = time.at(0).toInt()*60+time.at(1).toInt();
            QTime tmp(0,0,0,0);
            d.start = tmp.addSecs(seconds);

            cdata.push_back(d);
        }
    }
    for (int i = 0; i < cdata.length(); i++){
        if (i<cdata.length()-1){
            QTime ct = cdata.at(i).start;
            QTime nt = cdata.at(i+1).start;
            QTime empt(0,0,0,0);

            int ntime = ct.secsTo(nt);
            cueData newCue = cdata.value(i);
            newCue.stop = empt.addSecs(ntime);
            cdata.replace(i,newCue);
            qDebug()<<cdata.at(i).start<<cdata.at(i).stop;
        }else
            qDebug()<<cdata.at(i).start<<cdata.at(i).stop;
    }
}
