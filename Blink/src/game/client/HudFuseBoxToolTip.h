#include "hudelement.h"
#include <vgui_controls/Panel.h>
 
using namespace vgui;
 
class CHudFuseBoxToolTip : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudFuseBoxToolTip, Panel );
 
   public:
   CHudFuseBoxToolTip( const char *pElementName );
   void togglePrint();
   virtual void OnThink();
 
   protected:
   virtual void Paint();
   int m_nImport;
};