#include "hud.h"
#include "cbase.h"
#include "edict.h"
#include "HudDeath.h"
#include "HudGameOver.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudDeath );

static ConVar show_death ("show_death", "1", 0, "Show death animation");
//extern IVEngineServer *engine;
//extern CGlobalVars		*gpGlobals;

CHudDeath::CHudDeath( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudDeath" ), started(false), startTime(0)
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );
   SetVisible( true );
   SetAlpha( 64 );
   //CBasePlayer *pPlayer = UTIL_PlayerByIndex(1);
   //pPlayer->blinking = true;
   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "death/death", true, true);//starting in materials dir
  
   SetSize( ScreenWidth(), ScreenHeight() );
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
   this->SetEnabled(false);
   show_death.SetValue(1);	
   pPlayer = NULL;
}

void CHudDeath::Paint()
{
	if(IsEnabled())
	{
	   SetPaintBorderEnabled(false);
	   surface()->DrawSetTexture( m_nImport );
	   surface()->DrawTexturedRect( 0, 0, 1920, 1080 );
	}
	else 	
	{
   Reset();
		//surface()->Shutdown();
		//InvalidateLayout();
		//surface()->inv
	}
}

void CHudDeath::OnThink()
{
	pPlayer = UTIL_PlayerByIndex(1);
	if(started)
	{
		if((gpGlobals->curtime - startTime) > 1.45) 
		{
			this->SetEnabled(false);
			started = false;
			pPlayer->BlinkSetHealth(2);
			CHudGameOver::gameOver = true;
		}
	}
	else if (!started && pPlayer->GetHealth() == 1)
	{
		startTime = gpGlobals->curtime;
		started = true;
		this->SetEnabled(true);
		
		//Play a sound
		CPASAttenuationFilter filter( pPlayer );
		//pPlayer->EmitSound( filter, pPlayer->GetSoundSourceIndex(), "Blink.Death" );
	}
   togglePrint();
   BaseClass::OnThink();
}

void CHudDeath::togglePrint()
{
	if (started)
      this->SetEnabled(true);
   else
      this->SetEnabled(false);
}