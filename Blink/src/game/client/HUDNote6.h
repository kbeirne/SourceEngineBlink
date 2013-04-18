#include "hudelement.h"
#include <vgui_controls/Panel.h>

using namespace vgui;
 
class CHudNote6 : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudNote6, Panel );
 
public:
	CHudNote6( const char *pElementName );
	void togglePrint();
	virtual void OnThink();
protected:
	virtual void Paint();
	int m_nImport;
};