#include "hudelement.h"
#include <vgui_controls/Panel.h>

using namespace vgui;
 
class CHudDeath : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudDeath, Panel );
 
public:
	CHudDeath( const char *pElementName );
	void togglePrint();
	virtual void OnThink();
protected:
	virtual void Paint();
	float startTime;
	bool started;
	int m_nImport;
	
	CBasePlayer * pPlayer;
};