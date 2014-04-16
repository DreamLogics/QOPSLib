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
#include "property.h"
#include "propertyprivate.h"
#include "opsinformationprovider.h"
#include "propsheet.h"

using namespace QOPS;

Property::Property(QString name)
{
    m_p = new PropertyPrivate();
    m_sName = name;
}

Property::Property()
{
    m_p = new PropertyPrivate();
}

Property::Property(const Property &ref)
{
#ifndef NO_SMART_POINTERS
    m_p = ref.m_p;
    m_sName = ref.m_sName;
    m_p->iRefCount++;
#else
    m_p = new PropertyPrivate();
    m_p->bIsNull = ref.m_p->bIsNull;
    m_p->sRule = ref.m_p->sRule;
    m_p->sValues = ref.m_p->sValues;
    m_p->pIP = ref.m_p->pIP;
    m_sName = ref.m_sName;
#endif
}

Property::~Property()
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
 * \brief Creates a copy of the current property.
 * \return Copy of current property.
 */

Property Property::copy() const
{
    Property p(m_sName);
    p.m_p->bIsNull = m_p->bIsNull;
    p.m_p->sRules = m_p->sRules;
    p.m_p->sValues = m_p->sValues;
    p.m_sName = m_sName;
    return p;
}
#endif
/*!
 * \brief Returns the property name.
 * \return Property name as string.
 */

QString Property::name() const
{
    return m_sName;
}

/*!
 * \brief Sets the value of the property.
 * \param val The value as a string.
 * \param index Multipart values may be set using an index. Omit for singlepart values.
 */

void Property::setValue(QString val, int index) const
{
    m_p->sValues[index] = val;
    m_p->bIsNull = val.isNull();
}

/*!
 * \brief Sets the value of the property.
 * \param val The value as an integer.
 * \param index Multipart values may be set using an index. Omit for singlepart values.
 */

void Property::setValue(int val, int index) const
{
    m_p->sValues[index] = QString::number(val);
    m_p->bIsNull = false;
}

/*!
 * \brief Sets the value of the property.
 * \param val The value as a double.
 * \param index Multipart values may be set using an index. Omit for singlepart values.
 */

void Property::setValue(double val, int index) const
{
    m_p->sValues[index] = QString::number(val);
    m_p->bIsNull = false;
}

/*!
 * \brief Sets the value of the property.
 * \param val The value as a boolean.
 * \param index Multipart values may be set using an index. Omit for singlepart values.
 */

void Property::setValue(bool val, int index) const
{
    if (val)
        m_p->sValues[index] = "true";
    else
        m_p->sValues[index] = "false";
    m_p->bIsNull = false;
}

/*!
 * \brief Gives the property value as a string.
 * \param index If the property has a multipart value, use the index to retreive the correct part. Omit for singlepart values.
 * \return Property value as a string.
 */

QString Property::toString(int index) const
{
    if (!m_p->sValues.contains(index))
        return QString();
    if (m_p->pIP)
        return m_p->pIP->valueForProp(m_sName,m_p->sValues[index],index,m_p->sRules);
    return m_p->sValues[index];
}

/*!
 * \brief Gives the property value as an integer.
 * \param index If the property has a multipart value, use the index to retreive the correct part. Omit for singlepart values.
 * \return Property value as an integer.
 */

int Property::toInt(int index) const
{
    return toString(index).toInt();
}

/*!
 * \brief Gives the property value as a double.
 * \param index If the property has a multipart value, use the index to retreive the correct part. Omit for singlepart values.
 * \return Property value as a double.
 */

double Property::toDouble(int index) const
{
    return toString(index).toDouble();
}

/*!
 * \brief Gives the property value as a boolean.
 * \param index If the property has a multipart value, use the index to retreive the correct part. Omit for singlepart values.
 * \return Property value as a boolean.
 */

bool Property::toBool(int index) const
{
    QString val = toString(index);
    if (val == "1" || val == "true")
        return true;
    return false;
}

/*!
 * \brief Gives the amount of parts.
 * \return %Property part count.
 */

int Property::partCount() const
{
    return m_p->sValues.size();
}

/*!
 * \brief Checks if the property has a value set. (an empty string is still a valid value)
 * \return True when null.
 */

bool Property::isNull() const
{
    return m_p->bIsNull;
}

/*!
 * \brief Gives the property rules.
 * \return Property rules as a QStringList.
 */

QStringList Property::rules() const
{
    return m_p->sRules;
}

/*!
 * \brief Checks if the property has the given rule.
 * \param rule The rule to check for.
 * \return True if the property has the rule.
 */

bool Property::hasRule(QString rule) const
{
    return m_p->sRules.contains(rule);
}

/*!
 * \brief Adds a property rule.
 * \param rule The propery rule as a string.
 */

void Property::addRule(QString rule) const
{
    if (m_p->sRules.contains(rule))
        return;
    m_p->sRules.append(rule);
}

/*!
 * \brief Removes the rule from the property.
 * \param rule The rule to remove.
 */

void Property::removeRule(QString rule) const
{
    m_p->sRules.removeAll(rule);
}

/*!
 * \brief The ValueType for the property. If no InformationProvider is used, vtString is returned. Returns vtUndefined when an invalid index is provided.
 * \param index If the property has a multipart value, use the index to target the correct part. Omit for singlepart values.
 * \return Value type as ValueType.
 */

ValueType Property::valueType(int index) const
{
    if (!m_p->sValues.contains(index))
        return vtUndefined;
    if (m_p->pIP)
        return m_p->pIP->valueTypeForProp(m_sName,m_p->sValues[index],index,m_p->sRules);
    return vtString;
}

#ifdef USE_GUI

/*!
 * \brief Sets the value of the property.
 * \param val The value as a QColor object.
 * \param index Multipart values may be set using an index. Omit for singlepart values.
 */

void Property::setValue(QColor val, int index) const
{
    //rgba(#,#,#,#)
    m_p->sValues[index] = "rgba(" + QString::number(val.red()) + "," + QString::number(val.green()) + "," + QString::number(val.blue()) + "," + QString::number(val.alpha()) + ")";
}

/*!
 * \brief Gives the property value as a QColor object.
 * \param index If the property has a multipart value, use the index to retreive the correct part. Omit for singlepart values.
 * \return Property value as a QColor object.
 */

QColor Property::toColor(int index) const
{
    QRegExp rgbareg("rgba\\(([0-9]+),([0-9]+),([0-9]+),([0-9]+)\\)");
    QRegExp rgbreg("rgb\\(([0-9]+),([0-9]+),([0-9]+)\\)");
    QString val = toString(index);
    QColor c;
    if (rgbareg.indexIn(val) != -1)
    {
        c.setRed(rgbareg.cap(1).toInt());
        c.setGreen(rgbareg.cap(2).toInt());
        c.setBlue(rgbareg.cap(3).toInt());
        c.setAlpha(rgbareg.cap(4).toInt());
    }
    else if (rgbreg.indexIn(val) != -1)
    {
        c.setRed(rgbreg.cap(1).toInt());
        c.setGreen(rgbreg.cap(2).toInt());
        c.setBlue(rgbreg.cap(3).toInt());
        c.setAlpha(255);
    }
    return c;
}
#endif

/*!
 * \brief Checks if the properties have the same name.
 * \param other Property to check againts.
 * \return name() == other.name()
 */

bool Property::operator ==(const Property& other)
{
    return name() == other.name();
}
#ifndef NO_SMART_POINTERS
Property& Property::operator=(const Property &other)
{
    if (--m_p->iRefCount <= 0)
        delete m_p;
    m_p = other.m_p;
    m_sName = other.m_sName;
    m_p->iRefCount++;
    return *this;
}
#endif
/*
 * Private Impl.
 */
#ifndef NO_SMART_POINTERS
PropertyPrivate::PropertyPrivate() : iRefCount(1), pIP(0), bIsNull(true)
#else
PropertyPrivate::PropertyPrivate() : pIP(0), bIsNull(true)
#endif
{

}
