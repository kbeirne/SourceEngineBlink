#include "hud.h"
#include "cbase.h"
#include "HudCrouchToolTip.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudCrouchToolTip );

static ConVar show_crouch_tooltip("show_crouch_tooltip", "0", 0, "Toggles Interact tooltip");

CHudCrouchToolTip::CHudCrouchToolTip( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudCrouchTooltip" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "crouchtip", true, true);//starting in materials dir

   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

void CHudCrouchToolTip::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, 256, 64 );
}

void CHudCrouchToolTip::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudCrouchToolTip::togglePrint()
{
	if (!show_crouch_tooltip.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}