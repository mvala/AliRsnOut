#ifndef __CINT__
#include <TArrayI.h>
#include <TString.h>
#include <AliRsnUtils.h>
#endif

void test()
{
   Printf("Starting test ...");
   Double_t nEventsSel = 6.68945e7;
   Double_t delta_pt = 0.5;
   Double_t delta_y = 1.0;
   // trigger eff 0.851 +7% - 3.5%
   Double_t effTrigger = 0.851;
   // Phi loss because of VertexCut We have to check
   // (for now 0.01 -> 1% as in 7TeV paper
   Double_t effVert = 0.909;
   // Branching ratio
   Double_t br = 0.489;

   Double_t fac = effTrigger / (delta_y * delta_pt * effVert * br * nEventsSel);



//   factor = 1.;
   TArrayI rows(2);
   rows.AddAt(2,0);
   rows.AddAt(4,1);

   AliRsnUtils::MultiplyRowInTextFile("~/Documents/Work/AnalysisNotes/2013/phi_pp_2.76/analysis/Viktor/PtComparison/Viktor/pt_R",
                                      "~/Documents/Work/AnalysisNotes/2013/phi_pp_2.76/analysis/Viktor/PtComparison/Viktor/pt_RF",&rows,fac);
//   factor = 2.;
//   AliRsnUtils::MultiplyRowInTextFile("/tmp/in.txt","/tmp/out1.txt",&rows,fac);
}
