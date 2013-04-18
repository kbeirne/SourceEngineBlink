#include "hud.h"
#include "cbase.h"
#include "HudRunToolTip.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudRunToolTip );

static ConVar show_run_tooltip("show_run_tooltip", "0", 0, "Toggles Interact tooltip");

CHudRunToolTip::CHudRunToolTip( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudRunTooltip" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "runtip", true, true);//starting in materials dir

   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

void CHudRunToolTip::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, 256, 64 );
}

void CHudRunToolTip::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudRunToolTip::togglePrint()
{
	if (!show_run_tooltip.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}