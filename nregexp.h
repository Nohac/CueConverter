#ifndef NREGEXP_H
#define NREGEXP_H

#include <QRegExp>
#include <QObject>
#include <QtCore>


class NRegExp : public QRegExp
{
    //Q_OBJECT
public:
    explicit NRegExp(QObject *parent = 0);
    //explicit NRegExp():QRegExp(){}
    explicit NRegExp( const QString & pattern, Qt::CaseSensitivity cs = Qt::CaseSensitive, PatternSyntax syntax = RegExp ):QRegExp(pattern,cs,syntax){}
    explicit NRegExp( const QRegExp & rx ):QRegExp(rx){}

    QStringList pregMatch(QString str);
    
signals:
    
public slots:
    
};

#endif // NREGEXP_H
