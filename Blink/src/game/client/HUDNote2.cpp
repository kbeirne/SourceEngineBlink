#include "hud.h"
#include "cbase.h"
#include "edict.h"
#include "HudNote2.h"
#include "HudGameOver.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudNote2 );

static ConVar show_note2 ("show_note2", "1", 0, "Show Note2");

CHudNote2::CHudNote2( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudNote2" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "notes/note2", true, true);//starting in materials dir

   SetSize( ScreenWidth(), ScreenHeight() );
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );

   show_note2.SetValue(0);
}

void CHudNote2::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, ScreenWidth(), ScreenHeight() );
}

void CHudNote2::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudNote2::togglePrint()
{
	if (!show_note2.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}