#include "hud.h"
#include "cbase.h"
#include "edict.h"
#include "HUDNewspaper.h"
#include "HudGameOver.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudNewspaper );

static ConVar show_newspaper ("show_newspaper", "1", 0, "Show Newspaper");

CHudNewspaper::CHudNewspaper( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudNewspaper" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "notes/newspaper", true, true);//starting in materials dir

   SetSize( ScreenWidth(), ScreenHeight() );
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );

   show_newspaper.SetValue(0);
}

void CHudNewspaper::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, ScreenWidth(), ScreenHeight() );
}

void CHudNewspaper::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudNewspaper::togglePrint()
{
	if (!show_newspaper.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}