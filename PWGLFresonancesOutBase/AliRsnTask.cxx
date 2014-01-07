//
// *** Class AliRsnTask ***
//
// TODO
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#include <TROOT.h>
#include <TString.h>
#include <TFolder.h>

#include "AliRsnTask.h"

ClassImp(AliRsnTask)

//______________________________________________________________________________
AliRsnTask::AliRsnTask(const char *name, const char *title) :
   TTask(name, title), fParent(0), fExecTaskBefore(kTRUE), fInput(0)
{
//
// Default constructor
//

}

//______________________________________________________________________________
AliRsnTask::AliRsnTask(const AliRsnTask &copy) :
   TTask(copy), fParent(copy.fParent), fExecTaskBefore(copy.fExecTaskBefore),fInput(copy.fInput)

{
//
// Copy constructor
//

}

//______________________________________________________________________________
AliRsnTask &AliRsnTask::operator=(const AliRsnTask &copy)
{
//
// Assignment constructor
//

   TTask::operator=(copy);
   if (this == &copy) return *this;

   fParent = copy.fParent;
   fExecTaskBefore = copy.fExecTaskBefore;
   fInput = copy.fInput;
   return (*this);
}

//______________________________________________________________________________
AliRsnTask::~AliRsnTask()
{
//
// Destructor
//

   fInput->Delete();
   
//    delete fInput;
//    delete fFolder;
}

//______________________________________________________________________________
void AliRsnTask::Add(TTask *task)
{
   TTask::Add(task);

   AliRsnTask *se = dynamic_cast<AliRsnTask *>(task);
   if (se) {
     if (!fParent) fFolder = gROOT->GetRootFolder()->AddFolder(GetName(),GetTitle());
     se->SetParent(this);
     se->SetFolder(GetFolder()->AddFolder(se->GetName(),se->GetTitle()));
  }
   
}

//______________________________________________________________________________
void AliRsnTask::Print(Option_t */*option*/) const
{

   // Creating spaces for prefix
   TString prefix;
   for (Int_t i = 0; i < GetLevel(); i++)
      prefix += "  ";

   // Printing info
   Printf("%sname='%s' title='%s' level=%d path=%s", prefix.Data(), GetName(), GetTitle(), GetLevel(), GetFullPath().Data());

   // Printing info of all subtasks
   TIter next(fTasks);
   AliRsnTask *se;
   while ((se = (AliRsnTask *) next())) {
      se->Print();
   }
}

//______________________________________________________________________________
Int_t AliRsnTask::GetLevel() const
{
   //
   // Calculate level
   //

   if (!fParent) return 0;

   Int_t level = 0;
   AliRsnTask *p = GetParent();
   while (p) {
      p = p->GetParent();
      level++;
   }
   return level;
}

//______________________________________________________________________________
TString AliRsnTask::GetFullPath(TString delim, Bool_t removeFirstChar) const
{
   //
   // Gets Full path to current task
   //

   // Get current name
   TString fp = GetName();
   fp.Prepend(delim.Data());

   AliRsnTask *p = GetParent();
   while (p) {
      fp.Prepend(p->GetName());
      fp.Prepend(delim.Data());
      p = p->GetParent();
   }

   if (removeFirstChar) fp.Remove(0, 1);

   return fp;
}

//__________________________________________________________________________________________________
AliRsnTask *AliRsnTask::GetListByPath(TString path) const
{

   // let's remove first '/'
   TString name = GetName();
   path.Remove(0, name.Length() + path.First('/') + 2);

   // let's remove last '/'
   if (path.Last('/') == path.Length()) path.Remove(path.Length() - 1, path.Length());

   // if path is null return current object
   if (path.IsNull()) return (AliRsnTask *) this;

   // let's find out current name
   TString curName = path(0, path.First('/'));

   // if curName is null, then path is our name
   if (curName.IsNull()) {
      curName = path;
      return (AliRsnTask *) fTasks->FindObject(curName.Data());
   }

   // let's remove current name from path
   path.Remove(0, curName.Length());

   // let' see if we have task in fTasks
   AliRsnTask *t = (AliRsnTask *) fTasks->FindObject(curName.Data());

   // if not we return 0
   if (!t) return 0;

   // if yes, let's continue searching
   return t->GetListByPath(path);
}

//______________________________________________________________________________
void AliRsnTask::ExecuteTask(Option_t *option)
{
   // Execute main task and its subtasks.
   // When calling this function, the Exec function of the corresponding class
   // is invoked, then the list of its subtasks is executed calling recursively
   // all the subtasks, etc.
   //
   // The option parameter may be used to select different execution steps
   // within a task. This parameter is passed also to all the subtasks.

   if (!fInput) fInput = new TList();
   if (!fParent && !fFolder) fFolder = gROOT->GetRootFolder()->AddFolder(GetName(),GetTitle());


   if (fgBeginTask) {
      Error("ExecuteTask", "Cannot execute task:%s, already running task: %s", GetName(), fgBeginTask->GetName());
      return;
   }
   if (!IsActive()) return;

   fOption = option;
   fgBeginTask = this;
   fgBreakPoint = 0;

   if (fBreakin) return;

   if (fExecTaskBefore) Exec(option);

   fHasExecuted = kTRUE;
   ExecuteTasks(option);

   if (!fExecTaskBefore) Exec(option);

   if (fBreakout) return;

   if (!fgBreakPoint) {
      fgBeginTask->CleanTasks();
      fgBeginTask = 0;
   }
}
