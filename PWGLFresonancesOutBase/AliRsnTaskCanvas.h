//
// *** Class AliRsnTaskCanvas ***
//
//  Empty AliRsnTask
//
// authors: Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIRSNTASKCANVAS_H
#define ALIRSNTASKCANVAS_H

#include <Rtypes.h>
#include "AliRsnTask.h"

class AliRsnTaskCanvas : public AliRsnTask
{
public:

   AliRsnTaskCanvas(const char *name="empty", const char *title="Empty Task");
   AliRsnTaskCanvas(const AliRsnTaskCanvas &copy);
   AliRsnTaskCanvas &operator=(const AliRsnTaskCanvas &copy);
   virtual ~AliRsnTaskCanvas();

   virtual void   Exec(Option_t *option);

   void GetOptimalDivide(Int_t num, Int_t &x, Int_t &y);

private:

   ClassDef(AliRsnTaskCanvas, 1)
};

#endif
