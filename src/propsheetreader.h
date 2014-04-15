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
#ifndef PROPSHEETREADER_H
#define PROPSHEETREADER_H

#include "propsheet.h"
#include <QByteArray>

namespace QOPS
{

class InformationProvider;
class PropsheetReaderPrivate;

/*!
 * \brief The %PropsheetReader class is used to parse an Object %Property Sheet document.
 */

class PropsheetReader
{
public:
    PropsheetReader(InformationProvider *ip=0);
    PropsheetReader(PropsheetReader &ref);
    ~PropsheetReader();

    Propsheet fromUtf8String(QByteArray &data) const;
    Propsheet fromString(QString str) const;

    int getError() const;

private:

    PropsheetReaderPrivate *m_p;

};

}

#endif // PROPSHEETREADER_H
