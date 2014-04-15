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
    QStringList ns = ps.m_p->variables.keys();
    ns += ps.m_p->objectPropTables.keys();
    ns += ps.m_p->sequences.keys();

    ns.removeDuplicates();
    ns.removeAll(QString());
    ns.push_front(QString());


    for (int i=0;i<ns.size();i++)
    {
        if (i!=0)
            doc += "@namespace " + ns[i] + "\n{\n";

        //vars
        if (ps.m_p->variables.contains(ns[i]))
        {
            QStringList vars = ps.m_p->variables[ns[i]].keys();
            for (int n=0;n<vars.size();n++)
            {
                doc += vars[n] + "=" + ps.m_p->variables[ns[i]][vars[n]] + ";\n";
            }
        }

        //tables
        if (ps.m_p->objectPropTables.contains(ns[i]))
        {
            QStringList tables = ps.m_p->objectPropTables[ns[i]].keys();

            for (int n=0;n<tables.size();n++)
            {
                Table t = ps.m_p->objectPropTables[ns[i]][tables[n]];

                doc += tables[n] + "\n{\n";

                QStringList props = t.m_p->pProps.keys();
                for (int s=0;s<props.size();s++)
                {
                    Property p = t.property(props[s]);
                    doc += props[s] + ": ";
                    for (int r=0;r<p.partCount();r++)
                        doc += p.toString(r);
                    doc += ";\n";
                }

                doc += "}\n";
            }
        }

        //sequences


        if (i!=0)
            doc += "}\n";
    }
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
