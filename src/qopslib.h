#ifndef QOPSLIB_H
#define QOPSLIB_H

#include <propsheet.h>
#include <propsheetreader.h>
#include <propsheetwriter.h>
#include <table.h>
#include <property.h>
#include <valuetypes.h>

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

#endif // QOPSLIB_H
