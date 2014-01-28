//
// *** Class AliRsnTaskSpectraBin ***
//
// Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//
#include <TArrayI.h>
#include <TArrayD.h>
#include <TH1D.h>
#include <TFolder.h>
#include <RooRealVar.h>
#include <RooVoigtian.h>
#include <RooChebychev.h>
#include <RooPolynomial.h>
#include <RooPlot.h>
#include <RooDataHist.h>
#include <RooExtendPdf.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>
#include <TROOT.h>
#include <TDirectory.h>


#include "AliRsnTaskInput.h"
#include "AliRsnTaskSpectraBin.h"
#include "AliRsnTaskParticleYield.h"

ClassImp(AliRsnTaskSpectraBin)

//______________________________________________________________________________
AliRsnTaskSpectraBin::AliRsnTaskSpectraBin(const char *name, const char *title) : AliRsnTask(name, title),
   fIDProjection(0),
   fCutIDs(0),
   fCutMins(0),
   fCutMaxs(0)
{
   //
   // Defauult constructor
   //

   // We need that current task is executed after sub-tasks
//    fExecTaskBefore = kFALSE;
}

//______________________________________________________________________________
AliRsnTaskSpectraBin::AliRsnTaskSpectraBin(const AliRsnTaskSpectraBin &copy) : AliRsnTask(copy),
   fIDProjection(copy.fIDProjection),
   fCutIDs(copy.fCutIDs),
   fCutMins(copy.fCutMins),
   fCutMaxs(copy.fCutMaxs)

{
   //
   // Copy constructor
   //

}

//______________________________________________________________________________
AliRsnTaskSpectraBin &AliRsnTaskSpectraBin::operator=(const AliRsnTaskSpectraBin &copy)
{
   //
   // Assignment constructor
   //

   TTask::operator=(copy);
   if (this == &copy)
      return *this;
   fIDProjection = copy.fIDProjection;
   fCutIDs = copy.fCutIDs;
   fCutMins = copy.fCutMins;
   fCutMaxs = copy.fCutMaxs;

   return (*this);
}

//______________________________________________________________________________
AliRsnTaskSpectraBin::~AliRsnTaskSpectraBin()
{
   //
   // Destructor
   //

   delete fCutIDs;
   delete fCutMins;
   delete fCutMaxs;
}

//______________________________________________________________________________
void AliRsnTaskSpectraBin::Exec(Option_t * /*option*/)
{
   // Put your task job

   Printf("Executing %s ...", GetName());

   AliRsnTaskInput *input = (AliRsnTaskInput *) GetParent();

   TH1D *hUnlike = input->CreateHistogram("Unlike",fIDProjection,fCutIDs,fCutMins,fCutMaxs);
   TH1D *hLikePP = input->CreateHistogram("LikePP",fIDProjection,fCutIDs,fCutMins,fCutMaxs);
   TH1D *hLikeMM = input->CreateHistogram("LikeMM",fIDProjection,fCutIDs,fCutMins,fCutMaxs);
   TH1D *hMixing = input->CreateHistogram("Mixing",fIDProjection,fCutIDs,fCutMins,fCutMaxs);

   TList l;
   l.Add(hUnlike);
   l.Add(hLikePP);
   l.Add(hLikeMM);
   l.Add(hMixing);

   // return to the home directory (so we are not in file)
   // TODO check for memory leak (object are saved in memory)
   gROOT->cd();
   // sets all tasks with needed histograms
   AliRsnTaskParticleYield *t;
   for (Int_t i=0; i<fTasks->GetEntries(); i++) {
      t = dynamic_cast<AliRsnTaskParticleYield *>(fTasks->At(i));
      if (!t) continue;
      t->GenerateHistograms(&l);
   }

   if (fFolder) {
      fFolder->Add(hUnlike);
      fFolder->Add(hLikePP);
      fFolder->Add(hLikeMM);
      fFolder->Add(hMixing);
   }

}


//______________________________________________________________________________
void AliRsnTaskSpectraBin::SetCuts(TArrayI *ids, TArrayD *mins, TArrayD *maxs)
{
   if (fCutIDs) delete fCutIDs;
   if (fCutMins) delete fCutMins;
   if (fCutMaxs) delete fCutMaxs;

   fCutIDs = new TArrayI(*ids);
   fCutMins = new TArrayD(*mins);
   fCutMaxs = new TArrayD(*maxs);
}

//______________________________________________________________________________
void AliRsnTaskSpectraBin::AddVariations(TList *l)
{
   if (!l) return;
   TIter next(l);
   AliRsnTask *t;
   AliRsnTaskParticleYield *tpy;
   while((t = (AliRsnTask *)next())) {
//       Printf("Adding Variation %s",t->GetName());
      tpy = dynamic_cast<AliRsnTaskParticleYield *> (t);
      if (tpy) {
         tpy = (AliRsnTaskParticleYield *)tpy->Clone();
         Add((AliRsnTask *)tpy);
      }
   }
}
