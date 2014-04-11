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
#include "tableprivate.h"
#include "propertyprivate.h"

using namespace QOPS;

Propsheet::Propsheet()
{
    m_p = new PropsheetPrivate();
}

Propsheet::Propsheet(Propsheet &ref)
{
#ifndef NO_SMART_POINTERS
    m_p = ref.m_p;
    m_p->m_iRefCount++;
#else
    m_p = new PropsheetPrivate();
    m_p->m_objectPropTables = ref.m_p->m_objectPropTables;
    m_p->m_pInformationProvider = ref.m_p->m_pInformationProvider;
    m_p->m_sequences = ref.m_p->m_sequences;
    m_p->m_variables = ref.m_p->m_variables;
#endif
}

Propsheet::Propsheet(InformationProvider *ip)
{
    m_p = new PropsheetPrivate();
    m_p->m_pInformationProvider = ip;
}

Propsheet::~Propsheet()
{
#ifndef NO_SMART_POINTERS
    if (--m_p->m_iRefCount <= 0)
        delete m_p;
#else
    delete m_p;
#endif
}

#ifndef NO_SMART_POINTERS
/*!
 * \brief Create a copy of this %property sheet.
 * \return The copy.
 */
Propsheet Propsheet::copy() const
{
    Propsheet p(m_p->m_pInformationProvider);
    p.m_p->m_variables = m_p->m_variables;

    //copy tables
    QList<QString> nskeys = m_p->m_objectPropTables.keys();
    for (int i=0;i<nskeys.size();i++)
    {
        QList<QString> keys = m_p->m_objectPropTables[nskeys[i]].keys();
        for (int n=0;n<keys.size();n++)
        {
            p.addObjectPropertyTable(m_p->m_objectPropTables[nskeys[i]][keys[n]].copy(),nskeys[i]);
        }
    }

    //copy sequences
    nskeys = m_p->m_sequences.keys();
    for (int i=0;i<nskeys.size();i++)
    {
        QList<QString> keys = m_p->m_sequences[nskeys[i]].keys();
        for (int n=0;n<keys.size();n++)
        {
            p.addSequence(m_p->m_sequences[nskeys[i]][keys[n]].copy(),nskeys[i]);
        }
    }

    return p;
}
#endif

/*!
 * \brief Give the object %property %table named "id".
 * \param id The id/name of the %property %table.
 * \param ns The namespace.
 * \return A %property %table.
 */

Table Propsheet::objectPropertyTable(QString id, QString ns) const
{
    if (!m_p->m_objectPropTables.contains(ns))
        return Table();
    if (!m_p->m_objectPropTables[ns].contains(id))
        return Table();
    m_p->m_objectPropTables[ns][id];
}

/*!
 * \brief Propsheet::addObjectPropertyTable
 * \param t
 * \param ns
 */

void Propsheet::addObjectPropertyTable(Table t, QString ns) const
{
    if (!m_p->m_objectPropTables.contains(ns))
        m_p->m_objectPropTables.insert(ns);

    if (m_p->m_objectPropTables[ns].contains(t.name()))
        m_p->m_objectPropTables[ns][t.name()] = t;
    else
        m_p->m_objectPropTables[ns].insert(t.name()) = t;

    t.m_p->pIP = m_p->pInformationProvider;
    QList<Property> props = t.m_p->pProps.values();
    for (int i=0;i<props.size();i++)
        props[i].m_p->pIP = m_p->pInformationProvider;
//TODO: maken voor NO_SMART_POITNER

}

/*!
 * \brief Propsheet::sequence
 * \param id
 * \param ns
 * \return
 */

Sequence Propsheet::sequence(QString id, QString ns) const
{
    if (!m_p->m_sequences.contains(ns))
        return Sequence();
    if (!m_p->m_sequences[ns].contains(id))
        return Sequence();
    m_p->m_sequences[ns][id];
}

/*!
 * \brief Propsheet::addSequence
 * \param seq
 * \param ns
 */

void Propsheet::addSequence(Sequence seq, QString ns) const
{
    if (!m_p->m_sequences.contains(ns))
        m_p->m_sequences.insert(ns);

    if (m_p->m_sequences[ns].contains(id))
        m_p->m_sequences[ns][id] = seq;
    else
        m_p->m_sequences[ns].insert(id) = seq;

}

/*!
 * \brief Propsheet::variable
 * \param varname
 * \param ns
 * \return
 */

QString Propsheet::variable(QString varname, QString ns) const
{
    if (!m_p->variables.contains(ns))
        return QString();
    if (!m_p->variables[ns].contains(id))
        return QString();
    m_p->variables[ns][id];
}

/*!
 * \brief Propsheet::setVariable
 * \param varname
 * \param value
 * \param ns
 */

void Propsheet::setVariable(QString varname, QString value, QString ns) const
{
    if (!m_p->variables.contains(ns))
        m_p->variables.insert(ns);

    if (m_p->variables[ns].contains(varname))
        m_p->variables[ns][varname] = value;
    else
        m_p->variables[ns].insert(varname) = value;
}

#ifndef NO_SMART_POINTERS
Propsheet& Propsheet::operator=(const Propsheet &ref)
{
    if (--m_p->iRefCount <= 0)
        delete m_p;
    m_p = ref.m_p;
    m_p->iRefCount++;
    return *this;
}
#endif


/*
 * Private Impl.
 */

#ifndef NO_SMART_POINTERS
PropsheetPrivate::PropsheetPrivate() : m_iRefCount(1), m_pInformationProvider(0)
#else
PropsheetPrivate::PropsheetPrivate() : m_pInformationProvider(0)
#endif
{

}

