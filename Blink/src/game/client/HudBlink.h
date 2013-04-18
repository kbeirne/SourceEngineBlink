#include "hudelement.h"
#include "cbase.h"
#include <vgui_controls/Panel.h>
 
using namespace vgui;
 
class CHudBlink : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudBlink, Panel );
 
public:
	CHudBlink( const char *pElementName );
	void togglePrint();
	virtual void OnThink();
protected:
	virtual void Paint();
	int m_nImport;
};