#include "hud.h"
#include "cbase.h"
#include "edict.h"
#include "HudGameOver.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudGameOver );

//extern IVEngineServer *engine;
//extern CGlobalVars		*gpGlobals;
bool CHudGameOver::gameOver = false; //you must do this in .cpp file

CHudGameOver::CHudGameOver( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudGameOver" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );
   SetVisible( true );
   SetAlpha( 64 );
   //CBasePlayer *pPlayer = UTIL_PlayerByIndex(1);
   //pPlayer->blinking = true;
   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "gameover", true, true);//starting in materials dir
  
   SetSize( ScreenWidth(), ScreenHeight() );
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
   this->SetEnabled(true);
   this->SetVisible(false);
   //pPlayer = NULL;
}

void CHudGameOver::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, 1920, 1080 );
}

void CHudGameOver::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudGameOver::togglePrint()
{
	if (gameOver)
      SetVisible(true);
   else
      SetVisible(false);
}