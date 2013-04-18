#include "hudelement.h"
#include <vgui_controls/Panel.h>

using namespace vgui;
 
class CHudNote4 : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudNote4, Panel );
 
public:
	CHudNote4( const char *pElementName );
	void togglePrint();
	virtual void OnThink();
protected:
	virtual void Paint();
	int m_nImport;
};