#include "hud.h"
#include "cbase.h"
#include "edict.h"
#include "HudNote3.h"
#include "HudGameOver.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudNote3 );

static ConVar show_note3 ("show_note3", "1", 0, "Show Note3 animation");

CHudNote3::CHudNote3( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudNote3" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "notes/note3", true, true);//starting in materials dir

   SetSize( ScreenWidth(), ScreenHeight() );
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
   
   show_note3.SetValue(0);
}

void CHudNote3::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, ScreenWidth(), ScreenHeight() );
}

void CHudNote3::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudNote3::togglePrint()
{
	if (!show_note3.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}