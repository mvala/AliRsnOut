//
// *** Class AliRsnTaskParticleYield ***
//
// Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//
#include <TArrayI.h>
#include <TArrayD.h>
#include <TH1D.h>

#include "AliRsnTaskInput.h"
#include "AliRsnTaskParticleYield.h"

ClassImp(AliRsnTaskParticleYield)

//______________________________________________________________________________
AliRsnTaskParticleYield::AliRsnTaskParticleYield(const char *name, const char *title) : AliRsnTask(name, title),
   fIDProjection(0),
   fCutIDs(0),
   fCutMins(0),
   fCutMaxs(0)
{
   //
   // Defauult constructor
   //

   // We need that current task is executed after sub-tasks
   fExecTaskBefore = kFALSE;
}

//______________________________________________________________________________
AliRsnTaskParticleYield::AliRsnTaskParticleYield(const AliRsnTaskParticleYield &copy) : AliRsnTask(copy),
   fIDProjection(copy.fIDProjection),
   fCutIDs(copy.fCutIDs),
   fCutMins(copy.fCutMins),
   fCutMaxs(copy.fCutMaxs)

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
   fIDProjection = copy.fIDProjection;
   fCutIDs = copy.fCutIDs;
   fCutMins = copy.fCutMins;
   fCutMaxs = copy.fCutMaxs;

   return (*this);
}

//______________________________________________________________________________
AliRsnTaskParticleYield::~AliRsnTaskParticleYield()
{
   //
   // Destructor
   //

   delete fCutIDs;
   delete fCutMins;
   delete fCutMaxs;
}

void AliRsnTaskParticleYield::Exec(Option_t * /*option*/)
{
   // Put your task job

   Printf("Executing %s ...", GetName());

   AliRsnTaskInput *input = (AliRsnTaskInput *) GetParent();

   TH1D *h1 = input->CreateHistogram("Unlike",fIDProjection,fCutIDs,fCutMins,fCutMaxs);
   TH1D *h2 = input->CreateHistogram("LikePP",fIDProjection,fCutIDs,fCutMins,fCutMaxs);
   TH1D *h3 = input->CreateHistogram("LikeMM",fIDProjection,fCutIDs,fCutMins,fCutMaxs);
   TH1D *h4 = input->CreateHistogram("Mixing",fIDProjection,fCutIDs,fCutMins,fCutMaxs);

   h1->Print();
   h2->Print();
   h3->Print();
   h4->Print();
}


void AliRsnTaskParticleYield::SetCuts(TArrayI *ids, TArrayD *mins, TArrayD *maxs)
{
   if (fCutIDs) delete fCutIDs;
   if (fCutMins) delete fCutMins;
   if (fCutMaxs) delete fCutMaxs;

   fCutIDs = new TArrayI(*ids);
   fCutMins = new TArrayD(*mins);
   fCutMaxs = new TArrayD(*maxs);
}
