#include "hud.h"
#include "cbase.h"
#include "edict.h"
#include "HudNote6.h"
#include "HudGameOver.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudNote6 );

static ConVar show_note6 ("show_note6", "1", 0, "Show Note6");

CHudNote6::CHudNote6( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudNote6" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "notes/note6", true, true);//starting in materials dir

   SetSize( ScreenWidth(), ScreenHeight() );
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );

   show_note6.SetValue(0);
}

void CHudNote6::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, ScreenWidth(), ScreenHeight() );
}

void CHudNote6::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudNote6::togglePrint()
{
	if (!show_note6.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}