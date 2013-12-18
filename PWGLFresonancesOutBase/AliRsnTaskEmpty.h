//
// *** Class AliRsnTaskEmpty ***
//
//  Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIRSNTASKEMPTY_H
#define ALIRSNTASKEMPTY_H

#include <Rtypes.h>
#include "AliRsnTask.h"

class AliRsnTaskEmpty : public AliRsnTask
{
public:

   AliRsnTaskEmpty(const char *name="empty", const char *title="Empty Task");
   AliRsnTaskEmpty(const AliRsnTaskEmpty &copy);
   AliRsnTaskEmpty &operator=(const AliRsnTaskEmpty &copy);
   virtual ~AliRsnTaskEmpty();

   virtual void   Exec(Option_t *option);

private:

   ClassDef(AliRsnTaskEmpty, 1)
};

#endif
