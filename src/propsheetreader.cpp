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
#include "propsheetreader.h"
#include "propsheetreaderprivate.h"
#include "opsinformationprovider.h"

using namespace QOPS;


PropsheetReader::PropsheetReader(InformationProvider *ip=0)
{
    m_p = new PropsheetReaderPrivate();
    m_p->pIP = ip;
}

PropsheetReader::PropsheetReader(PropsheetReader &ref)
{
    m_p = ref.m_p;
    m_p->iRefCount++;
}

PropsheetReader::~PropsheetReader()
{
    if (--m_p->iRefCount <= 0)
        delete m_p;
}

Propsheet PropsheetReader::fromData(QByteArray &data) const
{
    Propsheet p;
    m_p->load(QString::fromUtf8(data),p);
    return p;
}
Propsheet PropsheetReader::fromString(QString str) const
{
    Propsheet p;
    m_p->load(str,p);
    return p;
}

int PropsheetReader::getError() const
{
    return m_p->iErrorCode;
}

/*
 * Private Impl.
 */

PropsheetReaderPrivate::PropsheetReaderPrivate()
{

}

void PropsheetReaderPrivate::load(QString data, Propsheet &propsheet)
{
    //preprocess
    data.replace(QRegExp("[\n\r\t]+"),"");

    if (pIP)
    {
        //process @import
        import(data);
    }

    int level = 0;
    QChar c;
    QString temp;
    QString ns;
    QString name;
    Table table;

    mode cmode = mode_base;
    mode pmode;

    for (int i=0;i<data.size();i++)
    {
        c = data[i];
        if (cmode == mode_base)
        {
            if (c == '$')
            {
                ns = QString();
                pmode = mode_base;
                cmode = mode_in_var;
                temp = "";
            }
            else if (c == '@')
            {
                ns = QString();
                pmode = mode_base;
                cmode = mode_in_atrule;
                temp = "";
            }
            else if (c == '}')
            {
                iErrorCode = QOPS_ERR_PARSE;
                return;
            }
            else if (c == '{')
            {
                //object prop table found
                table = Table();
                temp.replace(" ","");
                propsheet.addObjectPropertyTable(temp,table);
                pmode = mode_base;
                cmode = mode_in_objproptable;
            }
            else
                temp += c;

        }
        else if (cmode == mode_in_objproptable)
        {
            if (c == '}')
            {
                parseProps(temp,table);
                temp = "";
                cmode = pmode;
            }
            else if (c == '{')
            {
                iErrorCode = QOPS_ERR_PARSE;
                return;
            }
            else
                temp += c;
        }
        else if (cmode == mode_in_ns)
        {

        }
        else if (cmode == mode_in_seq)
        {

        }
        else if (cmode == mode_in_var)
        {
            if (c == ';')
            {
                parseVar(temp,propsheet,ns);
                cmode = pmode;
            }
            else
                temp += c;
        }
        else if (cmode == mode_in_atrule)
        {
            if (c == '{')
            {
                if (temp.startsWith("namespace"))
                {
                    temp.replace(" ","");
                    temp = temp.mid(9);
                    ns = temp;
                    temp = "";
                    cmode = mode_in_ns;
                }
                else if (temp.startsWith("sequence"))
                {
                    temp.replace(" ","");
                    temp = temp.mid(8);
                    name = temp;
                    temp = "";
                    cmode = mode_in_seq;
                }
                else
                {
                    iErrorCode = QOPS_ERR_PARSE;
                    return;
                }
            }
            else if (c == '}')
            {
                iErrorCode = QOPS_ERR_PARSE;
                return;
            }
            else
                temp += c;
        }

    }

}

void PropsheetReaderPrivate::parseProps(QString data, Table &table)
{

}
