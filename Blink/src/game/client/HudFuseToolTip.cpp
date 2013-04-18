#include "hud.h"
#include "cbase.h"
#include "HudFuseToolTip.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudFuseToolTip );

static ConVar show_fuse_tooltip("show_fuse_tooltip", "0", 0, "Toggles Interact tooltip");

CHudFuseToolTip::CHudFuseToolTip( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudFuseTooltip" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "fusetip", true, true);//starting in materials dir

   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

void CHudFuseToolTip::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, 512, 64 );
}

void CHudFuseToolTip::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudFuseToolTip::togglePrint()
{
	if (!show_fuse_tooltip.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}