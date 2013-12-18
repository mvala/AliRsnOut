#ifndef __CINT__
#include <TString.h>
#include <TSystem.h>
#include <TStopwatch.h>
#endif

void MainRun(TString macro, TString macro_argc, TString projectDir = "")
{

   gSystem->Load("libGui.so");
   gSystem->Load("libRooFit");
   gSystem->Load("libPWGLFresonancesOutBase.so");

   Printf(projectDir.Data());
   gSystem->AddIncludePath(TString::Format("-I%sinclude", projectDir.Data()).Data());
   TStopwatch timer;
   timer.Start();

   if (!macro.IsNull()) {
      if (macro.EndsWith(".C")) {
         gROOT->ProcessLine(TString::Format(".L %s+", macro.Data()).Data());
         macro.ReplaceAll(".C","");
         gROOT->ProcessLine(TString::Format("%s(%s)", macro.Data(),macro_argc.Data()).Data());
      }
   }

   timer.Stop();
   timer.Print();

   Printf("Working dir: '%s'", gSystem->WorkingDirectory());
}
