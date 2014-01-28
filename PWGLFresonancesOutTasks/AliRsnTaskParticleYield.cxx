//
// *** Class AliRsnTaskParticleYield ***
//
// Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#include "AliRsnTaskParticleYield.h"
#include <TError.h>
#include <TCollection.h>
#include <TH1D.h>
#include <TObjArray.h>
#include <TList.h>
#include <TFolder.h>
#include <TObjString.h>
#include <RooRealVar.h>
#include <RooVoigtian.h>
#include <RooChebychev.h>
#include <RooPolynomial.h>
#include <RooPlot.h>
#include <RooDataHist.h>
#include <RooExtendPdf.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>

ClassImp(AliRsnTaskParticleYield)

//______________________________________________________________________________
AliRsnTaskParticleYield::AliRsnTaskParticleYield(const char *name, const char *title) : AliRsnTask(name, title)
{
   //
   // Defauult constructor
   //

   for (Int_t iType=0; iType<kAll; iType++) {
      fActionType[iType] = kSum;
      fHistosStr[iType] = "";
      fHist[iType] = 0;
   }
   for (Int_t i=0; i<2; i++) {
      fNorm[i] = 0.0;
      fFit[i] = 0.0;
   }

   // We need that current task is executed after sub-tasks
//    fExecTaskBefore = kFALSE;
}

//______________________________________________________________________________
AliRsnTaskParticleYield::AliRsnTaskParticleYield(const AliRsnTaskParticleYield &copy) : AliRsnTask(copy)
{
   //
   // Copy constructor
   //
   for (Int_t iType=0; iType<kAll; iType++) {
      fActionType[iType] = copy.fActionType[iType];
      fHistosStr[iType] = copy.fHistosStr[iType];
      fHist[iType] = copy.fHist[iType];
   }
   for (Int_t i=0; i<2; i++) {
      fNorm[i] = copy.fNorm[i];
      fFit[i] = copy.fFit[i];
   }
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

   for (Int_t iType=0; iType<kAll; iType++) {
      fActionType[iType] = copy.fActionType[iType];
      fHistosStr[iType] = copy.fHistosStr[iType];
      fHist[iType] = copy.fHist[iType];
   }

   for (Int_t i=0; i<2; i++) {
      fNorm[i] = copy.fNorm[i];
      fFit[i] = copy.fFit[i];
   }
   return (*this);
}

//______________________________________________________________________________
AliRsnTaskParticleYield::~AliRsnTaskParticleYield()
{
   //
   // Destructor
   //

//    for (Int_t iType=0; iType<kAll; iType++) {
//       delete fHist[iType];
//    }
}

//______________________________________________________________________________
void AliRsnTaskParticleYield::Exec(Option_t * /*option*/)
{
   //
   // Put your task job
   //

   Printf("Exec Yield %s ...",GetName());

   TH1D *hScaled = (TH1D *) fHist[kBkg]->Clone();
   TH1D *hSub = (TH1D *) fHist[kSigBkg]->Clone();

   // Normalize
//    fNorm[0] = 1.045;
//    fNorm[1] = 1.065;
//    fNorm[0] = 0.995;
//    fNorm[1] = 1.005;
   Double_t scale = fHist[kSigBkg]->Integral(fHist[kSigBkg]->FindBin(fNorm[0]),fHist[kSigBkg]->FindBin(fNorm[1]))/
                    fHist[kBkg]->Integral(fHist[kBkg]->FindBin(fNorm[0]),fHist[kBkg]->FindBin(fNorm[1]));
   hScaled->Scale(scale);
   hSub->Add(hScaled,-1);
   ShiftHistogram(hSub);


   if (fFolder) {
      fFolder->Add(hScaled);
      fFolder->Add(hSub);
   }
//    fFit[0] = 1.019445-3*0.0045;
//    fFit[1] = 1.019445+3*0.0045;

   DoFit(hSub,fFit[0],fFit[1]);

}

//______________________________________________________________________________
void AliRsnTaskParticleYield::GenerateHistograms(TList *l)
{
   //
   // Adding Signal and Background
   //

   if (!l) return;
   TString histos;
   TString name="hBAD";
   for (Int_t iType=0; iType<kAll; iType++) {
      if (fHist[iType]) { delete fHist[iType]; fHist[iType] = 0; }

      histos = fHistosStr[iType];
      if (iType == kSigBkg) name = "hSigBkg";
      else if (iType == kBkg) name = "hBkg";
      TObjArray *oa = histos.Tokenize(",");
      switch (fActionType[iType]) {
         case kSum:
            TH1D *h;
            TObjString *os;
            for (Int_t i=0; i<oa->GetEntries(); i++) {
               os = (TObjString *) oa->At(i);
               Printf("Finding hist %s",os->GetString().Data());
               h = (TH1D *) l->FindObject(os->GetString().Data());
               if (!h) continue;
               if (!fHist[iType]) {
                  fHist[iType] = (TH1D *) h->Clone();
                  fHist[iType]->SetDirectory(0);
                  fHist[iType]->SetName(name.Data());
               } else {
                  fHist[iType]->Add(h);
               }
            }
            break;
         default:
            Error("AliRsnTaskParticleYield::AddHistograms","Action is not supported");
            return;
      }
   }

}

//______________________________________________________________________________
void AliRsnTaskParticleYield::ShiftHistogram(TH1 *h)
{
//    Printf("Shifting %f",h->GetMinimum());
   Double_t mymin = h->GetMinimum();
   if (mymin<0) {
      for (Int_t i=1; i< h->GetXaxis()->GetNbins()+1; i++) {
         h->SetBinContent(i,h->GetBinContent(i)-mymin);
      }

   }
}

//______________________________________________________________________________
void AliRsnTaskParticleYield::DoFit(TH1 *h, Double_t min, Double_t max)
{
//    RooRealVar x("x","m_{ES} (GeV)",h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
   RooRealVar x("x","m_{ES} (GeV)",0.99,h->GetXaxis()->GetXmax());
   RooDataHist data("data","data",x,RooFit::Import(*h));
   // --- Observable ---
   RooRealVar mean("mean","mean",1.019,1.017,1.021);
   RooRealVar width("width","width",0.0045,0.0010,0.0150);
   RooRealVar sigma("sigma","sigma",0.001,0.001,0.003);
   RooVoigtian sig("sig","signal p.d.f.",x,mean,width,sigma);

//    RooRealVar c0("c0","coefficient #0", 0.0,-1.,1.);
//    RooRealVar c1("c1","coefficient #1", 0.0,-1.,1.);
//    RooRealVar c2("c2","coefficient #2", 0.0,-1.,1.);
   RooRealVar c0("c0","coefficient #0", 0.0,-1000.,1000.);
   RooRealVar c1("c1","coefficient #1", 0.0,-1000.,1000.);
   RooRealVar c2("c2","coefficient #2", 0.0,-1000.,1000.);

//    RooPolynomial bkg("bkg","background p.d.f.",x,RooArgList(c0));
//    RooPolynomial bkg("bkg","background p.d.f.",x,RooArgList(c0,c1));
   RooPolynomial bkg("bkg","background p.d.f.",x,RooArgList(c0,c1,c2));

//    RooChebychev bkg("bkg","background p.d.f.",x,RooArgList(c0));
//    RooChebychev bkg("bkg","background p.d.f.",x,RooArgList(c0,c1));
//    RooChebychev bkg("bkg","background p.d.f.",x,RooArgList(c0,c1,c2));

//    RooRealVar fsig("fsig","signal fraction",0.5,0.,1.);
// // model(x) = fsig*sig(x) + (1-fsig)*bkg(x)
//    RooAddPdf model("model","model",RooArgList(sig,bkg),fsig);


//    x.setRange("window",1.01,1.03);
//    RooRealVar nsigw("nsigw","nsignal in window",500,0,1e7);
//    RooExtendPdf esig("esig","esig",sig,nsigw,"window");


   RooRealVar nsig("nsig","nsignal",500,0,1e7);
   RooExtendPdf esig("esig","esig",sig,nsig);
   RooRealVar nbkg("nbkg","nbackground",500,0,1e7);
   RooExtendPdf ebkg("ebkg","ebkg",bkg,nbkg);
   RooAddPdf model("model","model",RooArgList(esig,ebkg));
//    RooRealVar nsig("nsig","signal fraction",500,0.,100000.);
//    RooRealVar nbkg("nbkg","background fraction",500,0.,100000.);
//    RooAddPdf model("model","model",RooArgList(sig,bkg),RooArgList(nsig,nbkg));

   sigma.setConstant();

//    model.fitTo(data, RooFit::SumW2Error(kFALSE),RooFit::Range(1.0,1.04));
//    model.fitTo(data,RooFit::SumW2Error(kTRUE), RooFit::Range(1.,1.05));
//    model.fitTo(data, RooFit::PrintLevel(-1), RooFit::Range(1.,1.06), RooFit::SumW2Error(kFALSE));
   RooFitResult *fr = model.fitTo(data, RooFit::Range(min,max), RooFit::PrintLevel(-1),RooFit::Extended(kTRUE),RooFit::Save());
//    model.fitTo(data);
//    RooFitResult *fr = model.fitTo(data, RooFit::Extended(kTRUE),RooFit::Save());
   if (fr) fr->SetName(TString::Format("FitResult_%s",h->GetName()));
//    mean.Print();
//    width.Print();
//    sigma.Print();
//    if (fr) fr->Print();
//    else return 0;


   RooPlot *xframe = x.frame();
   xframe->SetName(TString::Format("frame_%s",h->GetName()));
   data.plotOn(xframe);
   model.plotOn(xframe);
   model.plotOn(xframe,RooFit::Components(bkg),RooFit::LineStyle(kDashed));
   model.paramOn(xframe,&data);
   if (fFolder) {
      if (fr) fFolder->Add(fr);
      if (xframe) fFolder->Add(xframe);
   }

}

//______________________________________________________________________________
void AliRsnTaskParticleYield::SetHistograms(AliRsnTaskParticleYield::EHistType type, TString histos, AliRsnTaskParticleYield::EHistAction action)
{
   fActionType[type] = action;
   fHistosStr[type] = histos;
}

//______________________________________________________________________________
void AliRsnTaskParticleYield::SetFitRange(Double_t min, Double_t max)
{
   fFit[0] = min;
   fFit[1] = max;
}

//______________________________________________________________________________
void AliRsnTaskParticleYield::SetNormalizationRange(Double_t min, Double_t max)
{
   fNorm[0] = min;
   fNorm[1] = max;

}
