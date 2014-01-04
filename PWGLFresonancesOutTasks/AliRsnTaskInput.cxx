//
// *** Class AliRsnTaskInput ***
//
// Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#include <TFile.h>
#include <TH1.h>
#include <TArrayI.h>
#include <TArrayD.h>
#include <TAxis.h>
#include <THnSparse.h>


#include "AliRsnTaskInput.h"

ClassImp(AliRsnTaskInput)

//______________________________________________________________________________
AliRsnTaskInput::AliRsnTaskInput(const char *name, const char *title) : AliRsnTask(name, title),
fFileName(),
fCacheDir(),
fFile(0)

{
   //
   // Defauult constructor
   //

   // We need that current task is executed after sub-tasks
   fExecTaskBefore = kFALSE;
}

//______________________________________________________________________________
AliRsnTaskInput::AliRsnTaskInput(const AliRsnTaskInput &copy) : AliRsnTask(copy),
fFileName(copy.fFileName),
fCacheDir(copy.fCacheDir),
fFile(copy.fFile)

{
   //
   // Copy constructor
   //

}

//______________________________________________________________________________
AliRsnTaskInput &AliRsnTaskInput::operator=(const AliRsnTaskInput &copy)
{
   //
   // Assignment constructor
   //

   TTask::operator=(copy);
   if (this == &copy)
      return *this;
   fFileName = copy.fFileName;
   fCacheDir = copy.fCacheDir;
   fFile = copy.fFile;

   return (*this);
}

//______________________________________________________________________________
AliRsnTaskInput::~AliRsnTaskInput()
{
   //
   // Destructor
   //
   if (fFile) fFile->Close();
   delete fFile;
}

//______________________________________________________________________________
void AliRsnTaskInput::Exec(Option_t * /*option*/)
{

   // Put your task job
   if (!OpenFile()) {
      Error("AliRsnTaskInput::Exec",TString::Format("File '%s' could not be opened !!!",fFileName.Data()).Data());
      // TODO - Disable all sub tasks
      return;
   }
}

//______________________________________________________________________________
Bool_t AliRsnTaskInput::OpenFile()
{
   // if file exists then return true
   if (fFile) return kTRUE;

   if (fFileName.IsNull()) return kFALSE;

   TString opt = "READ";
   if (!fCacheDir.IsNull()) {
      TFile::SetCacheFileDir(fCacheDir.Data());
      opt = "CACHEREAD";
   }


   fFile = TFile::Open(fFileName.Data(),opt.Data());

   if (fFile) return kTRUE;

   return kFALSE;
}

//______________________________________________________________________________
TH1D* AliRsnTaskInput::CreateHistogram(const char* name, Int_t idProj, Int_t idForName, TArrayI *idCuts, TArrayD *minCut, TArrayD *maxCut)
{
   if (!fFile) return 0;

   THnSparse *sparse = dynamic_cast<THnSparse*>(fFile->Get(name));
   if (!sparse) {
         Error("AliRsnTaskInput::CreateHistogram",TString::Format("Couldn't open sparse '%s' !!!",name).Data());
         return 0;
   }

   Double_t min=0.0,max=0.0;
   if (idCuts) {
      if ((idCuts->GetSize()!=minCut->GetSize()) || (idCuts->GetSize()!=maxCut->GetSize())) {
         Error("AliRsnTaskInput::CreateHistogram","Sizes of idCuts, minCut and maxCut doesn't match !!!");
         return 0;
      }

      for (Int_t i=0;i<idCuts->GetSize();i++) {
         sparse->GetAxis(idCuts->At(i))->SetRangeUser(minCut->At(i),maxCut->At(i));
         if (idCuts->At(i) == idForName) { min = minCut->At(i); max = maxCut->At(i);}
      }
   }

   TH1D *h = sparse->Projection(idProj);
   if (!h) return 0;
   h->SetName(TString::Format("%s_%.2f_%.2f", h->GetName(), min, max).Data());

   return h;

}
