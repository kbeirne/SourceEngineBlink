#include "hudelement.h"
#include <vgui_controls/Panel.h>
 
using namespace vgui;
 
class CHudWalkToolTip : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudWalkToolTip, Panel );
 
   public:
   CHudWalkToolTip( const char *pElementName );
   void togglePrint();
   virtual void OnThink();
 
   protected:
   virtual void Paint();
   int m_nImport;
};