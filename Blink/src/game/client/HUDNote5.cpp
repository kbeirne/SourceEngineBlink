#include "hud.h"
#include "cbase.h"
#include "edict.h"
#include "HudNote5.h"
#include "HudGameOver.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudNote5 );

static ConVar show_note5 ("show_note5", "1", 0, "Show Note5");

CHudNote5::CHudNote5( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudNote5" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "notes/note5", true, true);//starting in materials dir

   SetSize( ScreenWidth(), ScreenHeight() );
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );

   show_note5.SetValue(0);
}

void CHudNote5::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, ScreenWidth(), ScreenHeight() );
}

void CHudNote5::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudNote5::togglePrint()
{
	if (!show_note5.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}