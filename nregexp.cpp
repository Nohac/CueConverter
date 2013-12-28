#include "nregexp.h"

QStringList NRegExp::pregMatch(QString str)
{
    int pos = 0;
    QStringList list;

    while ((pos = indexIn(str, pos)) != -1) {
        list << cap(1);
        pos += matchedLength();
    }

    return list;
}
