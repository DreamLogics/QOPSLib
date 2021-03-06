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
#ifndef PROPSHEETREADERPRIVATE_H
#define PROPSHEETREADERPRIVATE_H

#include <QString>
#include "propsheet.h"

namespace QOPS
{

class PropsheetReaderPrivate
{
public:
    PropsheetReaderPrivate();

private:

    enum mode
    {
        mode_base = 0,
        mode_in_objproptable,
        mode_in_var,
        mode_in_seq,
        mode_in_ns,
        mode_in_atrule,
        mode_literal,
        mode_in_frame,
        mode_in_obj
    };

    void load(QString data, Propsheet &propsheet);
    void import(QString &data);
    void parseProps(QString data, Table &table, int lines);
    void parseVar(QString data, Propsheet &propsheet, QString ns);
    void parseSeq(QString data, Sequence &seq, int lines);

    int countLines(QString &data, int end);

    int iErrorCode;
    QString sErrorMsg;

    InformationProvider *pIP;

    friend class PropsheetReader;
};

}

#endif // PROPSHEETREADERPRIVATE_H
