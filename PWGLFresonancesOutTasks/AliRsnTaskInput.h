//
// *** Class AliRsnTaskInput ***
//
//  Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIRSNTASKINPUT_H
#define ALIRSNTASKINPUT_H

#include <Rtypes.h>
#include "AliRsnTask.h"

class TH1D;
class TFile;
class TArrayI;
class TArrayD;
class AliRsnTaskInput : public AliRsnTask
{
public:

   AliRsnTaskInput(const char *name="empty", const char *title="Empty Task");
   AliRsnTaskInput(const AliRsnTaskInput &copy);
   AliRsnTaskInput &operator=(const AliRsnTaskInput &copy);
   virtual ~AliRsnTaskInput();

   virtual void   Exec(Option_t *option);

   void SetFileName(TString fname) { fFileName = fname; }
   void SetCacheDir(TString dir) { fCacheDir = dir; }

   Bool_t OpenFile();
   TH1D *CreateHistogram(const char *name, Int_t idProj, TArrayI *idCuts=0, TArrayD *minCut=0, TArrayD *maxCut=0);

private:

   TString fFileName;
   TString fCacheDir;
   TFile *fFile;


   ClassDef(AliRsnTaskInput, 1)
};

#endif
