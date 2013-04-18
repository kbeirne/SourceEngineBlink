#include "hud.h"
#include "cbase.h"
#include "HudBlink.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudBlink );

static ConVar show_blink ("show_blink", "1", 0, "Starts a blink");

CHudBlink::CHudBlink( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudBlink" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );
   SetVisible( true );
   SetAlpha( 64 );
   //CBasePlayer *pPlayer = UTIL_PlayerByIndex(1);
   //pPlayer->blinking = true;
   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "blink/blinkanim1", true, true);//starting in materials dir
  
    SetSize( ScreenWidth(), ScreenHeight() );
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

void CHudBlink::Paint()
{
	if(IsEnabled())
	{
	   SetPaintBorderEnabled(false);
	   surface()->DrawSetTexture( m_nImport );
	   surface()->DrawTexturedRect( 0, 0, 1920, 1080 );
	}
	else 	
	{
   Reset();
		//surface()->Shutdown();
		//InvalidateLayout();
		//surface()->inv
	}
}

void CHudBlink::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudBlink::togglePrint()
{
	if (!show_blink.GetBool())
      this->SetEnabled(false);
   else
      this->SetEnabled(false);
}