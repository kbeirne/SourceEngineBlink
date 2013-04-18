#include "hudelement.h"
#include "AchievementHandler.h"
#include <vgui_controls/Panel.h>

using namespace vgui;
 
class CHudNote1 : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudNote1, Panel );
 
public:
	CHudNote1( const char *pElementName );
	void togglePrint();
	virtual void OnThink();
protected:
	virtual void Paint();
	int m_nImport;
};