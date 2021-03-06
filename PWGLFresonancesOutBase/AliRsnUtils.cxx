// Without this macro the THtml doc for TMath can not be generated
// #if !defined(R__ALPHA) && !defined(R__SOLARIS) && !defined(R__ACC) && !defined(R__FBSD)
// NamespaceImp(AliRsnUtils)
// #endif

#include <Riostream.h>
#include <TObject.h>
#include <TError.h>
#include <TString.h>
#include <TMath.h>
#include <TH1.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TSystem.h>
#include <TArrayI.h>
#include "AliRsnUtils.h"

//______________________________________________________________________________
TH1D *AliRsnUtils::GraphToHistogram(TGraphErrors *gr, Bool_t useGraphEY, Double_t min)
{
   //
   // Creating Histogram (TH1D) from TGraphErrors
   //

   // we return 0, if no grpah
   if ((!gr) || (!gr->GetN())) {
      ::Error("AliRsnUtils::GraphToHistogram", "Graph 'gr' is null or has no points !!!");
      return 0;
   }

   // Finding range of graph
   Double_t xMin, xMax, yMin, yMax;
   gr->ComputeRange(xMin, yMin, xMax, yMax);

   // check if min is less then xMax of graph
   if (min > xMax) {
      ::Error("AliRsnUtils::GraphToHistogram", "Value 'min' is higher then maximum of x axis of graph 'gr' !!!");
      return 0;
   }

   // clonig graph for internal useage
   gr = (TGraphErrors *) gr->Clone();

   // sorting graph, so x values are sorted correctly
   gr->Sort();

   // getting all values and errors from graph gr
   Double_t *x, *y, *ex, *ey;
   x = gr->GetX();
   y = gr->GetY();
   ex = gr->GetEX();
   ey = gr->GetEY();

   Bool_t isExNonZero = kTRUE;
   Int_t i;
   // check if ex is non-zero
   for (i = 0; i < gr->GetN() ; ++i) {
      if (ex[i] < 1e-15) {
         isExNonZero = kFALSE;
         break;
      }
   }

   // Creating array of bins for histogram
   Int_t nPoints = gr->GetN();
   Int_t nBins = nPoints + 1;
   Double_t bins[nBins];


   if (isExNonZero) {
      for (i = 0; i < nPoints ; ++i) {
         bins[i] = x[i] - ex[i];
      }
      bins[nPoints] = x[nPoints - 1] + ex[nPoints - 1];
   } else {
      ::Warning("AliRsnUtils::GraphToHistogram", "One of value ErrorX (ex) of graph 'gr' is zero !!!");
      ::Warning("AliRsnUtils::GraphToHistogram", TString::Format("We are going to guess histogram starting from min=%f !!!", min).Data());
      ::Warning("AliRsnUtils::GraphToHistogram", "Check if histogram is as you expected !!!");

      Double_t d;
      // setting low-edge of first bin
      bins[0] = min;
      for (i = 0; i < gr->GetN() ; ++i) {
         d = x[i] - min;
         min = x[i] + d;
         bins[i + 1] = x[i] + d;
         // TODO : Add check for range and so
      }

   }

   TH1D *h = new TH1D(TString::Format("%s", gr->GetName()).Data(), "", nBins - 1, bins);

   // FILL values
   for (i = 1; i <= h->GetNbinsX(); ++i) {
      h->SetBinContent(i, y[i - 1]);
      if (useGraphEY) h->SetBinError(i, ey[i - 1]);

   }

   // cleanup
   delete gr;

   return h;
}

//______________________________________________________________________________
TMultiGraph *AliRsnUtils::CreateGraphStatSysErr(const char *name, Color_t color, const char *filename, Double_t fac)
{
   //
   // Create mutli graph containing 2 graphs one showhing stat. errors and other with sys. errors.
   // Stat. errors are drawn with "P" and sys. errors with "E2"
   //

   TMultiGraph *mg = new TMultiGraph(name, "");
   if (!mg) return 0;

   TGraphErrors *stat = new TGraphErrors(filename, "%lg %lg %lg %lg");
   if (stat->IsZombie()) {
      ::Error("AliRsnUtils::CreateGraphStatSysErr", "Could not load stat error part !!!");
      return 0;
   }
   TGraphErrors *sys = new TGraphErrors(filename, "%lg %lg %lg %*lg %lg");
   if (sys->IsZombie()) {
      ::Error("AliRsnUtils::CreateGraphStatSysErr", "Could not load sys error part !!!");
      return 0;
   }

   for (Int_t i=0; i<sys->GetN(); i++) {
      sys->SetPointError(i,sys->GetErrorX(i)*fac,sys->GetErrorY(i));
   }

   stat->SetFillStyle(0);
   stat->SetMarkerColor(color);
   stat->SetLineColor(color);
   sys->SetFillStyle(0);
   sys->SetMarkerColor(color);
   sys->SetLineColor(color);

   mg->Add(stat,"P");
   mg->Add(sys,"E2");

   return mg;
}

//______________________________________________________________________________
Double_t *AliRsnUtils::GetRowFromTextFile(const char *inFile, Int_t row, Int_t numRows, Int_t &n)
{
   //
   // Get Row from Text File and save it to array
   // Value n will contain number of lines
   //

   n = 0;
   ifstream inNumLines;
   inNumLines.open(gSystem->ExpandPathName(inFile));
   std::string line;
   while (getline(inNumLines,line)) n++;
   inNumLines.close();
   ifstream in;
   in.open(gSystem->ExpandPathName(inFile));
   Double_t x[numRows];
   Double_t *val= new Double_t[n];
   Int_t c = 0;
   while (1) {
      for (Int_t i=0; i<numRows; i++) {
         in >> x[i];
         if (i+1 == row) val[c] = x[i];
      }
      // we need end of line
      if (!in.good()) break;
      c++;
   }
   in.close();
   return val;
}


//______________________________________________________________________________
void AliRsnUtils::MultiplyRowInTextFile(const char *inFile, const char *outfile, TArrayI *rows, Double_t factor)
{
   //
   // Multiply Row In TextFile
   //

   Double_t factors[1];
   factors[0] = factor;

   MultiplyRowInTextFile(inFile, outfile, rows, factors, 0);
}

//______________________________________________________________________________
void AliRsnUtils::MultiplyRowInTextFile(const char *inFile, const char *outfile, TArrayI *rows, Double_t *factors, Int_t numFactors)
{
   //
   // Multiply Row In TextFile
   //

   Int_t numRows = 0;

   ifstream testFile;
   testFile.open(gSystem->ExpandPathName(inFile));
   if (!testFile.is_open()) {
      ::Error("AliRsnUtils::MultiplyRowInTextFile", TString::Format("File '%s' could not be opened !!!", gSystem->ExpandPathName(inFile) ).Data());
      return;
   }
   std::string oneline;
   getline(testFile,oneline);
   TString str = oneline;
   numRows = str.Tokenize(" ")->GetEntries();
   testFile.close();

   Printf("Num rows %d",numRows);
   ifstream in;
   in.open(gSystem->ExpandPathName(inFile));
   ofstream out;
   out.open(gSystem->ExpandPathName(outfile));
   TString line;
   Double_t x[numRows];
   Int_t c = 0;
   while (1) {

      for (Int_t i=0; i<numRows; i++) {
         in >> x[i];
         for (Int_t iRow=0; iRow<rows->GetSize(); iRow++) {
            if (i+1==rows->At(iRow)) {
               // We are doing only one facrtor
               if (!numFactors) x[i] = factors[0]*x[i];
               // we are doing only limited number of factors
               else if (c < numFactors) x[i] = factors[c]*x[i];
            }
         }
      }

      // we need end of line
      if (!in.good()) break;

      line="";
      for (Int_t i=0; i<numRows; i++) {
         line += TString::Format("%e",x[i]).Data();
         if (i == numRows-1) line += "\n";
         else line += " ";
      }
      printf(line.Data());
      out << line.Data();
      c++;
   }
   in.close();
   out.close();

}
