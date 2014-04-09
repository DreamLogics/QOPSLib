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
#include "propsheet.h"
#include "propsheetprivate.h"

using namespace QOPS;

Propsheet::Propsheet()
{
    m_p = new PropsheetPrivate();
}

Propsheet::Propsheet(Propsheet &ref)
{
    m_p = ref.m_p;
    m_p->m_iRefCount++;
}

Propsheet::~Propsheet()
{
    if (--m_p->m_iRefCount <= 0)
        delete m_p;
}

Table Propsheet::objectPropertyTable(QString id, QString ns) const
{
    if (!m_p->m_objectPropTables.contains(ns))
        return Table();
    if (!m_p->m_objectPropTables[ns].contains(id))
        return Table();
    m_p->m_objectPropTables[ns][id];
}

void Propsheet::addObjectPropertyTable(QString id, Table t, QString ns) const
{
    if (!m_p->m_objectPropTables.contains(ns))
        m_p->m_objectPropTables.insert(ns);

    if (m_p->m_objectPropTables[ns].contains(id))
        m_p->m_objectPropTables[ns][id] = t;
    else
        m_p->m_objectPropTables[ns].insert(id) = t;

    m_p->bIsNull = false;
}

Sequence Propsheet::sequence(QString id, QString ns) const
{
    if (!m_p->m_sequences.contains(ns))
        return Sequence();
    if (!m_p->m_sequences[ns].contains(id))
        return Sequence();
    m_p->m_sequences[ns][id];
}

void Propsheet::addSequence(QString id, Sequence seq, QString ns) const
{
    if (!m_p->m_sequences.contains(ns))
        m_p->m_sequences.insert(ns);

    if (m_p->m_sequences[ns].contains(id))
        m_p->m_sequences[ns][id] = seq;
    else
        m_p->m_sequences[ns].insert(id) = seq;

    m_p->bIsNull = false;
}

QString Propsheet::variable(QString varname, QString ns) const
{
    if (!m_p->m_variables.contains(ns))
        return QString();
    if (!m_p->m_variables[ns].contains(id))
        return QString();
    m_p->m_variables[ns][id];
}
void Propsheet::setVariable(QString varname, QString value, QString ns) const
{
    if (!m_p->m_variables.contains(ns))
        m_p->m_variables.insert(ns);

    if (m_p->m_variables[ns].contains(varname))
        m_p->m_variables[ns][varname] = value;
    else
        m_p->m_variables[ns].insert(varname) = value;

    m_p->bIsNull = false;
}


/*
 * Private Impl.
 */

PropsheetPrivate::PropsheetPrivate() : m_iRefCount(1), m_pInformationProvider(0), bIsNull(true)
{

}

