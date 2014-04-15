#ifndef SEQUENCEPRIVATE_H
#define SEQUENCEPRIVATE_H

#include <QMap>
#include "table.h"

namespace QOPS
{

class SequencePrivate
{
public:
    SequencePrivate();

private:

    int iRefCount;

    int iStart;
    int iEnd;

    Table frame(QString id,int i);

    QMap<QString, QMap<int,Table> > frames;
};

}

#endif // SEQUENCEPRIVATE_H
