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
#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QString>
#include "table.h"
#include "qopslib_global.h"

namespace QOPS
{

class SequencePrivate;

/*!
 * \brief A %Sequence contains frames (object %property tables) for objects. Sequences can
 * be used as animations or as a way to represent multiple object states.
 */

class QOPSLIBSHARED_EXPORT Sequence
{
public:
    Sequence(QString name, int start, int end);
    Sequence(const Sequence &ref);
    Sequence();
    ~Sequence();

    QString name() const;

    int startIndex() const;
    int endIndex() const;

#ifndef NO_SMART_POINTERS
    Sequence copy() const;
#endif

    Table startFrame(QString object_id) const;
    Table endFrame(QString object_id) const;
    Table frame(QString object_id, int frame_index) const;

    void setFrame(QString object_id, int index, Table frame) const;

    virtual bool operator==(const Sequence&);
#ifndef NO_SMART_POINTERS
    virtual Sequence& operator=(const Sequence&);
#endif

private:

    SequencePrivate *m_p;
    QString m_sName;

    friend class PropsheetWriter;
};
}

#endif // SEQUENCE_H
