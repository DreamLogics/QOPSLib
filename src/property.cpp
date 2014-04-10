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

Property::Property(Property &ref)
{
    m_p = ref.m_p;
    m_sName = ref.m_sName;
    m_p->iRefCount++;
}

Property::~Property()
{
    if (--m_p->iRefCount <= 0)
        delete m_p;
}

Property Property::copy() const
{
    Property p(m_sName);
    p.m_p->bIsNull = m_p->bIsNull;
    p.m_p->sRule = m_p->sRule;
    p.m_p->sValues = m_p->sValues;
    p.m_sName = m_sName;
    return p;
}

QString Property::name() const
{
    return m_sName;
}

void Property::setValue(QString val, int index) const
{
    m_p->sValues[index] = val;
}
void Property::setValue(int val, int index) const
{
    m_p->sValues[index] = QString::number(val);
}
void Property::setValue(double val, int index) const
{
    m_p->sValues[index] = QString::number(val);
}
void Property::setValue(bool val, int index) const
{
    if (val)
        m_p->sValues[index] = "true";
    else
        m_p->sValues[index] = "false";
}

QString Property::toString(int index) const
{
    if (m_p->pIP)
        return m_p->pIP->valueForProp(m_sName,m_p->sValues[index],m_p->sRule);
    return m_p->sValues[index];
}
int Property::toInt(int index) const
{
    return toString(index).toInt();
}
double Property::toDouble(int index) const
{
    return toString(index).toDouble();
}
bool Property::toBool(int index) const
{
    QString val = toString(index);
    if (val == "1" || val == "true")
        return true;
    return false;
}

bool Property::isNull() const
{
    return m_p->bIsNull;
}

QString Property::rule() const
{
    return m_p->sRule;
}
void Property::setRule(QString rule) const
{
    m_p->sRule = rule;
}

#ifdef USE_GUI
void Property::setValue(QColor val, int index) const
{

}
QColor Property::toColor(int index) const
{

}
#endif
