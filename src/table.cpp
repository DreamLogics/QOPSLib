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
#include "table.h"
#include "tableprivate.h"
#include "propertyprivate.h"

using namespace QOPS;


Table::Table()
{
    m_p = new TablePrivate();
}

Table::Table(const Table &ref)
{
#ifndef NO_SMART_POINTERS
    m_p = ref.m_p;
    m_sName = ref.m_sName;
    m_p->iRefCount++;
#else
    m_p = new TablePrivate();
    m_sName = ref.m_sName;
    m_p->pIP = ref.m_p->pIP;
    m_p->pProps = ref.m_p->pProps;
#endif
}

Table::Table(QString name)
{
    m_p = new TablePrivate();
    m_sName = name;
}

Table::~Table()
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
 * \brief Creates a copy of the property table. Also copies all contained properties.
 * Parents will not be copied.
 * \return The copy of the table.
 */

Table Table::copy() const
{
    Table t(name());

    QList<QString> keys = m_p->pProps.keys();
    //copy all props
    for (int i=0;i<keys.size();i++)
        t.addProperty(m_p->pProps[keys[i]].copy());

    return t;
}
#endif

/*!
 * \brief Gives the name of the property table.
 * \return The property table name as a string.
 */

QString Table::name() const
{
    return m_sName;
}

/*!
 * \brief Table::addParent
 * \param parent_id
 */

void Table::addParent(Table parent) const
{
    m_p->parents.append(parent);
}

/*!
 * \brief Table::removeParent
 * \param parent_id
 */

void Table::removeParent(Table parent) const
{
    m_p->parents.removeAll(parent);
}

/*!
 * \brief Give the parent tables.
 * \return
 */

QList<Table> Table::parents() const
{
    return m_p->parents;
}

/*!
 * \brief Adds a property to the property table.
 * \param prop The property to add.
 */

void Table::addProperty(Property prop) const
{
    if (prop.name().isNull())
        return;
    m_p->pProps.insert(prop.name(),prop);
    prop.m_p->pIP = m_p->pIP;
}

/*!
 * \brief Gives the property "name".
 * \param name The name of the property.
 * \return The property.
 */

Property Table::property(QString name) const
{
    if (m_p->pProps.contains(name))
        return m_p->pProps[name];
    for (int i=0;i<m_p->parents.size();i++)
    {
        Property p = m_p->parents[i].property(name);
        if (!p.isNull())
            return p;
    }
    return Property();
}

/*!
 * \brief Remove a property.
 * \param name The property name.
 */

void Table::removeProperty(QString name) const
{
    if (!m_p->pProps.contains(name))
        return;
    m_p->pProps.remove(name);
}

/*!
 * \brief Give the properies in this table.
 * \return List of property names as a QStringList.
 */

QStringList Table::properties() const
{
    return m_p->pProps.keys();
}

/*!
 * \brief Merge property table "proptable" with the current table.
 * \param proptable The table to merge with the current one.
 * \param replace_props Whether properties from "proptable" should replace existing properties.
 * \param merge_parents Whether the list of parents should also be merged. (note that when using
 * smart poitners, the parents will link to the original parent tables.)
 */

void Table::merge(Table proptable, bool replace_props, bool merge_parents) const
{
    QList<QString> props = proptable.m_p->pProps.keys();
    for (int i=0;i<props.size();i++)
    {
        if (!m_p->pProps.contains(props[i]))
            m_p->pProps.insert(props[i],proptable.property(props[i]));
        else if (replace_props)
            m_p->pProps[props[i]] = proptable.property(props[i]);
    }
    //also add the parents
    if (merge_parents)
        m_p->parents.append(proptable.m_p->parents);
}

bool Table::operator ==(const Table& other)
{
    return name() == other.name();
}

#ifndef NO_SMART_POINTERS
Table& Table::operator=(const Table &ref)
{
    if (--m_p->iRefCount <= 0)
        delete m_p;
    m_p = ref.m_p;
    m_sName = ref.m_sName;
    m_p->iRefCount++;
    return *this;
}
#endif

/*
 * Private Impl.
 */
#ifndef NO_SMART_POINTERS
TablePrivate::TablePrivate() : iRefCount(1), pIP(0)
#else
TablePrivate::TablePrivate() : pIP(0)
#endif
{

}
