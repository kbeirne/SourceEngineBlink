#include "hudelement.h"
#include <vgui_controls/Panel.h>
 
using namespace vgui;
 
class CHudRunToolTip : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudRunToolTip, Panel );
 
   public:
   CHudRunToolTip( const char *pElementName );
   void togglePrint();
   virtual void OnThink();
 
   protected:
   virtual void Paint();
   int m_nImport;
};