#include "hudelement.h"
#include <vgui_controls/Panel.h>

using namespace vgui;
 
class CHudGameOver : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CHudGameOver, Panel );
 
public:
	CHudGameOver( const char *pElementName );
	void togglePrint();
	virtual void OnThink();
	static bool gameOver;
protected:
	virtual void Paint();
	int m_nImport;
};