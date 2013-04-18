#include "hud.h"
#include "cbase.h"
#include "HudWalkToolTip.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudWalkToolTip );

static ConVar show_walk_tooltip("show_walk_tooltip", "0", 0, "Toggles walk tooltip");

CHudWalkToolTip::CHudWalkToolTip( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudWalkTooltip" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "walktip", true, true);//starting in materials dir

   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

void CHudWalkToolTip::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, 512, 64 );
}

void CHudWalkToolTip::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudWalkToolTip::togglePrint()
{
	if (!show_walk_tooltip.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}