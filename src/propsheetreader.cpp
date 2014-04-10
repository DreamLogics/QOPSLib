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
#include <QList>
#include <QRegExp>

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

    //TODO: make all the magic happen in 1 run instead of 2?

    int level = 0;
    QString nd;
    bool bLC = false;
    bool bSC = false;
    bool bLit = false;

    for (int i=0;i<data.size();i++) // remove comments and check brackets
    {

        if (!bLC && !bSC && !bLit)
        {
            if (data[i] == '"')
            {
                bLit = true;
                nd += data[i];
            }
            else if (data.mid(i,2) == "//")
                bLC = true;
            else if (data.mid(i,2) == "/*")
                bSC = true;
            else
            {
                if (data[i] == '{')
                    level++;
                else if (data[i] == '}')
                    level--;
                nd += data[i];
            }
        }
        else if (bLC)
        {
            if (data[i] == '\n' || data[i] == '\r')
                bLC = false;
        }
        else if (bSC)
        {
            if (data.mid(i,2) == "*/")
                bSC = false;
        }
        else if (bLit)
        {
            if (data[i] == '"')
                bLit = false;
            nd += data[i];
        }
    }

    if (level != 0) //too few or too many brackets
    {
        iErrorCode = QOPS_ERR_PARSE;
        return;
    }

    data = nd;

    data.replace(QRegExp("[\n\r\t]+"),"");

    if (pIP)
    {
        //process @import
        import(data);
    }


    QChar c;
    QString temp;
    QString ns;
    QString name;
    Table table;
    Sequence seq;

    mode cmode = mode_base;
    //mode pmode;
    QList<mode> pmode;

    for (int i=0;i<data.size();i++)
    {
        c = data[i];
        if (cmode == mode_base)
        {
            if (c == '$') // variable
            {
                pmode.push_front(cmode);
                cmode = mode_in_var;
                temp = "";
            }
            else if (c == '@') // at-rule
            {
                pmode.push_front(cmode);
                cmode = mode_in_atrule;
                temp = "";
            }
            else if (c == '}') // } found before {, error
            {
                iErrorCode = QOPS_ERR_PARSE;
                return;
            }
            else if (c == '{') // not in at-rule, must be prop table
            {
                //object prop table found
                table = Table();
                temp.replace(" ","");
                propsheet.addObjectPropertyTable(temp,table);
                pmode.push_front(cmode);
                cmode = mode_in_objproptable;
            }
            else
                temp += c; // temp will contain the identifier for the prop table

        }
        else if (cmode == mode_in_objproptable)
        {
            if (c == '"') // literal mode on
            {
                pmode.push_front(cmode);
                cmode = mode_literal;
                temp += c;
            }
            else if (c == '}') // prop table closing tag found, parse props and go back to previous level
            {
                parseProps(temp,table);
                temp = "";
                cmode = pmode.front();
                pmode.pop_front();
            }
            else if (c == '{') // prop table can't contain {, error
            {
                iErrorCode = QOPS_ERR_PARSE;
                return;
            }
            else
                temp += c;
        }
        else if (cmode == mode_literal)
        {
            if (c == '"') // literal mode off
            {
                cmode = pmode.front();
                pmode.pop_front();
            }

            temp += c;
        }
        else if (cmode == mode_in_ns)
        {
            if (c == '$') // variable
            {
                pmode.push_front(cmode);
                cmode = mode_in_var;
                temp = "";
            }
            else if (c == '@') // at-rule
            {
                pmode.push_front(cmode);
                cmode = mode_in_atrule;
                temp = "";
            }
            else if (c == '}') // end of namespace
            {
                ns = QString();
                cmode = pmode.front();
                pmode.pop_front();
                temp = "";
            }
            else if (c == '{') // not in at-rule, must be prop table
            {
                //object prop table found
                temp.replace(" ","");
                table = Table(temp);
                propsheet.addObjectPropertyTable(temp,table);
                pmode.push_front(cmode);
                cmode = mode_in_objproptable;
            }
            else
                temp += c; // temp will contain the identifier for the prop table
        }
        else if (cmode == mode_in_seq)
        {
            // name should be "some_name(range_start,range_end)"

            if (c == '"') // enter literal mode
            {
                pmode.push_front(cmode);
                cmode = mode_literal;
            }
            else if (c == '{')
                level++;
            else if (c == '}')
                level--;


            if (level == 0) // last } found, parse sequence
            {
                QRegExp seqreg("([^\\()]+)\\(([0-9]+),([0-9]+)\\)");
                name.replace(" ","");
                if (seqreg.indexIn(name) == -1)
                {
                    iErrorCode = QOPS_ERR_PARSE;
                    return;
                }
                seq = Sequence(seqreg.cap(1),seqreg.cap(2).toInt(),seqreg.cap(3).toInt());
                parseSeq(temp,seq);
                propsheet.addSequence(seqreg.cap(1),seq,ns);

                cmode = pmode.front();
                pmode.pop_front();
            }
            else
                temp += c;

        }
        else if (cmode == mode_in_var)
        {
            if (c == ';')
            {
                // temp should be "var_name=value"
                parseVar(temp,propsheet,ns);
                cmode = pmode;
            }
            else
                temp += c;
        }
        else if (cmode == mode_in_atrule)
        {
            if (c == '{') // start of at-rule block
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
                    level = 1;
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
            else if (c == '}') // the at-rule block ended without starting, error
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
    QRegExp propreg("([^:]+):([^;]+);");
    QRegExp cleanspaces("(^ +| +$)");
    QRegExp proprulereg("!([a-zA-Z]+)$");
    int n;
    int off = 0;
    QString name,value,proprule;


    while ((n = propreg.indexIn(data,off)) != -1)
    {
        name = propreg.cap(1).replace(cleanspaces,"");

        Property prop(name);

        value = propreg.cap(2).replace(cleanspaces,"");
        if (proprulereg.indexIn(value) != -1)
        {
            proprule = proprulereg.cap(1);
            value = value.left(value.size()-proprulereg.cap(0).size()).replace(cleanspaces,"");
            prop.setRule(proprule);
        }

        prop.setValue(value);

        // extract parts
        // exlude literal and between "(" ")"

        bool bLit = false;
        bool bSpace = false;
        QString part;
        int partc = 0;

        for (int i=0;i<value.size();i++)
        {
            if (!bLit && !bSpace)
            {
                if (value[i] == '"' || value[i] == '(')
                    bLit = true;
                else if (value[i] == ' ')
                {
                    //end of part
                    prop.setValue(part,partc);
                    partc++;
                    part="";
                    bSpace = true;
                    continue;
                }
            }
            else if (bSpace)
            {
                if (value[i] != ' ')
                    bSpace = false;
                else
                    continue;
            }
            else
            {
                if (value[i] == '"' || value[i] == ')')
                    bLit = false;
            }
            part += value[i];
        }


        off = n + propreg.cap(0).size();
    }
}

void PropsheetReaderPrivate::parseVar(QString data, Propsheet &propsheet, QString ns)
{

}

void PropsheetReaderPrivate::parseSeq(QString data, Sequence &seq)
{

}
