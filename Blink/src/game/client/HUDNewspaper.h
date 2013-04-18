#include "hudelement.h"
#include <vgui_controls/Panel.h>

using namespace vgui;
 
class CHudNewspaper : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudNewspaper, Panel );
 
public:
	CHudNewspaper( const char *pElementName );
	void togglePrint();
	virtual void OnThink();
protected:
	virtual void Paint();
	int m_nImport;
};