#ifndef __CINT__
#include <TString.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TFolder.h>
#include <TFile.h>
#include <TH1D.h>
#include <AliRsnTaskInput.h>
#include <AliRsnTaskSpectraBin.h>
#include <AliRsnTaskParticleYield.h>
#endif

TList *CreateVariations();

TObject *test2()
{
   AliRsnTaskInput *input = new AliRsnTaskInput("Phi","Test title");
   input->SetCacheDir(gSystem->HomeDirectory());
   input->SetFileName("root://eos.saske.sk//eos/saske.sk/alice/rsn/PHIKK/LHC11a/ESD_pass4_without_SDD/RSN_20131015/Merged/All/STD2010/00_DEFAULT/KTPCnsig30/RsnOutput.root");
//    input->SetFileName("root://eos.saske.sk//eos/saske.sk/alice/rsn/PHIKK/LHC11a/ESD_pass4_without_SDD/RSN_20131015/Merged/All/STD2010/00_DEFAULT/qualityonly/RsnOutput.root");

   Int_t numCuts = 10;
   Int_t idProj = 0;
   TArrayI *ids = new TArrayI(numCuts);
   TArrayD *mins = new TArrayD(numCuts);
   TArrayD *maxs = new TArrayD(numCuts);

   Int_t idx = 0;
   // Default cuts
   // Rapidity
   ids->AddAt(2,idx); mins->AddAt(-0.5,idx); maxs->AddAt(0.5,idx); idx++;
   // Multiplicity
//    ids->AddAt(4,idx); mins->AddAt(0,idx); maxs->AddAt(10,idx); idx++;
   // Pt
   ids->AddAt(1,idx); mins->AddAt(0.0,idx); maxs->AddAt(5.0,idx); /*idx++;*/


   TList *lVariations = CreateVariations();

   Double_t minPt=1.0;
   Double_t maxPt=2.0;
   Double_t stepPt = 0.1;
   Int_t maxNumBisns = 100;
   TArrayD stepsPt(maxNumBisns);
   stepsPt.AddAt(0.2,0);
//    stepsPt.AddAt(0.5,10);
//    stepsPt.AddAt(1.0,0);
//   stepsPt.AddAt(2.0,3);
//    stepsPt.AddAt(2.5,0);
//    stepsPt.AddAt(5.0,0);

   Int_t idxPt = 1;
   Int_t i=0;
   AliRsnTaskSpectraBin *phi;

   Double_t smallVal = 1e-5;
   if (stepsPt.At(0)>smallVal) stepPt = stepsPt.At(0);
   for (Double_t curMinPt=minPt; curMinPt<maxPt-(stepPt/2); curMinPt+=stepPt,++i) {
      if (stepsPt.At(i)>1e-5) stepPt = stepsPt.At(i);
      Printf("[%.2f,%.2f]",curMinPt,curMinPt+stepPt);
      ids->SetAt(idxPt,idx); mins->SetAt(curMinPt,idx); maxs->SetAt(curMinPt+stepPt,idx);
      phi = new AliRsnTaskSpectraBin(TString::Format("0-10/PT/%.2f_%.2f",curMinPt,curMinPt+stepPt),"");
      phi->SetIDProjection(idProj);
      phi->SetCuts(ids,mins,maxs);
      input->Add(phi);
      // it has to be after input->Add() function
      phi->AddVariations(lVariations);

      if (stepsPt.At(i)>smallVal) stepPt = stepsPt.At(i);
   }

//   input->Exec("");
   input->ExecuteTask();

   gROOT->GetListOfBrowsables()->Add(input->GetFolder());
//    TFile *f = TFile::Open("outTest.root","RECREATE");
//    input->GetFolder()->Write();
//    f->Close();

   return input;

}

TList *CreateVariations()
{
   TList *lVariations = new TList();
   AliRsnTaskParticleYield *py= new AliRsnTaskParticleYield("DEFAULT");
   py->SetNormalizationRange(1.045,1.065);
   py->SetFitRange(1.0,1.04);
   py->SetHistograms(AliRsnTaskParticleYield::kSigBkg,"Unlike",AliRsnTaskParticleYield::kSum);
   py->SetHistograms(AliRsnTaskParticleYield::kBkg,"Mixing",AliRsnTaskParticleYield::kSum);
   lVariations->Add(py);
   py = (AliRsnTaskParticleYield *) py->Clone();
   py->SetName("NORM1");
   py->SetNormalizationRange(1.035,1.075);
   py->SetFitRange(1.0,1.04);
   py->SetHistograms(AliRsnTaskParticleYield::kSigBkg,"Unlike",AliRsnTaskParticleYield::kSum);
   py->SetHistograms(AliRsnTaskParticleYield::kBkg,"Mixing",AliRsnTaskParticleYield::kSum);
   lVariations->Add(py);
   py = (AliRsnTaskParticleYield *) py->Clone();
   py->SetName("NORM2");
   py->SetNormalizationRange(1.035,1.085);
   py->SetFitRange(1.0,1.04);
   py->SetHistograms(AliRsnTaskParticleYield::kSigBkg,"Unlike",AliRsnTaskParticleYield::kSum);
   py->SetHistograms(AliRsnTaskParticleYield::kBkg,"Mixing",AliRsnTaskParticleYield::kSum);
   lVariations->Add(py);
   py = (AliRsnTaskParticleYield *) py->Clone();
   py->SetName("FIT1");
   py->SetNormalizationRange(1.045,1.065);
   py->SetFitRange(1.01,1.03);
   py->SetHistograms(AliRsnTaskParticleYield::kSigBkg,"Unlike",AliRsnTaskParticleYield::kSum);
   py->SetHistograms(AliRsnTaskParticleYield::kBkg,"Mixing",AliRsnTaskParticleYield::kSum);
   lVariations->Add(py);

   py->SetName("LikeSign");
   py->SetNormalizationRange(1.045,1.065);
   py->SetFitRange(1.0,1.04);
   py->SetHistograms(AliRsnTaskParticleYield::kSigBkg,"Unlike",AliRsnTaskParticleYield::kSum);
   py->SetHistograms(AliRsnTaskParticleYield::kBkg,"LikePP,LikeMM",AliRsnTaskParticleYield::kSum);
   lVariations->Add(py);

   return lVariations;
}