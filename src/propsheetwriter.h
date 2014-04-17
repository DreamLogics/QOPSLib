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
#ifndef STYLESHEETWRITER_H
#define STYLESHEETWRITER_H

#include <QByteArray>
#include "propsheet.h"
#include "qopslib_global.h"

namespace QOPS
{

/*!
 * \brief The %PropsheetWriter class is used to
 */

class QOPSLIBSHARED_EXPORT PropsheetWriter
{
public:
    PropsheetWriter();

    QString toString(Propsheet &ps);
    QByteArray toUtf8String(Propsheet &ps);

    int getError() const;

private:

    int m_iErr;
};

}

#endif // STYLESHEETWRITER_H
