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
#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QString>
#include "table.h"
#include "sequence.h"
#include "qopslib_global.h"

namespace QOPS
{

class PropsheetPrivate;
class InformationProvider;

/*!
 * \brief A %property sheet.
 *
 * %Property sheets contain object %property %tables, %property sequences and variables.
 */

class QOPSLIBSHARED_EXPORT Propsheet
{
public:
    Propsheet(InformationProvider *ip=0);
    Propsheet(const Propsheet &ref);
    ~Propsheet();

#ifndef NO_SMART_POINTERS
    Propsheet copy() const;
#endif

    Table objectPropertyTable(QString id, QString ns = QString()) const;
    void addObjectPropertyTable(Table t, QString ns = QString()) const;

    Sequence sequence(QString id, QString ns = QString()) const;
    void addSequence(Sequence seq, QString ns = QString()) const;

    QString variable(QString varname, QString ns = QString()) const;
    void setVariable(QString varname, QString value, QString ns = QString()) const;

#ifndef NO_SMART_POINTERS
    virtual Propsheet& operator=(const Propsheet&);
#endif

private:

    PropsheetPrivate *m_p;

    friend class PropsheetWriter;

};

}

#endif // STYLESHEET_H
