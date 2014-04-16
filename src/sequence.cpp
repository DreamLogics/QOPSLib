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
#include "sequence.h"
#include "sequenceprivate.h"

using namespace QOPS;


Sequence::Sequence(QString name, int start, int end)
{
    m_sName = name;
    m_p = new SequencePrivate();
    m_p->iStart = start;
    m_p->iEnd = end;
}

Sequence::Sequence(const Sequence &ref)
{
#ifndef NO_SMART_POINTERS
    m_p = ref.m_p;
    m_sName = ref.m_sName;
    m_p->iRefCount++;
#else
    m_p = new SequencePrivate();
    m_p->frames = ref.m_p->frames;
    m_p->iStart = ref.m_p->iStart;
    m_p->iEnd = ref.m_p->iEnd;
    m_sName = ref.m_sName;
#endif
}

Sequence::Sequence()
{
    m_p = new SequencePrivate();
}

Sequence::~Sequence()
{
    if ((--m_p->iRefCount) <= 0)
        delete m_p;
}

/*!
 * \brief Give the name of the sequence.
 * \return Name as QString.
 */

QString Sequence::name() const
{
    return m_sName;
}

#ifndef NO_SMART_POINTERS
Sequence Sequence::copy() const
{
    Sequence s(m_sName,m_p->iStart,m_p->iEnd);
    s.m_p->frames = m_p->frames;
    return s;
}
#endif

/*!
 * \brief Start of the range.
 * \return
 */

int Sequence::startIndex() const
{
    return m_p->iStart;
}

/*!
 * \brief End of the range.
 * \return
 */

int Sequence::endIndex() const
{
    return m_p->iEnd;
}

/*!
 * \brief Give the start frame for the object "object_id".
 * \param object_id ID of the object.
 * \return Object %Property %Table
 */

Table Sequence::startFrame(QString object_id) const
{
    if (!m_p->frames.contains(object_id))
        return Table();

    return m_p->frame(object_id,m_p->iStart);
}

/*!
 * \brief Give the end frame for the object "object_id".
 * \param object_id ID of the object.
 * \return Object %Property %Table
 */

Table Sequence::endFrame(QString object_id) const
{
    if (!m_p->frames.contains(object_id))
        return Table();

    return m_p->frame(object_id,m_p->iEnd);
}

/*!
 * \brief Give the frame on index "frame_index" for the object "object_id".
 * \param object_id The object ID.
 * \param frame_index The frame index.
 * \return Object %Property %Table
 */

Table Sequence::frame(QString object_id, int frame_index) const
{
    if (!m_p->frames.contains(object_id))
        return Table();
    return m_p->frame(object_id,frame_index);
}

/*!
 * \brief Sets the frame for object "object_id" on index "index".
 * \param object_id The object ID.
 * \param index The frame index.
 * \param frame The frame as an object %property %table.
 */

void Sequence::setFrame(QString object_id, int index, Table frame) const
{
    if (!m_p->frames.contains(object_id))
        m_p->frames.insert(object_id,QMap<int,Table>());
    if (!m_p->frames[object_id].contains(index))
        m_p->frames[object_id].insert(index,frame);
    m_p->frames[object_id][index] = frame;
}

bool Sequence::operator==(const Sequence&)
{

}

#ifndef NO_SMART_POINTERS
Sequence& Sequence::operator=(const Sequence &other)
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

SequencePrivate::SequencePrivate() : iRefCount(1), iStart(0), iEnd(0)
{

}

Table SequencePrivate::frame(QString id, int i)
{
    if (frames[id].contains(i))
        return frames[id][i];

    //create frame
    int from=iStart,to=iEnd;

    for (int n=i;n>=iStart;n--)
    {
        if (frames[id].contains(n))
        {
            from = n;
            break;
        }
    }

    for (int n=i;n<=iEnd;n++)
    {
        if (frames[id].contains(n))
        {
            to = n;
            break;
        }
    }



}
