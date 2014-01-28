//
// *** Class AliRsnTaskEmpty ***
//
//  Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIRSNTASKSPECTRABIN_H
#define ALIRSNTASKSPECTRABIN_H

#include <Rtypes.h>
#include "AliRsnTask.h"
class RooPlot;
class TH1;
class AliRsnTaskSpectraBin : public AliRsnTask
{
public:

   AliRsnTaskSpectraBin(const char *name="empty", const char *title="Empty Task");
   AliRsnTaskSpectraBin(const AliRsnTaskSpectraBin &copy);
   AliRsnTaskSpectraBin &operator=(const AliRsnTaskSpectraBin &copy);
   virtual ~AliRsnTaskSpectraBin();

   virtual void   Exec(Option_t *option);

   void SetIDProjection(Int_t idProj) {fIDProjection = idProj; }
   void SetCuts(TArrayI *ids, TArrayD *mins, TArrayD *maxs);

   void AddVariations(TList *l);

private:

   Int_t fIDProjection;
   TArrayI *fCutIDs;
   TArrayD *fCutMins;
   TArrayD *fCutMaxs;

   ClassDef(AliRsnTaskSpectraBin, 1)
};

#endif
