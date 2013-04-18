#include "hud.h"
#include "cbase.h"
#include "HudFuseBoxToolTip.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudFuseBoxToolTip );

static ConVar show_fusebox_tooltip("show_fusebox_tooltip", "0", 0, "Toggles Interact tooltip");

CHudFuseBoxToolTip::CHudFuseBoxToolTip( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudFuseBoxTooltip" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "fuseboxtip", true, true);//starting in materials dir

   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

void CHudFuseBoxToolTip::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, 512, 64 );
}

void CHudFuseBoxToolTip::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudFuseBoxToolTip::togglePrint()
{
	if (!show_fusebox_tooltip.GetBool())
      SetVisible(false);
   else
      SetVisible(true);
}