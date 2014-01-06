#ifndef __CINT__
#include <TString.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TFolder.h>
#include <TFile.h>
#include <TH1D.h>
#include <AliRsnTaskInput.h>
#endif

void test()
{
   AliRsnTaskInput *input = new AliRsnTaskInput("input","Test title");
   input->SetCacheDir(gSystem->HomeDirectory());
   input->SetFileName("root://eos.saske.sk//eos/saske.sk/alice/rsn/PHIKK/LHC11a/ESD_pass4_without_SDD/RSN_20131015/Merged/All/STD2010/00_DEFAULT/KTPCnsig30/RsnOutput.root");
   input->Exec("");

   Int_t numCuts = 2;
   Int_t idProj = 0;
   TArrayI *ids = new TArrayI(numCuts);
   TArrayD *mins = new TArrayD(numCuts);
   TArrayD *maxs = new TArrayD(numCuts);

   Int_t idx = 0;

   // Rapodity
   ids->AddAt(2,idx); mins->AddAt(-0.5,idx); maxs->AddAt(0.5,idx); idx++;

   // Pt
   ids->AddAt(1,idx); mins->AddAt(0.0,idx); maxs->AddAt(5.0,idx); /*idx++;*/


   Double_t minPt=0.0;
   Double_t maxPt=5.0;
   Double_t stepPt = 0.2;
   stepPt = -1.0;
//   Double_t stepsPt[] = { 0.2,0.2,0.2,0.2,0.2,
//                          0.2,0.2,0.2,0.2,0.2,
//                          0.5,0.5,0.5,0.5,0.5,
//                          0.5,0.5,0.5,0.5,0.5};

   Double_t stepsPt[] = { 1.0, 1.0, 1.0, 1.0, 1.0 };

   Int_t idxPt = 1;
   Int_t i=0;
   TFolder *root = gROOT->GetRootFolder()->AddFolder("rsnMy","My Rsn");
   root->SetOwner();
   gROOT->GetListOfBrowsables()->Add(root);
   TFolder *currFolder;
   for (Double_t curMinPt=minPt; curMinPt<maxPt; curMinPt+=stepsPt[i],i++) {
      stepPt = stepsPt[i];
      Printf("[%.2f,%.2f]",curMinPt,curMinPt+stepPt);
      ids->SetAt(idxPt,idx); mins->SetAt(curMinPt,idx); maxs->SetAt(curMinPt+stepPt,idx);

      TH1D *h1 = input->CreateHistogram("Unlike",idProj,ids,mins,maxs);
      TH1D *h2 = input->CreateHistogram("LikePP",idProj,ids,mins,maxs);
      TH1D *h3 = input->CreateHistogram("LikeMM",idProj,ids,mins,maxs);
      TH1D *h4 = input->CreateHistogram("Mixing",idProj,ids,mins,maxs);

      currFolder = root->AddFolder(TString::Format("pt_%.2f_%.2f",curMinPt,curMinPt+stepPt).Data(),"");
      currFolder->Add(h1);
      currFolder->Add(h2);
      currFolder->Add(h3);
      currFolder->Add(h4);
      currFolder->SetOwner();
   }

   TFile *f = TFile::Open("outTest.root","RECREATE");
   root->Write();
   f->Close();
}
