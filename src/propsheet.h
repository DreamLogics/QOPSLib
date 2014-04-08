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
#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QString>
#include "style.h"
#include "animation.h"

namespace QOPS
{

class PropsheetPrivate;

class Propsheet
{
public:
    Propsheet();
    ~Propsheet();

    Style objectPropertyTable(QString id, QString ns = QString()) const;
    Style classPropertyTable(QString classname, QString object_id, QString ns = QString()) const;
    void addObjectStyle(QString id, Style s, QString ns = QString()) const;
    void addClassStyle(QString classname, Style s, QString ns = QString()) const;
    void addClassStyle(QString classname, Style s, QString ns = QString(), QString object_id = QString()) const;

    Animation animation(QString id) const;
    void addAnimation(QString id, Animation ani) const;

    QString variable(QString varname) const;
    void setVariable(QString varname, QString value) const;

private:

    PropsheetPrivate *m_p;

};

}

#endif // STYLESHEET_H
