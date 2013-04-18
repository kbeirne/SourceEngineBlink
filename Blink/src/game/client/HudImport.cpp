#include "hud.h"
#include "cbase.h"
#include "HudImport.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
 
#include "tier0/memdbgon.h"
 
using namespace vgui;
 
DECLARE_HUDELEMENT( CHudImport );

static ConVar show_fuse("show_fuse", "0", 0, "Toggles fuse icon in upper right corner");

CHudImport::CHudImport( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudImport" )
{
   Panel *pParent = g_pClientMode->GetViewport();
   SetParent( pParent );   
 
   SetVisible( false ); 
   SetAlpha( 64 );

   //AW Create Texture for Looking around
   m_nImport = surface()->CreateNewTextureID();
   surface()->DrawSetTextureFile( m_nImport,  "fuse", true, true);//starting in materials dir

   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

void CHudImport::Paint()
{
   SetPaintBorderEnabled(false);
   surface()->DrawSetTexture( m_nImport );
   surface()->DrawTexturedRect( 0, 0, 64, 64 );
}

void CHudImport::OnThink()
{
   togglePrint();
   BaseClass::OnThink();
}

void CHudImport::togglePrint()
{
	if (!show_fuse.GetBool())
      this->SetVisible(false);
   else
      this->SetVisible(true);
}