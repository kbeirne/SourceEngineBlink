#include "hudelement.h"
#include <vgui_controls/Panel.h>
 
using namespace vgui;
 
class CHudFuseToolTip : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudFuseToolTip, Panel );
 
   public:
   CHudFuseToolTip( const char *pElementName );
   void togglePrint();
   virtual void OnThink();
 
   protected:
   virtual void Paint();
   int m_nImport;
};