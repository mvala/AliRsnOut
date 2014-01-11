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
#include <RooRealVar.h>
#include <RooVoigtian.h>
#include <RooChebychev.h>
#include <RooPolynomial.h>
#include <RooPlot.h>
#include <RooDataHist.h>
#include <RooExtendPdf.h>
#include <RooAddPdf.h>


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

   TH1D *hUnlike = input->CreateHistogram("Unlike",fIDProjection,fCutIDs,fCutMins,fCutMaxs);
   TH1D *hLikePP = input->CreateHistogram("LikePP",fIDProjection,fCutIDs,fCutMins,fCutMaxs);
   TH1D *hLikeMM = input->CreateHistogram("LikeMM",fIDProjection,fCutIDs,fCutMins,fCutMaxs);
   TH1D *hMixing = input->CreateHistogram("Mixing",fIDProjection,fCutIDs,fCutMins,fCutMaxs);

   hUnlike->Print();
   hLikePP->Print();
   hLikeMM->Print();
   hMixing->Print();

   TH1D *hSubLikeSum = (TH1D *)hUnlike->Clone();
   TH1D *hLikeSumNorm = (TH1D *)hLikePP->Clone();
   hLikeSumNorm->Add(hLikeMM);

   TH1D *hSubMixing = (TH1D *)hUnlike->Clone();
   TH1D *hMixNorm = (TH1D *)hMixing->Clone();;


   // Choose backgrouns and apply action

   // Normalize
   Double_t normMin = 1.045;
   Double_t normMax = 1.065;
//    normMin = 0.995;
//    normMax = 1.005;
   Double_t scale = hSubLikeSum->Integral(hSubLikeSum->FindBin(normMin),hSubLikeSum->FindBin(normMax))/
                    hLikeSumNorm->Integral(hLikeSumNorm->FindBin(normMin),hLikeSumNorm->FindBin(normMax));
   hLikeSumNorm->Scale(scale);
   hSubLikeSum->Add(hLikeSumNorm,-1);
   ShiftHistogram(hSubLikeSum);

   scale = hSubMixing->Integral(hSubMixing->FindBin(normMin),hSubMixing->FindBin(normMax))/
           hMixNorm->Integral(hMixNorm->FindBin(normMin),hMixNorm->FindBin(normMax));
   hMixNorm->Scale(scale);
   hSubMixing->Add(hMixNorm,-1);
   ShiftHistogram(hSubMixing);

   Double_t err;
   Double_t sigMin=1.019445-3*0.0045;
   Double_t sigMax=1.019445+3*0.0045;
   Double_t yield = hSubLikeSum->IntegralAndError(hSubLikeSum->FindBin(sigMin),hSubLikeSum->FindBin(sigMax),err);
   Printf("LikeSum %f %f",yield, err);

   yield = hSubMixing->IntegralAndError(hSubMixing->FindBin(sigMin),hSubMixing->FindBin(sigMax),err);
   Printf("Mixing %f %f",yield, err);


   //    hUnlike->Rebin(4);

//    hUnlike->Fit("gaus", "", "", sigMin, sigMax);
//    hUnlike->Draw();

   Printf("%s",GetFullPath().Data());
   hLikeSumNorm->SetName("LikeSumNorm");
   hSubLikeSum->SetName("Substracted_LikeSum");
   hMixNorm->SetName("MixNorm");
   hSubMixing->SetName("Substracted_Mixing");

//    RooPlot *frameLikeSum = DoFit(hSubLikeSum);
   RooPlot *frameMix = DoFit(hSubMixing);

   if (fFolder) {
      fFolder->Add(hUnlike);
      fFolder->Add(hLikePP);
      fFolder->Add(hLikeMM);
      fFolder->Add(hMixing);
      fFolder->Add(hLikeSumNorm);
      fFolder->Add(hSubLikeSum);
      fFolder->Add(hMixNorm);
      fFolder->Add(hSubMixing);
//       fFolder->Add(frameLikeSum);
      fFolder->Add(frameMix);
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

//______________________________________________________________________________
RooPlot *AliRsnTaskParticleYield::DoFit(TH1 *h)
{
//    RooRealVar x("x","m_{ES} (GeV)",h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax()) ;
   RooRealVar x("x","m_{ES} (GeV)",0.99,h->GetXaxis()->GetXmax()) ;
   RooDataHist data("data","data",x,RooFit::Import(*h)) ;
   // --- Observable ---
   RooRealVar mean("mean","mean",1.019,1.017,1.021) ;
   RooRealVar width("width","width",0.0045,0.0010,0.0150) ;
   RooRealVar sigma("sigma","sigma",0.001,0.001,0.003) ;
   RooVoigtian sig("sig","signal p.d.f.",x,mean,width,sigma) ;

//    RooRealVar c0("c0","coefficient #0", 0.0,-1.,1.) ;
//    RooRealVar c1("c1","coefficient #1", 0.0,-1.,1.) ;
//    RooRealVar c2("c2","coefficient #2", 0.0,-1.,1.) ;
   RooRealVar c0("c0","coefficient #0", 0.0,-1000.,1000.) ;
   RooRealVar c1("c1","coefficient #1", 0.0,-1000.,1000.) ;
   RooRealVar c2("c2","coefficient #2", 0.0,-1000.,1000.) ;

//    RooPolynomial bkg("bkg","background p.d.f.",x,RooArgList(c0)) ;
//    RooPolynomial bkg("bkg","background p.d.f.",x,RooArgList(c0,c1)) ;
   RooPolynomial bkg("bkg","background p.d.f.",x,RooArgList(c0,c1,c2)) ;

//    RooChebychev bkg("bkg","background p.d.f.",x,RooArgList(c0)) ;
//    RooChebychev bkg("bkg","background p.d.f.",x,RooArgList(c0,c1)) ;
//    RooChebychev bkg("bkg","background p.d.f.",x,RooArgList(c0,c1,c2)) ;

//    RooRealVar fsig("fsig","signal fraction",0.5,0.,1.) ;
// // model(x) = fsig*sig(x) + (1-fsig)*bkg(x)
//    RooAddPdf model("model","model",RooArgList(sig,bkg),fsig);


   x.setRange("window",1.01,1.03) ;
   RooRealVar nsigw("nsigw","nsignal in window",500,0,1e7) ;
   RooExtendPdf esig("esig","esig",sig,nsigw,"window") ;


//    RooRealVar nsig("nsig","nsignal",500,0,1e7) ;
//    RooExtendPdf esig("esig","esig",sig,nsig) ;
   RooRealVar nbkg("nbkg","nbackground",500,0,1e7) ;
   RooExtendPdf ebkg("ebkg","ebkg",bkg,nbkg) ;
   RooAddPdf model("model","model",RooArgList(esig,ebkg)) ;
//    RooRealVar nsig("nsig","signal fraction",500,0.,100000.) ;
//    RooRealVar nbkg("nbkg","background fraction",500,0.,100000.) ;
//    RooAddPdf model("model","model",RooArgList(sig,bkg),RooArgList(nsig,nbkg)) ;

   sigma.setConstant();

//    model.fitTo(data, RooFit::SumW2Error(kFALSE),RooFit::Range(1.0,1.04)) ;
//    model.fitTo(data,RooFit::SumW2Error(kTRUE), RooFit::Range(1.,1.05)) ;
//    model.fitTo(data, RooFit::PrintLevel(-1), RooFit::Range(1.,1.06), RooFit::SumW2Error(kFALSE)) ;
//    model.fitTo(data, RooFit::Range(0.99,1.06), RooFit::PrintLevel(-1),RooFit::Extended(kTRUE)) ;
//    model.fitTo(data);
   model.fitTo(data, RooFit::Extended(kTRUE));
   mean.Print();
   width.Print();
   sigma.Print();

   RooPlot *xframe = x.frame() ;
   xframe->SetName(TString::Format("frame_%s",h->GetName()));
   data.plotOn(xframe) ;
   model.plotOn(xframe);
   model.plotOn(xframe,RooFit::Components(bkg),RooFit::LineStyle(kDashed)) ;
   model.paramOn(xframe,&data);

   return xframe;
}

//______________________________________________________________________________
void AliRsnTaskParticleYield::ShiftHistogram(TH1 *h)
{
   Printf("%f",h->GetMinimum());
   Double_t mymin = h->GetMinimum();
   if (mymin<0) {
      for (Int_t i=1; i< h->GetXaxis()->GetNbins()+1; i++) {
         h->SetBinContent(i,h->GetBinContent(i)-mymin);
      }

   }
}
