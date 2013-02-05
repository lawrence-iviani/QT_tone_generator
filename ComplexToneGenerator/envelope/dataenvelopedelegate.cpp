#include "dataenvelopedelegate.h"

DataEnvelopeDelegate::DataEnvelopeDelegate(DataEnvelopeParameters * params, DataEnvelopeUI * ui , QObject *parent ) :
    DataUiHandlerDelegate((DataUiHandlerProperty *)params,(DataUiHandlerUI*)ui,"EnvelopeDocument","Envelope_RootParams",parent)
{
}

