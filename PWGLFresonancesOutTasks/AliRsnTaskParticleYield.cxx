//
// *** Class AliRsnTaskParticleYield ***
//
// Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#include "AliRsnTaskParticleYield.h"

ClassImp(AliRsnTaskParticleYield)

//______________________________________________________________________________
AliRsnTaskParticleYield::AliRsnTaskParticleYield(const char *name, const char *title) : AliRsnTask(name, title)
{
   //
   // Defauult constructor
   //

   // We need that current task is executed after sub-tasks
   fExecTaskBefore = kFALSE;
}

//______________________________________________________________________________
AliRsnTaskParticleYield::AliRsnTaskParticleYield(const AliRsnTaskParticleYield &copy) : AliRsnTask(copy)
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

   return (*this);
}

//______________________________________________________________________________
AliRsnTaskParticleYield::~AliRsnTaskParticleYield()
{
   //
   // Destructor
   //
}

void AliRsnTaskParticleYield::Exec(Option_t * /*option*/)
{
   // Put your task job
}
