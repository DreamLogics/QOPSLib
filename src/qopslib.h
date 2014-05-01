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

/*! \mainpage Object Property Sheet Library
 *
 * \section intro_sec Introduction
 *
 * Object Property Sheets are files using a stuctured way of defining object properties. It is based on the CSS specification,
 *  yet simplified (although extended in some areas) to create a more generic solution for defining properties/attributes of a specific object/element.
 * The Object Property Sheet Specification can be found here: .
 *
 * \section install_sec Installation
 *
 * Use "qmake" in src/ to create the makefiles then "make".
 * "make install" will install into /usr/local/lib/ and /usr/local/include/QOPSLib/
 *
 * \section smart_pointer Smart Pointers
 *
 * By default the Propsheet, Table, Sequence and Property classes are made as smart pointers. To disable this behaviour add the NO_SMART_POINTERS define.
 * (The copy methods will then be unavailable since they lost their purpose.)
 *
 * qmake "DEFINES += NO_SMART_POINTERS"
 */

#define QOPS_NO_ERR 0
#define QOPS_ERR_PARSE 1 //parsing error
