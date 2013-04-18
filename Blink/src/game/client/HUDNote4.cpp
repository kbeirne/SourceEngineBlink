#include "hud.h"
#include "cbase.h"
#include "edict.h"
#include "HudNote4.h"
#include "HudGameOver.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudNote4 );

static ConVar show_note4 ("show_note4", "1", 0, "Show Note4");

CHudNote4::CHudNote4( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudNote4" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "notes/note4", true, true);//starting in materials dir

   SetSize( ScreenWidth(), ScreenHeight() );
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );

   show_note4.SetValue(0);
}

void CHudNote4::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, ScreenWidth(), ScreenHeight() );
}

void CHudNote4::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudNote4::togglePrint()
{
	if (!show_note4.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}