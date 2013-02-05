#ifndef DATAENVELOPEDELEGATE_H
#define DATAENVELOPEDELEGATE_H

#include "envelope/dataenvelopeparameters.h"
#include "envelope/dataenvelopeui.h"


class DataEnvelopeDelegate : public DataUiHandlerDelegate
{
public:
    DataEnvelopeDelegate(DataEnvelopeParameters * params, DataEnvelopeUI * ui , QObject *parent = 0);
};

#endif // DATAENVELOPEDELEGATE_H
