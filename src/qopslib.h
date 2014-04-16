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
 * Use ./configure to configure then make.
 *
 * \section smart_pointer Smart Pointers
 *
 * By default the Propsheet, Table and Property classes are made as smart pointers. To disable this behaviour configure with --NO_SMART_POINTERS.
 * (The copy methods will then be unavailable since they lost their purpose.)
 */

#define QOPS_NO_ERR 0
#define QOPS_ERR_FILE 1 //could not open file
#define QOPS_ERR_PARSE 2 //parsing error

#endif // QOPSLIB_H
