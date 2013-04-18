#include "hud.h"
#include "cbase.h"
#include "edict.h"
#include "HudNote1.h"
#include "HudGameOver.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudNote1 );

static ConVar show_note1 ("show_note1", "1", 0, "Show Note1");

CHudNote1::CHudNote1( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudNote1" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false );
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "notes/note1", true, true);//starting in materials dir

   SetSize( ScreenWidth(), ScreenHeight() );
   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
   
   if(!AchievementHandler::hasAchievement("Nevik", "Daire"))
	AchievementHandler::addAchievement("Nevik", "Daire");
   if(AchievementHandler::hasAchievement("Nevik", "Daire"))
	   SetVisible(true);
   else
	   SetVisible(false);
   show_note1.SetValue(0);
}

void CHudNote1::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, ScreenWidth(), ScreenHeight() );
}

void CHudNote1::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudNote1::togglePrint()
{
	if (!show_note1.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}