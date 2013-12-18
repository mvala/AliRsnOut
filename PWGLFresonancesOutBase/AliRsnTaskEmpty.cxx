//
// *** Class AliRsnTaskEmpty ***
//
// Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#include "AliRsnTaskEmpty.h"

ClassImp(AliRsnTaskEmpty)

//______________________________________________________________________________
AliRsnTaskEmpty::AliRsnTaskEmpty(const char *name, const char *title) : AliRsnTask(name, title)
{
   //
   // Defauult constructor
   //

   // We need that current task is executed after sub-tasks
   fExecTaskBefore = kFALSE;
}

//______________________________________________________________________________
AliRsnTaskEmpty::AliRsnTaskEmpty(const AliRsnTaskEmpty &copy) : AliRsnTask(copy)
{
   //
   // Copy constructor
   //

}

//______________________________________________________________________________
AliRsnTaskEmpty &AliRsnTaskEmpty::operator=(const AliRsnTaskEmpty &copy)
{
   //
   // Assignment constructor
   //

   TTask::operator=(copy);
   if (this == &copy)
      return *this;

   return (*this);
}

//______________________________________________________________________________
AliRsnTaskEmpty::~AliRsnTaskEmpty()
{
   //
   // Destructor
   //
}

void AliRsnTaskEmpty::Exec(Option_t * /*option*/)
{
   // Put your task job
}
