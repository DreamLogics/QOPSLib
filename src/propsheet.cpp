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

Propsheet::Propsheet(const Propsheet &ref)
{
#ifndef NO_SMART_POINTERS
    m_p = ref.m_p;
    m_p->iRefCount++;
#else
    m_p = new PropsheetPrivate();
    m_p->objectPropTables = ref.m_p->objectPropTables;
    m_p->m_pInformationProvider = ref.m_p->m_pInformationProvider;
    m_p->sequences = ref.m_p->sequences;
    m_p->m_variables = ref.m_p->m_variables;
#endif
}

Propsheet::Propsheet(InformationProvider *ip)
{
    m_p = new PropsheetPrivate();
    m_p->pInformationProvider = ip;
}

Propsheet::~Propsheet()
{
#ifndef NO_SMART_POINTERS
    if (--m_p->iRefCount <= 0)
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
    Propsheet p(m_p->pInformationProvider);
    p.m_p->variables = m_p->variables;

    //copy tables
    QList<QString> nskeys = m_p->objectPropTables.keys();
    for (int i=0;i<nskeys.size();i++)
    {
        QList<QString> keys = m_p->objectPropTables[nskeys[i]].keys();
        for (int n=0;n<keys.size();n++)
        {
            p.addObjectPropertyTable(m_p->objectPropTables[nskeys[i]][keys[n]].copy(),nskeys[i]);
        }
    }

    //copy sequences
    nskeys = m_p->sequences.keys();
    for (int i=0;i<nskeys.size();i++)
    {
        QList<QString> keys = m_p->sequences[nskeys[i]].keys();
        for (int n=0;n<keys.size();n++)
        {
            p.addSequence(m_p->sequences[nskeys[i]][keys[n]].copy(),nskeys[i]);
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
    if (!m_p->objectPropTables.contains(ns))
        return Table();
    if (!m_p->objectPropTables[ns].contains(id))
        return Table();
    return m_p->objectPropTables[ns][id];
}

/*!
 * \brief Add an object %property %table to the %propsheet.
 * \param t The %property %table.
 * \param ns The namespace.
 */

void Propsheet::addObjectPropertyTable(Table t, QString ns) const
{
    if (!m_p->objectPropTables.contains(ns))
        m_p->objectPropTables.insert(ns,QHash<QString,Table>());

    if (m_p->objectPropTables[ns].contains(t.name()))
        m_p->objectPropTables[ns][t.name()] = t;
    else
        m_p->objectPropTables[ns].insert(t.name(),t);

    t.m_p->pIP = m_p->pInformationProvider;
    QList<Property> props = t.m_p->pProps.values();
#ifndef NO_SMART_POINTERS
    for (int i=0;i<props.size();i++)
        props[i].m_p->pIP = m_p->pInformationProvider;
#else
    for (int i=0;i<props.size();i++)
    {
        Property p = props[i];
        p.m_p->pIP = m_p->pInformationProvider;
        props[i] = p;
    }
#endif

}

/*!
 * \brief Get the %sequence named "id".
 * \param id The id/name of the %sequence.
 * \param ns The namespace.
 * \return The %sequence.
 */

Sequence Propsheet::sequence(QString id, QString ns) const
{
    if (!m_p->sequences.contains(ns))
        return Sequence();
    if (!m_p->sequences[ns].contains(id))
        return Sequence();
    return m_p->sequences[ns][id];
}

/*!
 * \brief Add a %property %sequence.
 * \param seq The %sequence.
 * \param ns The namespace.
 */

void Propsheet::addSequence(Sequence seq, QString ns) const
{
    if (!m_p->sequences.contains(ns))
        m_p->sequences.insert(ns, QHash<QString,Sequence>());

    if (m_p->sequences[ns].contains(seq.name()))
        m_p->sequences[ns][seq.name()] = seq;
    else
        m_p->sequences[ns].insert(seq.name(),seq);

}

/*!
 * \brief Get the variable "varname".
 * \param varname The variable name.
 * \param ns The namespace.
 * \return The variable value as a string.
 */

QString Propsheet::variable(QString varname, QString ns) const
{
    if (!m_p->variables.contains(ns))
        return QString();
    if (!m_p->variables[ns].contains(varname))
        return QString();
    return m_p->variables[ns][varname];
}

/*!
 * \brief Set a variable value.
 * \param varname The variable name.
 * \param value The variable value.
 * \param ns The namespace.
 */

void Propsheet::setVariable(QString varname, QString value, QString ns) const
{
    if (!m_p->variables.contains(ns))
        m_p->variables.insert(ns,QHash<QString,QString>());

    if (m_p->variables[ns].contains(varname))
        m_p->variables[ns][varname] = value;
    else
        m_p->variables[ns].insert(varname,value);
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
PropsheetPrivate::PropsheetPrivate() : iRefCount(1), pInformationProvider(0)
#else
PropsheetPrivate::PropsheetPrivate() : pInformationProvider(0)
#endif
{

}

