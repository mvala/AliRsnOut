//
// *** Class AliRsnTaskParticleYield ***
//
//  Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIRSNTASKPARTICLEYIELD_H
#define ALIRSNTASKPARTICLEYIELD_H

#include <Rtypes.h>
#include <TString.h>
#include "AliRsnTask.h"

class TH1;
class TH1D;
class TList;
class AliRsnTaskParticleYield : public AliRsnTask
{
public:

   enum EHistAction { kSum = 0 };
   enum EHistType { kSigBkg = 0, kBkg=1, kAll };

   AliRsnTaskParticleYield(const char *name="empty", const char *title="Empty Task");
   AliRsnTaskParticleYield(const AliRsnTaskParticleYield &copy);
   AliRsnTaskParticleYield &operator=(const AliRsnTaskParticleYield &copy);
   virtual ~AliRsnTaskParticleYield();

   virtual void   Exec(Option_t *option);

   void GenerateHistograms(TList *l);
   void ShiftHistogram(TH1 *h);
   void DoFit(TH1 *h, Double_t min, Double_t max);

   void SetHistograms(EHistType type,TString histos, EHistAction action);
   void SetNormalizationRange(Double_t min,Double_t max);
   void SetFitRange(Double_t min,Double_t max);


private:

   EHistAction   fActionType[kAll];
   TString       fHistosStr[kAll];
   TH1D          *fHist[kAll];
   Double_t      fNorm[2];
   Double_t      fFit[2];

   ClassDef(AliRsnTaskParticleYield, 1)
};

#endif
