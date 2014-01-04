//
// *** Class AliRsnTaskNSigma ***
//
// Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#include <TH2.h>
#include <TFile.h>
#include <TList.h>
#include <TSystem.h>
#include <TFolder.h>
#include <RooFit.h>
#include <RooRealVar.h>
#include <RooPlot.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooFitResult.h>

#include "AliRsnTaskNSigma.h"

ClassImp(AliRsnTaskNSigma)

//______________________________________________________________________________
AliRsnTaskNSigma::AliRsnTaskNSigma(const char *name, const char *title) :
   AliRsnTask(name, title), fUseRooFit(kTRUE)
{
   //
   // Defauult constructor
   //

   // We need that current task is executed after sub-tasks
   fExecTaskBefore = kFALSE;

   fBinRange[0] = 0;
   fBinRange[1] = 1;
   fFitRange[0] = 3.0;
   fFitRange[1] = -3.0;
   fFitDrawRange[0] = 4.0;
   fFitDrawRange[1] = -4.0;
}

//______________________________________________________________________________
AliRsnTaskNSigma::AliRsnTaskNSigma(const AliRsnTaskNSigma &copy) :
   AliRsnTask(copy), fUseRooFit(copy.fUseRooFit)
{
   //
   // Copy constructor
   //

   fUseRooFit = copy.fUseRooFit;
   fBinRange[0] = copy.fBinRange[0];
   fBinRange[1] = copy.fBinRange[1];
   fFitRange[0] = copy.fFitRange[0];
   fFitRange[1] = copy.fFitRange[1];
   fFitDrawRange[0] = copy.fFitDrawRange[0];
   fFitDrawRange[1] = copy.fFitDrawRange[1];
}

//______________________________________________________________________________
AliRsnTaskNSigma &AliRsnTaskNSigma::operator=(const AliRsnTaskNSigma &copy)
{
   //
   // Assignment constructor
   //

   TTask::operator=(copy);
   if (this == &copy) return *this;
   fUseRooFit = copy.fUseRooFit;
   fBinRange[0] = copy.fBinRange[0];
   fBinRange[1] = copy.fBinRange[1];
   fFitRange[0] = copy.fFitRange[0];
   fFitRange[1] = copy.fFitRange[1];
   fFitDrawRange[0] = copy.fFitDrawRange[0];
   fFitDrawRange[1] = copy.fFitDrawRange[1];

   return (*this);
}

//______________________________________________________________________________
AliRsnTaskNSigma::~AliRsnTaskNSigma()
{
   //
   // Destructor
   //
}

void AliRsnTaskNSigma::Exec(Option_t * /*option*/)
{

   Printf("Testing Nsigma task %s",GetName());
   TFolder *folder = (TFolder *) fParent->GetInput()->FindObject("nsig");
   if (!folder) {
      folder = new TFolder("nsig", "Nsigma folder");
      fParent->GetInput()->Add(folder);
   }
   folder->Add(GetDrawObject());

}
TObject *AliRsnTaskNSigma::GetDrawObject()
{

//   TFile *curFile = gFile;
   TFile::SetCacheFileDir(gSystem->HomeDirectory());
   const char *fn = "root://eos.saske.sk//eos/saske.sk/alice/rsn/PHIKK/LHC11a/ESD_pass4_without_SDD/RSN_20131015/Merged/All/STD2010/00_DEFAULT/qualityonly/RsnOutput.root";
   TFile *f = TFile::Open(fn, "CACHEREAD");
//   f->ls();
   TList *l = (TList *) f->Get("ExtraInfo");


   TH2D *hNSigmaP = (TH2D *) l->FindObject("K_Phi_qualityonly_STD2010_PRIMARY_mon.TPC_nsigma_p_K");
   Double_t minVal = hNSigmaP->GetXaxis()->GetBinLowEdge(fBinRange[0]);
   Double_t maxVal = hNSigmaP->GetXaxis()->GetBinLowEdge(fBinRange[1]);

   TH1D *hNsigma = hNSigmaP->ProjectionY(TString::Format("[%.2f,%.2f]", minVal, maxVal).Data(), fBinRange[0], fBinRange[1]);
   hNsigma->GetXaxis()->SetRangeUser(fFitDrawRange[0], fFitDrawRange[1]);
//   curFile->cd();

   if (fUseRooFit) {
      using namespace RooFit;
      RooRealVar x("x", "x [nsigma]", fFitDrawRange[0], fFitDrawRange[1]);
      RooRealVar mean("mean", "Mean", -1.0, 1.0);
      RooRealVar sigma("sigma", "Sigma", 0.0, 3.0);
      RooGaussian gauss("gauss", "Gauss PDF", x, mean, sigma);

      RooDataHist *data = new RooDataHist("hNsig", "Hist Nsigma", x, hNsigma);

//   RooFitResult *r = gauss.fitTo(*data, Range(fFitRange[0], fFitRange[1]), Save());
      RooPlot *frame = x.frame();
      data->plotOn(frame, Name("data"));
      mean = 0.0;
      sigma = 1.0;
//      gauss.plotOn(frame, Range(-3, 3), LineColor(kRed),FillColor(kRed),FillStyle(3001),DrawOption("F"));
      gauss.plotOn(frame, Range(-3, 3), LineColor(kRed));
//      gauss.fitTo(*data,Range(fFitRange[0], fFitRange[1]),Extended());
      gauss.fitTo(*data, Range(fFitRange[0], fFitRange[1]));
      gauss.plotOn(frame, Name("model"), Range(fFitDrawRange[0], fFitDrawRange[1]));
      gauss.paramOn(frame, data);

      frame->Print();
      mean.Print();
      sigma.Print();
      x.Print();
      gauss.chi2FitTo(*data);

      frame->SetTitle(TString::Format("[%.2f,%.2f]", minVal, maxVal).Data());
//   r->Print();

//      Double_t chi2 = frame->chiSquare("gauss_Norm", "h_hNsig", 3);
//      Double_t chi2 = frame->chiSquare(2);
      Double_t chi2 = frame->chiSquare("model", "data", 2);
      Printf("Chi2 = %f", chi2);
      return frame;

   } else {
      hNsigma->Fit("gaus", "", "", fFitRange[0], fFitRange[1]);
      return hNsigma;
   }

   return 0;
}

void AliRsnTaskNSigma::SetParameters(Int_t binf, Int_t binl, Double_t fitMin, Double_t fitMax, Double_t min, Double_t max)
{
   fBinRange[0] = binf;
   fBinRange[1] = binl;
   fFitRange[0] = fitMin;
   fFitRange[1] = fitMax;
   fFitDrawRange[0] = min;
   fFitDrawRange[1] = max;
}
