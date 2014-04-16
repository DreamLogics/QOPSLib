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
#ifndef OSSINFORMATIONPROVIDER_H
#define OSSINFORMATIONPROVIDER_H

#include <QString>
#include <QStringList>
#include "valuetypes.h"

namespace QOPS
{

/*!
 * \brief The InformationProvider class provides an interface for retreiving
 * application/system specific information.
 */

class InformationProvider
{
public:
    virtual ~InformationProvider() {}

    /*!
     * \brief Get a system defined variable.
     * \param varname The variable name.
     * \param id The object ID.
     * \param ns The namespace.
     * \return Variable value as a string.
     */
    virtual QString sysVarForObject(QString varname, QString id, QString ns) = 0;

    /*!
     * \brief All values are passed trough this function, allowing the information
     * provider to modify it if needed.
     * \param prop The property name.
     * \param value The current property value.
     * \param index The property index.
     * \param rules The property rules.
     * \return New value as a string.
     */
    virtual QString valueForProp(QString prop, QString value, int index, QStringList rules) = 0;

    /*!
     * \brief This function handles importing of %property sheet documents.
     * \param filename The file to import.
     * \return Utf8 encoded string.
     */
    virtual QByteArray importPropsheet(QString filename) = 0;

    /*!
     * \brief Give the value type for the given property.
     * \param prop The property name.
     * \param value The current property value.
     * \param index The property index.
     * \param rules The property rules.
     * \return Returns a ValueType.
     */

    virtual ValueType valueTypeForProp(QString prop, QString value, int index, QStringList rules) = 0;
};

}

#endif // OSSINFORMATIONPROVIDER_H
