//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ========
//
// Purpose: Simple model entity that randomly moves and changes direction
//			when activated.
//
//=============================================================================
 
#include "cbase.h"
 
class CAIAngel : public CBaseAnimating
{
public:
	DECLARE_CLASS( CAIAngel, CBaseAnimating );
	DECLARE_DATADESC();
 
	CAIAngel()
	{
		m_bActive = false;
	}
 
	void Spawn( void );
	void Precache( void );
 
	void MoveThink( void );
 
	// Input function
	void InputToggle( inputdata_t &inputData );
 
private:
 
	bool	m_bActive;
	float	m_flNextChangeTime;
};
 
LINK_ENTITY_TO_CLASS( ai_angel, CAIAngel );
 
// Start of our data description for the class
BEGIN_DATADESC( CAIAngel )
 
	// Save/restore our active state
	DEFINE_FIELD( m_bActive, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flNextChangeTime, FIELD_TIME ),
 
	// Links our input name from Hammer to our input member function
	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", InputToggle ),
 
	// Declare our think function
	DEFINE_THINKFUNC( MoveThink ),
 
END_DATADESC()
 
// Name of our entity's model
#define	ENTITY_MODEL	"models/The_Sniper_9/DoctorWho/Extras/Angels/angelattack.mdl"
 
//-----------------------------------------------------------------------------
// Purpose: Precache assets used by the entity
//-----------------------------------------------------------------------------
void CAIAngel::Precache( void )
{
	PrecacheModel( ENTITY_MODEL );
 
	BaseClass::Precache();
}
 
//-----------------------------------------------------------------------------
// Purpose: Sets up the entity's initial state
//-----------------------------------------------------------------------------
void CAIAngel::Spawn( void )
{
	Precache();
 
	SetModel( ENTITY_MODEL );
	SetSolid( SOLID_BBOX );
	UTIL_SetSize( this, -Vector(20,20,20), Vector(20,20,20) );
}
 
//-----------------------------------------------------------------------------
// Purpose: Think function to randomly move the entity
//-----------------------------------------------------------------------------
void CAIAngel::MoveThink( void )
{
	// See if we should change direction again
	if ( m_flNextChangeTime < gpGlobals->curtime )
	{
	}
 
		
 
	// Think every 20Hz
	SetNextThink( gpGlobals->curtime + 0.05f );
}
 
//-----------------------------------------------------------------------------
// Purpose: Toggle the movement of the entity
//-----------------------------------------------------------------------------
void CAIAngel::InputToggle( inputdata_t &inputData )
{
	// Toggle our active state
	if ( !m_bActive )
	{
		// Start thinking
		SetThink( &CAIAngel::MoveThink );
 
		SetNextThink( gpGlobals->curtime + 0.05f );
 
		// Start moving
		SetMoveType( MOVETYPE_FLY );
 
		// Force MoveThink() to choose a new speed and direction immediately
		m_flNextChangeTime = gpGlobals->curtime;
 
		// Update m_bActive to reflect our new state
		m_bActive = true;
	}
	else
	{
		// Stop thinking
		SetThink( NULL );
 
		// Stop moving
		SetAbsVelocity( vec3_origin );
 		SetMoveType( MOVETYPE_NONE );
 
		m_bActive = false;
	}
}