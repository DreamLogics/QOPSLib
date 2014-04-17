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
#include "propsheetwriter.h"
#include "qopslib.h"
#include "propsheetprivate.h"
#include "tableprivate.h"
#include "sequenceprivate.h"

using namespace QOPS;

PropsheetWriter::PropsheetWriter()
{

}

/*!
 * \brief Creates an Object %Property Sheet document.
 * \param ps The %Propsheet to parse.
 * \return The document as a string.
 */

QString PropsheetWriter::toString(Propsheet &ps)
{
    //get all namespaces
    QString doc;
    QString indent;
    QStringList ns = ps.m_p->variables.keys();
    ns += ps.m_p->objectPropTables.keys();
    ns += ps.m_p->sequences.keys();

    ns.removeDuplicates();
    ns.removeAll(QString());
    ns.push_front(QString());


    for (int i=0;i<ns.size();i++)
    {
        if (i!=0)
        {
            doc += "@namespace " + ns[i] + "\n{\n";
            indent = "    ";
        }

        //vars
        if (ps.m_p->variables.contains(ns[i]))
        {
            QStringList vars = ps.m_p->variables[ns[i]].keys();
            for (int n=0;n<vars.size();n++)
            {
                doc += indent + vars[n] + "=" + ps.m_p->variables[ns[i]][vars[n]] + ";\n";
            }
        }

        //tables
        if (ps.m_p->objectPropTables.contains(ns[i]))
        {
            QStringList tables = ps.m_p->objectPropTables[ns[i]].keys();

            for (int n=0;n<tables.size();n++)
            {
                Table t = ps.objectPropertyTable(tables[n],ns[i]);

                doc += indent + tables[n] + "\n"+indent+"{\n";
                indent += "    ";

                QStringList props = t.m_p->pProps.keys();
                for (int s=0;s<props.size();s++)
                {
                    Property p = t.property(props[s]);
                    doc += indent + props[s] + ": ";
                    for (int r=0;r<p.partCount();r++)
                        doc += p.toString(r);
                    for (int r=0;r<p.rules().size();r++)
                        doc += " !" + p.rules()[r];
                    doc += ";\n";
                }

                indent = indent.left(indent.size()-4);

                doc += indent + "}\n";
            }
        }

        //sequences
        if (ps.m_p->sequences.contains(ns[i]))
        {
            QStringList sequences = ps.m_p->sequences[ns[i]].keys();

            for (int n=0;n<sequences.size();n++)
            {
                Sequence seq = ps.sequence(sequences[n],ns[i]);

                doc += indent + "@sequence " + seq.name() + "(" + QString::number(seq.startIndex()) + "," + QString::number(seq.endIndex()) + ")\n";
                doc += indent + "{\n";

                indent += "    ";

                QStringList objects = seq.m_p->frames.keys();

                for (int s=0;s<objects.size();s++)
                {
                    QList<int> frames = seq.m_p->frames[objects[s]].keys();

                    doc += indent + objects[s] + "\n" + indent + "{\n";
                    indent += "    ";

                    for (int r=0;r<frames.size();r++)
                    {
                        Table t = seq.m_p->frames[objects[s]][frames[r]];
                        QStringList props = t.m_p->pProps.keys();

                        doc += indent + QString::number(frames[r]) + "\n" + indent + "{\n";
                        indent += "    ";

                        for (int ss=0;ss<props.size();ss++)
                        {
                            Property p = t.property(props[ss]);
                            doc += indent + props[ss] + ": ";
                            for (int rr=0;rr<p.partCount();rr++)
                                doc += p.toString(rr);
                            for (int rr=0;rr<p.rules().size();rr++)
                                doc += " !" + p.rules()[rr];
                            doc += ";\n";
                        }

                        indent = indent.left(indent.size()-4);
                        doc += indent + "}\n";
                    }

                    indent = indent.left(indent.size()-4);
                    doc += indent + "}\n";
                }

                indent = indent.left(indent.size()-4);

                doc += indent + "}\n";
            }
        }


        if (i!=0)
            doc += "}\n";
    }

    return doc;
}

/*!
 * \brief Creates an Object %Property Sheet document.
 * \param ps The %Propsheet to parse.
 * \return The document as an Utf8 encoded string.
 */

QByteArray PropsheetWriter::toUtf8String(Propsheet &ps)
{
    return toString(ps).toUtf8();
}

int PropsheetWriter::getError() const
{
    return m_iErr;
}
