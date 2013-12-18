//
// *** Class AliRsnUtils ***
//
//  Namespace AliRsnUtils
//
// authors: Martin Vala (martin.vala@cern.ch)
//          Jan Musinsky (jan.musinsky@cern.ch)
//

#ifndef ALIRSNUTILS_H
#define ALIRSNUTILS_H

#include <Rtypes.h>

class TMultiGraph;
class TGraphErrors;
class TH1D;
namespace AliRsnUtils
{
   TH1D *GraphToHistogram(TGraphErrors *gr, Bool_t useGraphEY = kFALSE, Double_t min = 0.0);
   TMultiGraph *CreateGraphStatSysErr(const char *name, Color_t color, const char *filename, Double_t fac = 1.0);

}

#endif
