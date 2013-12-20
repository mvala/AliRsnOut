//
// *** Class AliRsnTaskCanvas ***
//
// Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//
#include <TCanvas.h>
#include <TFolder.h>
#include <TMath.h>
#include "AliRsnTaskCanvas.h"

ClassImp(AliRsnTaskCanvas)

//______________________________________________________________________________
AliRsnTaskCanvas::AliRsnTaskCanvas(const char *name, const char *title) :
   AliRsnTask(name, title)
{
   //
   // Defauult constructor
   //

   // We need that current task is executed after sub-tasks
   fExecTaskBefore = kFALSE;
}

//______________________________________________________________________________
AliRsnTaskCanvas::AliRsnTaskCanvas(const AliRsnTaskCanvas &copy) :
   AliRsnTask(copy)
{
   //
   // Copy constructor
   //

}

//______________________________________________________________________________
AliRsnTaskCanvas &AliRsnTaskCanvas::operator=(const AliRsnTaskCanvas &copy)
{
   //
   // Assignment constructor
   //

   TTask::operator=(copy);
   if (this == &copy) return *this;

   return (*this);
}

//______________________________________________________________________________
AliRsnTaskCanvas::~AliRsnTaskCanvas()
{
   //
   // Destructor
   //
}

//______________________________________________________________________________
void AliRsnTaskCanvas::Exec(Option_t * /*option*/)
{
   Printf("Running Canvas task ...");

   TCanvas *c = 0;
   TCanvas *cf = 0;
   TObject *o;
   TFolder *f;
   Int_t numFolders = 0;
   Int_t x,y,x2,y2;

   for (Int_t i = 0; i < fInput->GetEntries(); ++i) {
      o = (TObject *) fInput->At(i);
      if (o->InheritsFrom("TFolder")) numFolders++;
   }

   GetOptimalDivide(numFolders,x,y);

   for (Int_t i = 0; i < fInput->GetEntries(); ++i) {
      o = (TObject *) fInput->At(i);
      if (o->InheritsFrom("TFolder")) {
         f = (TFolder *) o;
         // TODO create list of canvas folders (to cleanup)
         cf = new TCanvas(TString::Format("c_%s", f->GetName()).Data(), TString::Format("%s", f->GetName()).Data());
         TList *l = (TList *) f->GetListOfFolders();
         GetOptimalDivide(l->GetEntries(),x2,y2);
         cf->Divide(x2,y2);
         TObject *oTmp;
         for (Int_t j = 0; j < l->GetEntries(); ++j) {
            oTmp = (TObject *) l->At(j);
            cf->cd(j + 1);
            oTmp->Draw();
         }
      } else {
         if (!c) {
            c = new TCanvas(TString::Format("c_%s", GetName()).Data(), TString::Format("%s", GetName()).Data());
            c->Divide(x,y);
         }
         c->cd(i + 1 - numFolders);
         o->Draw();
      }
   }

}

//______________________________________________________________________________
void AliRsnTaskCanvas::GetOptimalDivide(Int_t num, Int_t &x, Int_t &y)
{
   Double_t tmp = TMath::Sqrt((Double_t) num);
   if (TMath::Abs(TMath::Power((Int_t)tmp, 2) - num) < 0.0001) {
      x = (Int_t) tmp;
      y = (Int_t) tmp;
   } else {
      x = (Int_t) tmp + 1;
      y = (num / x);
      if ((num % x) > 0) y++;
   }

//   Printf("num=%d x=%d y=%d", num, x, y);

}
