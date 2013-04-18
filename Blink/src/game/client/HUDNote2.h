#include "hudelement.h"
#include <vgui_controls/Panel.h>

using namespace vgui;
 
class CHudNote2 : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudNote2, Panel );
 
public:
	CHudNote2( const char *pElementName );
	void togglePrint();
	virtual void OnThink();
protected:
	virtual void Paint();
	int m_nImport;
};