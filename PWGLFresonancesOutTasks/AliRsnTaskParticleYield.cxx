//
// *** Class AliRsnTaskParticleYield ***
//
// Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//
#include <TArrayI.h>
#include <TArrayD.h>
#include <TH1D.h>
#include <TFolder.h>

#include "AliRsnTaskInput.h"
#include "AliRsnTaskParticleYield.h"

ClassImp(AliRsnTaskParticleYield)

//______________________________________________________________________________
AliRsnTaskParticleYield::AliRsnTaskParticleYield(const char *name, const char *title) : AliRsnTask(name, title),
   fIDProjection(0),
   fCutIDs(0),
   fCutMins(0),
   fCutMaxs(0)
{
   //
   // Defauult constructor
   //

   // We need that current task is executed after sub-tasks
   fExecTaskBefore = kFALSE;
}

//______________________________________________________________________________
AliRsnTaskParticleYield::AliRsnTaskParticleYield(const AliRsnTaskParticleYield &copy) : AliRsnTask(copy),
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
AliRsnTaskParticleYield &AliRsnTaskParticleYield::operator=(const AliRsnTaskParticleYield &copy)
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
AliRsnTaskParticleYield::~AliRsnTaskParticleYield()
{
   //
   // Destructor
   //

   delete fCutIDs;
   delete fCutMins;
   delete fCutMaxs;
}

//______________________________________________________________________________
void AliRsnTaskParticleYield::Exec(Option_t * /*option*/)
{
   // Put your task job

   Printf("Executing %s ...", GetName());

   AliRsnTaskInput *input = (AliRsnTaskInput *) GetParent();

   TH1D *h1 = input->CreateHistogram("Unlike",fIDProjection,fCutIDs,fCutMins,fCutMaxs);
   TH1D *h2 = input->CreateHistogram("LikePP",fIDProjection,fCutIDs,fCutMins,fCutMaxs);
   TH1D *h3 = input->CreateHistogram("LikeMM",fIDProjection,fCutIDs,fCutMins,fCutMaxs);
   TH1D *h4 = input->CreateHistogram("Mixing",fIDProjection,fCutIDs,fCutMins,fCutMaxs);

   h1->Print();
   h2->Print();
   h3->Print();
   h4->Print();

   TH1D *hSubLikeSum = (TH1D *)h1->Clone();
   TH1D *hLikeSumNorm = (TH1D *)h2->Clone();
   hLikeSumNorm->Add(h3);

   TH1D *hSubMixing = (TH1D *)h1->Clone();
   TH1D *hMixNorm = (TH1D *)h4->Clone();;


   // Choose backgrouns and apply action

   // Normalize
   Double_t normMin = 1.045;
   Double_t normMax = 1.065;
   Double_t scale = hSubLikeSum->Integral(hSubLikeSum->FindBin(normMin),hSubLikeSum->FindBin(normMax))/
                    hLikeSumNorm->Integral(hLikeSumNorm->FindBin(normMin),hLikeSumNorm->FindBin(normMax));

   hLikeSumNorm->Scale(scale);
   hSubLikeSum->Add(hLikeSumNorm,-1);

   scale = hSubMixing->Integral(hSubMixing->FindBin(normMin),hSubMixing->FindBin(normMax))/
           hMixNorm->Integral(hMixNorm->FindBin(normMin),hMixNorm->FindBin(normMax));

   hMixNorm->Scale(scale);
   hSubMixing->Add(hMixNorm,-1);


//   h1->Print("all");
   Double_t err;
   Double_t sigMin=1.019445-3*0.0045;
   Double_t sigMax=1.019445+3*0.0045;
   Double_t yield = hSubLikeSum->IntegralAndError(hSubLikeSum->FindBin(sigMin),hSubLikeSum->FindBin(sigMax),err);
   Printf("Mixing %f %f",yield, err);

   yield = hSubMixing->IntegralAndError(hSubMixing->FindBin(sigMin),hSubMixing->FindBin(sigMax),err);
   Printf("Mixing %f %f",yield, err);


   //    h1->Rebin(4);

//    h1->Fit("gaus", "", "", sigMin, sigMax);
//    h1->Draw();

   Printf("%s",GetFullPath().Data());
   hLikeSumNorm->SetName("LikeSumNorm");
   hSubLikeSum->SetName("Substracted_LikeSum");
   hMixNorm->SetName("MixNorm");
   hSubMixing->SetName("Substracted_Mixing");
   if (fFolder) {
      fFolder->Add(h1);
      fFolder->Add(h2);
      fFolder->Add(h3);
      fFolder->Add(h4);
      fFolder->Add(hLikeSumNorm);
      fFolder->Add(hSubLikeSum);
      fFolder->Add(hMixNorm);
      fFolder->Add(hSubMixing);
   }

}


//______________________________________________________________________________
void AliRsnTaskParticleYield::SetCuts(TArrayI *ids, TArrayD *mins, TArrayD *maxs)
{
   if (fCutIDs) delete fCutIDs;
   if (fCutMins) delete fCutMins;
   if (fCutMaxs) delete fCutMaxs;

   fCutIDs = new TArrayI(*ids);
   fCutMins = new TArrayD(*mins);
   fCutMaxs = new TArrayD(*maxs);
}
