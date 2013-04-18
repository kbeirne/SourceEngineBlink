#include "hudelement.h"
#include <vgui_controls/Panel.h>
 
using namespace vgui;
 
class CHudCrouchToolTip : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudCrouchToolTip, Panel );
 
public:
	CHudCrouchToolTip( const char *pElementName );
	void togglePrint();
	virtual void OnThink();
protected:
	virtual void Paint();
	int m_nImport;
};