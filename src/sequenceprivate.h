/****************************************************************************
**
** Object Property Sheet Library for Qt
** http://www.dreamlogics.com/
**
** Copyright (C) 2014 DreamLogics
** Copyright (C) 2014 Stefan Ladage <sladage@gmail.com>
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
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

    friend class Sequence;
    friend class PropsheetWriter;
};

}

#endif // SEQUENCEPRIVATE_H
