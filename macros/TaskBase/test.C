#ifndef __CINT__
#include <TString.h>
#include <TFile.h>
#include <TROOT.h>
#include <TBrowser.h>
#include <AliRsnTask.h>
#endif

void test()
{
   Printf("Starting test ...");
   AliRsnTask *main = new AliRsnTask("RsnTaskMain","Root of RsnTask");

   AliRsnTask *t1 = new AliRsnTask("t1","Test 1");
   AliRsnTask *t11 = new AliRsnTask("t11","Test 1 1");
   AliRsnTask *t12 = new AliRsnTask("t12","Test 1 2");
   t1->Add(t11);
   t1->Add(t12);

   AliRsnTask *t2 = new AliRsnTask("t2","Test 2");
   AliRsnTask *t21 = new AliRsnTask("t21","Test 2 1");
   AliRsnTask *t22 = new AliRsnTask("t22","Test 2 2");
   t2->Add(t21);
   t2->Add(t22);

   main->Add(t1);
   main->Add(t2);

   // main->Print();

   TTask *t = main->GetListByPath("/RsnTaskMain/t1");
   // TTask *t = main->GetListByPath("/RsnTaskMain/t1/t12");
   if (t) t->Print();

//   return;

   main->ExecuteTask();

   TFile *f = TFile::Open("test.root","RECREATE");
   main->Write();
   f->Close();

   gROOT->GetListOfBrowsables()->Add(main,main->GetName());

   new TBrowser;
}
