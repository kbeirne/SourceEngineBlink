#include "hud.h"
#include "cbase.h"
#include "HudInteractToolTip.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudInteractToolTip );

static ConVar show_interact_tooltip("show_interact_tooltip", "0", 0, "Toggles Interact tooltip");

CHudInteractToolTip::CHudInteractToolTip( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudInteractTooltip" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "interacttip", true, true);//starting in materials dir

   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

void CHudInteractToolTip::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, 512, 64 );
}

void CHudInteractToolTip::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudInteractToolTip::togglePrint()
{
	if (!show_interact_tooltip.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}