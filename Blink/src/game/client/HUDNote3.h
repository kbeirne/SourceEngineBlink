#include "hudelement.h"
#include <vgui_controls/Panel.h>

using namespace vgui;
 
class CHudNote3 : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudNote3, Panel );
 
public:
	CHudNote3( const char *pElementName );
	void togglePrint();
	virtual void OnThink();
protected:
	virtual void Paint();
	int m_nImport;
};