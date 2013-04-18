#include "hudelement.h"
#include <vgui_controls/Panel.h>

using namespace vgui;
 
class CHudNote5 : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudNote5, Panel );
 
public:
	CHudNote5( const char *pElementName );
	void togglePrint();
	virtual void OnThink();
protected:
	virtual void Paint();
	int m_nImport;
};