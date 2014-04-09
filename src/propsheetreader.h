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

#define QOPS_NO_ERR 0
#define QOPS_ERR_FILE 1 //could not open file
#define QOPS_ERR_PARSE 2 //parsing error

namespace QOPS
{

class InformationProvider;
class PropsheetReaderPrivate;

class PropsheetReader
{
public:
    PropsheetReader(InformationProvider *ip=0);
    PropsheetReader(PropsheetReader &ref);
    ~PropsheetReader();

    Propsheet fromData(QByteArray &data) const;
    Propsheet fromString(QString str) const;

    int getError() const;

private:

    PropsheetReaderPrivate *m_p;

};

}

#endif // PROPSHEETREADER_H
