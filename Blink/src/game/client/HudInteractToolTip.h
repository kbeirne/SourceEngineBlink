#include "hudelement.h"
#include <vgui_controls/Panel.h>
 
using namespace vgui;
 
class CHudInteractToolTip : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudInteractToolTip, Panel );
 
   public:
   CHudInteractToolTip( const char *pElementName );
   void togglePrint();
   virtual void OnThink();
 
   protected:
   virtual void Paint();
   int m_nImport;
};