//
// *** Class AliRsnTaskEmpty ***
//
//  Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIRSNTASKPARTICLEYIELD_H
#define ALIRSNTASKPARTICLEYIELD_H

#include <Rtypes.h>
#include "AliRsnTask.h"
class RooPlot;
class TH1;
class AliRsnTaskParticleYield : public AliRsnTask
{
public:

   AliRsnTaskParticleYield(const char *name="empty", const char *title="Empty Task");
   AliRsnTaskParticleYield(const AliRsnTaskParticleYield &copy);
   AliRsnTaskParticleYield &operator=(const AliRsnTaskParticleYield &copy);
   virtual ~AliRsnTaskParticleYield();

   virtual void   Exec(Option_t *option);

   void SetIDProjection(Int_t idProj) {fIDProjection = idProj; }
   void SetCuts(TArrayI *ids, TArrayD *mins, TArrayD *maxs);

   RooPlot *DoFit(TH1 *h);
   void ShiftHistogram(TH1 *h);

private:

   Int_t fIDProjection;
   TArrayI *fCutIDs;
   TArrayD *fCutMins;
   TArrayD *fCutMaxs;

   ClassDef(AliRsnTaskParticleYield, 1)
};

#endif
