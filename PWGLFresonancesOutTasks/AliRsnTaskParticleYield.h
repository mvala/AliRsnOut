//
// *** Class AliRsnTaskEmpty ***
//
//  Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIRSNTASKPARTICLEYIELD_H
#define ALIRSNTASKPARTICLEYIELD_H

#include <Rtypes.h>
#include "AliRsnTask.h"

class AliRsnTaskParticleYield : public AliRsnTask
{
public:

   AliRsnTaskParticleYield(const char *name="empty", const char *title="Empty Task");
   AliRsnTaskParticleYield(const AliRsnTaskParticleYield &copy);
   AliRsnTaskParticleYield &operator=(const AliRsnTaskParticleYield &copy);
   virtual ~AliRsnTaskParticleYield();

   virtual void   Exec(Option_t *option);

private:

   ClassDef(AliRsnTaskParticleYield, 1)
};

#endif
