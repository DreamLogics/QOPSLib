#ifndef PROPSHEETREADERPRIVATE_H
#define PROPSHEETREADERPRIVATE_H

#include <QString>
#include "propsheet.h"

namespace QOPS
{

class PropsheetReaderPrivate
{
public:
    PropsheetReaderPrivate();

private:

    enum mode
    {
        mode_base = 0,
        mode_in_objproptable,
        mode_in_var,
        mode_in_seq,
        mode_in_ns,
        mode_in_atrule,
        mode_literal,
        mode_in_frame,
        mode_in_obj
    };

    void load(QString data, Propsheet &propsheet);
    void import(QString &data);
    void parseProps(QString data, Table &table);
    void parseVar(QString data, Propsheet &propsheet, QString ns);
    void parseSeq(QString data, Sequence &seq);


    int iErrorCode;
    InformationProvider *pIP;

    friend class PropsheetReader;
};

}

#endif // PROPSHEETREADERPRIVATE_H
