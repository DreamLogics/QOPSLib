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
#include "qopslib.h"
#include <QDebug>

using namespace QOPS;

/*
 * TODO:
 * -linecount per file (@import)
 */


PropsheetReader::PropsheetReader(InformationProvider *ip)
{
    m_p = new PropsheetReaderPrivate();
    m_p->pIP = ip;
}

PropsheetReader::PropsheetReader(PropsheetReader &ref)
{
    m_p = new PropsheetReaderPrivate();
    m_p->pIP = ref.m_p->pIP;
}

PropsheetReader::~PropsheetReader()
{
    delete m_p;
}

/*!
 * \brief Read an Object %Property Sheet from an Utf8 String.
 * \param data Utf8 encoded data as QByteArray.
 * \return The generated Propsheet.
 */

Propsheet PropsheetReader::fromUtf8String(QByteArray &data) const
{
    Propsheet p;
    m_p->load(QString::fromUtf8(data),p);
    return p;
}

/*!
 * \brief Read an Object %Property Sheet from a QString.
 * \param str Data as QString.
 * \return The generated Propsheet.
 */

Propsheet PropsheetReader::fromString(QString str) const
{
    Propsheet p;
    m_p->load(str,p);
    return p;
}

/*!
 * \brief Retreive the error code.
 * \return Value is QOPS_NO_ERR if there is no error.
 */

int PropsheetReader::getError() const
{
    return m_p->iErrorCode;
}

/*!
 * \brief Give the error message if there was an error.
 * \return Error message as string.
 */

QString PropsheetReader::getErrorMsg() const
{
    return m_p->sErrorMsg;
}

/*
 * Private Impl.
 */

PropsheetReaderPrivate::PropsheetReaderPrivate() : pIP(0), iErrorCode(QOPS_NO_ERR)
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
        sErrorMsg = "Incorrect bracket count.";
        return;
    }

    data = nd;

    //data.replace(QRegExp("[\n\r\t]+"),"");

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

    QRegExp spaces("[ \t\r\n]+");

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
                temp.replace(spaces,"");
                if (temp != "")
                {
                    iErrorCode = QOPS_ERR_PARSE;
                    sErrorMsg = "Invalid input before variable. On line: "+QString::number(countLines(data,i));
                    return;
                }
                pmode.push_front(cmode);
                cmode = mode_in_var;
                temp = "";
            }
            else if (c == '@') // at-rule
            {
                temp.replace(spaces,"");
                if (temp != "")
                {
                    iErrorCode = QOPS_ERR_PARSE;
                    sErrorMsg = "Invalid input before @-rule. On line: "+QString::number(countLines(data,i));
                    return;
                }
                pmode.push_front(cmode);
                cmode = mode_in_atrule;
                temp = "";
            }
            else if (c == '}') // } found before {, error
            {
                iErrorCode = QOPS_ERR_PARSE;
                sErrorMsg = "Found closing bracket before an opening bracket. On line: "+QString::number(countLines(data,i));
                return;
            }
            else if (c == '{') // not in at-rule, must be prop table
            {
                //object prop table found
                temp.replace(spaces,"");
                table = Table(temp);
                temp="";
                propsheet.addObjectPropertyTable(table);
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
                parseProps(temp,table,countLines(data,i-temp.size()));
                temp = "";
                cmode = pmode.front();
                pmode.pop_front();
            }
            else if (c == '{') // prop table should not contain {, error
            {
                iErrorCode = QOPS_ERR_PARSE;
                sErrorMsg = "Found opening bracket in a property table. On line: "+QString::number(countLines(data,i));
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
                temp.replace(spaces,"");
                table = Table(temp);
                temp="";
                propsheet.addObjectPropertyTable(table,ns);
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
                QRegExp seqreg("([^\\()]+)\\(([0-9-]+),([0-9-]+)\\)");
                name.replace(spaces,"");
                if (seqreg.indexIn(name) == -1)
                {
                    iErrorCode = QOPS_ERR_PARSE;
                    sErrorMsg = "Invalid sequence identifier. On line: "+QString::number(countLines(data,i));
                    return;
                }
                seq = Sequence(seqreg.cap(1),seqreg.cap(2).toInt(),seqreg.cap(3).toInt());
                parseSeq(temp,seq,countLines(data,i-temp.size()));
                if (iErrorCode == QOPS_ERR_PARSE)
                    return;
                propsheet.addSequence(seq,ns);

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
                cmode = pmode.front();
                pmode.pop_front();
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
                    temp.replace(spaces,"");
                    temp = temp.mid(9);
                    ns = temp;
                    temp = "";
                    cmode = mode_in_ns;
                }
                else if (temp.startsWith("sequence"))
                {
                    level = 1;
                    temp.replace(spaces,"");
                    temp = temp.mid(8);
                    name = temp;
                    temp = "";
                    cmode = mode_in_seq;
                }
                else
                {
                    iErrorCode = QOPS_ERR_PARSE;
                    sErrorMsg = "Invalid @-rule type. On line: "+QString::number(countLines(data,i));
                    return;
                }
            }
            else if (c == '}') // the at-rule block ended without starting, error
            {
                iErrorCode = QOPS_ERR_PARSE;
                sErrorMsg = "{ expected on line: "+QString::number(countLines(data,i));
                return;
            }
            else
                temp += c;
        }

    }

}

void PropsheetReaderPrivate::parseProps(QString data, Table &table, int lines)
{
    QRegExp propreg("([^:]+):([^;]+);");
    QRegExp cleanspaces("(^[ \t\r\n]+|[ \t\r\n]+$)");
    QRegExp proprulereg("!([a-zA-Z_-]+)");
    int n;
    int off = 0;
    QString name,value;


    while ((n = propreg.indexIn(data,off)) != -1)
    {
        name = propreg.cap(1).replace(cleanspaces,"");

        Property prop(name);

        value = propreg.cap(2).replace(cleanspaces,"");
        /*if (proprulereg.indexIn(value) != -1)
        {
            proprule = proprulereg.cap(1);
            value = value.left(value.size()-proprulereg.cap(0).size()).replace(cleanspaces,"");
            prop.setRule(proprule);
        }*/
        int firstrule = -1;
        int in;
        int offr=0;
        while ((in=proprulereg.indexIn(value,offr)) != -1) // retrieve !rule rules
        {
            if (firstrule == -1)
                firstrule = in;
            prop.addRule(proprulereg.cap(1));
            offr = in + proprulereg.cap(0).size();
        }

        if (firstrule != -1)
            value = value.left(firstrule).replace(cleanspaces,"");

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

        table.addProperty(prop);


        off = n + propreg.cap(0).size();
    }
}

void PropsheetReaderPrivate::parseVar(QString data, Propsheet &propsheet, QString ns)
{
    QRegExp varreg("([^=]+)=([.]+)");
    if (varreg.indexIn(data) != -1)
    {
        propsheet.setVariable(varreg.cap(1),varreg.cap(2),ns);
    }
}

void PropsheetReaderPrivate::parseSeq(QString data, Sequence &seq, int lines)
{
    /* obj_id
     * {
     *   1
     *   {
     *     test: 1;
     *   }
     * }
     */
    QRegExp cleanspaces("(^[ \t\r\n]+|[ \t\r\n]+$)");
    QString temp;
    QString object;
    int frame;
    mode cmode = mode_base;
    QList<mode> pmode;
    QRegExp framereg("^[0-9-]+$");

    lines--;

    for (int i=0;i<data.size();i++)
    {
        QChar c = data[i];

        if (cmode == mode_base)
        {
            if (c == '{')
            {
                //now getting the frames
                //temp == object id
                object = temp.replace(cleanspaces,"");
                temp = "";
                pmode.push_front(cmode);
                cmode = mode_in_obj;
            }
            else if (c == '}')
            {
                iErrorCode = QOPS_ERR_PARSE;
                sErrorMsg = "Found closing bracket before an opening bracket. On line: "+QString::number(countLines(data,i)+lines);
                return;
            }
            else
                temp += c;

        }
        else if (cmode == mode_in_obj)
        {
            if (c == '{')
            {
                //found frame
                temp.replace(cleanspaces,"");
                if (framereg.indexIn(temp) == -1)
                {
                    //invalid frame id
                    iErrorCode = QOPS_ERR_PARSE;
                    sErrorMsg = "Invalid frame identifier. Must be a number. On line: "+QString::number(countLines(data,i)+lines);
                    return;
                }
                frame = temp.toInt();
                if (frame < seq.startIndex() || frame > seq.endIndex())
                {
                    //frame out of range
                    iErrorCode = QOPS_ERR_PARSE;
                    sErrorMsg = "Frame "+temp+" is out of range. Range is "+QString::number(seq.startIndex())+" to "+QString::number(seq.endIndex())+". On line: "+QString::number(countLines(data,i)+lines);
                    return;
                }
                temp = "";
                pmode.push_front(cmode);
                cmode = mode_in_frame;
            }
            else if (c == '}')
            {
                //end of object
                cmode = pmode.front();
                pmode.pop_front();
            }
            else
                temp += c;
        }
        else if (cmode == mode_in_frame)
        {
            if (c == '"')
            {
                pmode.push_front(cmode);
                cmode = mode_literal;
                temp += c;
            }
            else if (c == '}')
            {
                //end of frame
                Table tbl(QString::number(frame));
                parseProps(temp,tbl,(countLines(data,i) + lines));
                seq.setFrame(object,frame,tbl);
                cmode = pmode.front();
                pmode.pop_front();
                temp = "";
            }
            else if (c == '{')
            {
                iErrorCode = QOPS_ERR_PARSE;
                sErrorMsg = "Found opening bracket in a property table. On line: "+QString::number(countLines(data,i)+lines);
                return;
            }
            else
                temp += c;
        }
        else if (cmode == mode_literal)
        {
            if (c == '"')
            {
                cmode = pmode.front();
                pmode.pop_front();
            }
            temp += c;
        }
        else
            temp += c;
    }

}

void PropsheetReaderPrivate::import(QString &data)
{
    //TODO: search backwards to omit the need for a copy of data?

    QRegExp importrule("@import +([^;]+);");
    QString temp=data;
    int i,off=0;

    while ((i=importrule.indexIn(temp,off)) != -1)
    {
        if (pIP)
            data.replace(importrule.cap(0),QString::fromUtf8(pIP->importPropsheet(importrule.cap(1))));
        else
            data.replace(importrule.cap(0),"");
        off = i + importrule.cap(0).size();
    }
}

int PropsheetReaderPrivate::countLines(QString &data, int end)
{
    int count = 1;
    QChar c,p='\0';

    for (int i=0;i<end;i++)
    {
        c = data[i];
        if ((c == '\n' && p != '\r') || c == '\r')
            count++;
        p=c;
    }

    return count;
}
