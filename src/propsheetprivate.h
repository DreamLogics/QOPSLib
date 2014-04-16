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
#ifndef STYLESHEETPRIVATE_H
#define STYLESHEETPRIVATE_H

#include <QString>
#include <QHash>
#include <table.h>
#include <sequence.h>

namespace QOPS
{

class InformationProvider;

class PropsheetPrivate
{
public:
    PropsheetPrivate();

private:

#ifndef NO_SMART_POINTERS
    int iRefCount;
#endif

    QHash<QString, QHash<QString,Table> > objectPropTables;
    QHash<QString, QHash<QString,Sequence> > sequences;
    QHash<QString, QHash<QString,QString> > variables;

    InformationProvider *pInformationProvider;

    friend class Propsheet;
    friend class PropsheetReader;
    friend class PropsheetWriter;
};

}

#endif // STYLESHEETPRIVATE_H
