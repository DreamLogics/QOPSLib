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
#ifndef TABLE_H
#define TABLE_H

#include <QString>
#include "property.h"
#include "qopslib_global.h"

namespace QOPS
{

class TablePrivate;


/*!
 * \brief %Property table class.
 */

class QOPSLIBSHARED_EXPORT Table
{
public:
    Table();
    Table(const Table &ref);
    Table(QString name);
    ~Table();
#ifndef NO_SMART_POINTERS
    Table copy() const;
#endif
    QString name() const;

    void addProperty(Property prop) const;
    Property property(QString name) const;
    void removeProperty(QString name) const;

    void merge(Table proptable, bool replace_props = true) const;

    virtual bool operator==(const Table&);
#ifndef NO_SMART_POINTERS
    virtual Table& operator=(const Table&);
#endif

private:

    TablePrivate *m_p;
    QString m_sName;

    friend class Propsheet;
    friend class PropsheetWriter;

};
}
#endif // TABLE_H
