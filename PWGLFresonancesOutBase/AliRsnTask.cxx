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
#include <TCollection.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <Rtypes.h>

#include "AliRsnTask.h"

ClassImp(AliRsnTask)

//______________________________________________________________________________
AliRsnTask::AliRsnTask(const char *name, const char *title) : TTask(name, title),
   fParent(0),
   fExecTaskBefore(kTRUE),
   fInput(0),
   fFolder(0)
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

//    if (!fParent && !fFolder) fFolder = gROOT->GetRootFolder()->AddFolder(GetName(),GetTitle());
//    else if (fParent && !fFolder) fFolder = fParent->GetFolder()->AddFolder(GetName(),GetTitle());

   AliRsnTask *se = dynamic_cast<AliRsnTask *>(task);
   if (se) {
      se->SetParent(this);

   }
//       TString name = se->GetName();
//       if (name.Contains("/")) {
//          TObjArray *folders = name.Tokenize("/");
//          TFolder *folder = fFolder;
//          TObjString *str;
//          for( int i=0; i<folders->GetEntries(); ++i ) {
//             str = (TObjString *)folders->At(i);
//             TIter next(folder->GetListOfFolders());
//             TFolder *folderTmp;
//             Bool_t found = kFALSE;
//             while ((folderTmp = (TFolder *)next())) {
//                if (!str->GetString().CompareTo(folderTmp->GetName())) {
//                   folder = folderTmp;
//                   found = kTRUE;
//                   break;
//                }
//             }
//             if (found) continue;
//             folder = folder->AddFolder(str->GetString().Data(),se->GetTitle());
//             se->SetFolder(folder);
//          }
//          name.ReplaceAll("/","_");
//          se->SetName(name.Data());
//       } else {
// //          name.ReplaceAll("/","_");
// //          Printf("Creating folder %s %s",fFolder->GetName(),se->GetFolder()->GetName());
//          Printf("Creating folder %s %s",fFolder->GetName(),name.Data());
//          se->SetFolder(fFolder->AddFolder(se->GetName(),se->GetTitle()));
//       }
//    }
}

//______________________________________________________________________________
void AliRsnTask::InitOutput()
{

   if (!fParent) fFolder = gROOT->GetRootFolder()->AddFolder(GetName(),GetTitle());
   else {
      CreateDirFromName();
   }
   TIter next(fTasks);
   AliRsnTask *se;
   while ((se = (AliRsnTask *) next())) {
      se->InitOutput();
   }

}

//______________________________________________________________________________
void AliRsnTask::CreateDirFromName()
{
   TString name = GetName();
   if (name.Contains("/")) {
      TObjArray *folders = name.Tokenize("/");
      TFolder *folder = fParent->GetFolder();
      if (!folder) return;
      TObjString *str;
      for( int i=0; i<folders->GetEntries(); ++i ) {
         str = (TObjString *)folders->At(i);
         TIter next(folder->GetListOfFolders());
         TFolder *folderTmp;
         Bool_t found = kFALSE;
         while ((folderTmp = (TFolder *)next())) {
            if (!str->GetString().CompareTo(folderTmp->GetName())) {
               folder = folderTmp;
               found = kTRUE;
            }
         }
         if (found) continue;
         folder = folder->AddFolder(str->GetString().Data(),GetTitle());
         SetFolder(folder);
      }
      name.ReplaceAll("/","_");
      SetName(name.Data());
   } else {
      SetFolder(fParent->GetFolder()->AddFolder(GetName(),GetTitle()));
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
   if (!fParent) InitOutput();

   if (fgBeginTask) {
      Error("ExecuteTask", "Cannot execute task:%s, already running task: %s", GetName(), fgBeginTask->GetName());
      return;
   }
   if (!IsActive()) return;

   fOption = option;
   fgBeginTask = this;
   fgBreakPoint = 0;

   if (fBreakin) return;

   if (fExecTaskBefore) {
      Exec(option);
      if (fExecTaskBefore) fHasExecuted = kTRUE;
   }

   ExecuteTasks(option);

   if (!fExecTaskBefore) {
      Exec(option);
      fHasExecuted = kTRUE;
   }

   if (fBreakout) return;

   if (!fgBreakPoint) {
      fgBeginTask->CleanTasks();
      fgBeginTask = 0;
   }
}

void AliRsnTask::ExecuteTasks(Option_t *option)
{
   // Execute all the subtasks of a task.

   TIter next(fTasks);
   AliRsnTask *task;
   while((task=(AliRsnTask *)next())) {
      if (fgBreakPoint) return;
      if (!task->IsActive()) continue;
      if (task->fHasExecuted) {
         task->ExecuteTasks(option);
         continue;
      }
      if (task->fBreakin == 1) {
         printf("Break at entry of task: %s\n",task->GetName());
         fgBreakPoint = this;
         task->fBreakin++;
         return;
      }

//       if (gDebug > 1) {
//          TROOT::IndentLevel();
//          cout<<"Execute task:"<<task->GetName()<<" : "<<task->GetTitle()<<endl;
//          TROOT::IncreaseDirLevel();
//       }

      if (task->fExecTaskBefore) {
         task->Exec(option);
         if (task->fExecTaskBefore) task->fHasExecuted = kTRUE;
      }

      task->ExecuteTasks(option);

      if (!task->fExecTaskBefore) {
         task->Exec(option);
         task->fHasExecuted = kTRUE;
      }

      if (gDebug > 1) TROOT::DecreaseDirLevel();
      if (task->fBreakout == 1) {
         printf("Break at exit of task: %s\n",task->GetName());
         fgBreakPoint = this;
         task->fBreakout++;
         return;
      }
   }
}
