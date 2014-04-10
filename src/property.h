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
#ifndef PROPERTY_H
#define PROPERTY_H

#include <QString>

#ifdef USE_GUI
#include <QColor>
#endif

namespace QOPS
{

class PropertyPrivate;

class Property
{
public:
    Property(QString name);
    Property();
    Property(Property &ref);
    ~Property();

    Property copy() const;

    QString name() const;

    void setValue(QString val, int index=0) const;
    void setValue(int val, int index=0) const;
    void setValue(double val, int index=0) const;
    void setValue(bool val, int index=0) const;

    QString toString(int index=0) const;
    int toInt(int index=0) const;
    double toDouble(int index=0) const;
    bool toBool(int index=0) const;

    bool isNull() const;

    QString rule() const;
    void setRule(QString rule) const;

#ifdef USE_GUI
    void setValue(QColor val, int index=0) const;
    QColor toColor(int index=0) const;
#endif

private:

    PropertyPrivate *m_p;
    QString m_sName;
};

}

#endif // PROPERTY_H
