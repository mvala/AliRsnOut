//
// *** Class AliRsnTask ***
//
//  TODO
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIRSNTASK_H
#define ALIRSNTASK_H

#include <TTask.h>

class TList;
class TFolder;
class AliRsnTask : public TTask
{
public:
   AliRsnTask(const char *name="rsnTask", const char *title="Rsn Task");
   AliRsnTask(const AliRsnTask &copy);
   AliRsnTask &operator=(const AliRsnTask &copy);
   virtual ~AliRsnTask();

   virtual void   Add(TTask *task);
   virtual void   Add(AliRsnTask *se) { Add((TTask *)se); }
   virtual void   Print(Option_t *option = "") const;
   virtual void   ExecuteTask(Option_t *option="");

   void           SetParent(AliRsnTask *se) { fParent = se; }
   AliRsnTask    *GetParent() const { return fParent; }
   Int_t          GetLevel() const;
   TString        GetFullPath(TString delim="/", Bool_t removeFirstChar = kFALSE) const;
   AliRsnTask    *GetListByPath(TString path="/") const;

   TList         *GetInput() const { return fInput;}
   void          SetFolder(TFolder *folder) { fFolder = folder;}
   TFolder       *GetFolder() const { return fFolder;}

protected:

   AliRsnTask *fParent;
   Bool_t      fExecTaskBefore;
   TList      *fInput;
   TFolder    *fFolder;

   ClassDef(AliRsnTask, 1)
};

#endif
