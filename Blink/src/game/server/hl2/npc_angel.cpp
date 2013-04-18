//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "soundent.h"
#include "game.h"
#include "beam_shared.h"
#include "Sprite.h"
#include "npcevent.h"
#include "npc_angel.h"
#include "ai_hull.h"
#include "ai_default.h"
#include "ai_node.h"
#include "ai_network.h"
#include "ai_hint.h"
#include "ai_link.h"
#include "ai_waypoint.h"
#include "ai_navigator.h"
#include "ai_senses.h"
#include "ai_squadslot.h"
#include "ai_squad.h"
#include "ai_memory.h"
#include "ai_tacticalservices.h"
#include "ai_moveprobe.h"
#include "npc_talker.h"
#include "activitylist.h"
#include "bitstring.h"
#include "decals.h"
#include "player.h"
#include "entitylist.h"
#include "ndebugoverlay.h"
#include "ai_interactions.h"
#include "animation.h"
#include "scriptedtarget.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "world.h"
#include "triggers.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define	MAX_ANGEL_VIEW_DISTANCE		3600 // 3600 feet.
#define	ANGEL_STALK_DISTANCE		1500 // Will give up chase if angel can't see you and is beyond this distance
#define	ANGEL_ATTACK_DISTANCE		700 // 3600 feet.
#define	ANGEL_KILL_DISTANCE			40 // 3600 feet.
#define ANGEL_WARN_TIME				5
extern void		SpawnBlood(Vector vecSpot, const Vector &vAttackDir, int bloodColor, float flDamage);

LINK_ENTITY_TO_CLASS( npc_angel, CNPC_Angel );


//=========================================================
// Private activities.
//=========================================================
static int ACT_ANGEL_WORK = 0;

//=========================================================
// Custom Conditions
//=========================================================
enum 
{
	COND_ANGEL_SEEN_BY_PLAYER = LAST_SHARED_CONDITION,//Angel can't move because in FOV of player, different to being known to player (see below)
	COND_ANGEL_KNOWS_PLAYER,//Has angel seen player at some point? Then is he's in range he can keep following him
	COND_ANGEL_KNOWN_TO_PLAYER,//Has the player seen the angel move, or has he gotten within a certain distance of it
	COND_ANGEL_PLAYER_BLINKING,
	COND_ANGEL_IN_ATTACK_RANGE,
	COND_ANGEL_IN_KILL_RANGE,//Player is dead if they blink or turn away from angel
	COND_ANGEL_SEES_ANGEL,
	COND_ANGEL_NEAR_LIGHT,
	COND_ANGEL_WARNING_PLAYER
};

//=========================================================
// Stalker schedules
//=========================================================
enum
{
	SCHED_ANGEL_IDLE = LAST_SHARED_SCHEDULE,
	SCHED_ANGEL_STALK_PLAYER,
	SCHED_ANGEL_ATTACK_PLAYER,
	SCHED_ANGEL_ACQUIRE_PLAYER, 
	SCHED_ANGEL_PATROL,
};

//=========================================================
// Stalker Tasks
//=========================================================
enum 
{
	TASK_ANGEL_SCREAM = LAST_SHARED_TASK
};

//-----------------------------------------------------------------------------

BEGIN_DATADESC( CNPC_Angel )

	DEFINE_FIELD( m_flNextNPCThink,	FIELD_FLOAT),
	DEFINE_THINKFUNC( AngelThink )

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
float CNPC_Angel::MaxYawSpeed( void ) {	return 1000.0f; }//hopefully instantaneous kevin b


//-----------------------------------------------------------------------------
// Purpose: 
//
//
// Output : int
//-----------------------------------------------------------------------------
Class_T CNPC_Angel::Classify( void )
{
	return CLASS_ANGEL;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_Angel::PrescheduleThink()
{
	double currentTime = clock();
	//timeSinceLastBlink += gpGlobals->frametime;
	timeSinceLastBlink = ((currentTime - timeOfLastBlink) / 1000.0);

	if(timeSinceLastBlink > timeBetweenBlinks)//this code is bullshit
	{
		timeOfLastBlink = timeOfLastBlink + (timeBetweenBlinks * 1000.0);
		//timeOfLastBlink = ((float)((int)(timeOfLastBlink * 1000) % (int)(timeBetweenBlinks * 1000))) / 1000.0f;
		//eyesClosed = false;//shouldn't be needed, just being safe
		isBlinking = true;
	}
	else if(timeSinceLastBlink > blinkTime)
	{
		isBlinking = false;
		//eyesClosed = false;//shouldn't be needed, just being safe
	}
	else if(timeSinceLastBlink > timeBeforeEyesClosed && timeSinceLastBlink < (timeBeforeEyesClosed + eyeCloseTime))
	{
		eyesClosed = true;
		//Msg("Eyes closed!");
		
	}
	else
	{
		eyesClosed = false;
	}

	Vector vecForward, vecPlayerPos, vecDelta;
	CBasePlayer* pPlayer = UTIL_PlayerByIndex(1);
	pPlayer->EyeVectors(&vecForward);
	vecPlayerPos = pPlayer->EyePosition();
	vecDelta = WorldSpaceCenter() - vecPlayerPos;
	vecDelta.NormalizeInPlace();
	pPlayer->GetAbsAngles();
	
	vec_t res = DotProduct(vecForward, vecDelta);
	double playerFOV = cos( (float)pPlayer->GetFOV());
	if((res * 2) >= playerFOV)
	{
		SetCondition(COND_ANGEL_SEEN_BY_PLAYER);
		seenByPlayer = true;
		if(HasCondition(COND_ANGEL_WARNING_PLAYER) && (gpGlobals->curtime - timeOfWarning) < ANGEL_WARN_TIME)
		{
			timeOfWarning -= ANGEL_WARN_TIME;//Make it so player is already used his warn time
		}
	}
	else
	{
		//SetCondition(COND_ANGEL_PLAYER_BLINKING);
		seenByPlayer = false;
		ClearCondition(COND_ANGEL_SEEN_BY_PLAYER);
	}
	if(HasCondition(COND_SEE_PLAYER) || hasSeenPlayer)
	{
		SetCondition(COND_ANGEL_KNOWS_PLAYER);
		hasSeenPlayer = true;//Technically I shouldn't have to set this bool, it should remember that COND_ANGEL_KNOWS_PLAYER, but it doesn't seem to for some reason
	}
	else if((pPlayer->GetAbsOrigin() - GetAbsOrigin()).Length() > ANGEL_STALK_DISTANCE)
	{
		hasSeenPlayer = false;
		ClearCondition(COND_ANGEL_KNOWS_PLAYER);
	}

	if(HasCondition(COND_ANGEL_SEEN_BY_PLAYER) && (pPlayer->GetAbsOrigin() - GetAbsOrigin()).Length() < ANGEL_ATTACK_DISTANCE) 
	{
		//DevMsg("ATK time!");
		SetCondition(COND_ANGEL_IN_ATTACK_RANGE);
	}
	else ClearCondition(COND_ANGEL_IN_ATTACK_RANGE);

	if(!seenByPlayer && (pPlayer->GetAbsOrigin() - GetAbsOrigin()).Length() < ANGEL_KILL_DISTANCE) 
	{
		if(!warningGiven)
		{
			warningGiven = true;
			SetCondition(COND_ANGEL_WARNING_PLAYER);
			EmitSound( "Angel.Approaching" );
			timeOfWarning = gpGlobals->curtime;
		}
		else if((gpGlobals->curtime - timeOfWarning) > ANGEL_WARN_TIME)
		{
			pPlayer->caughtByAngel = true;
			pPlayer->SetHealth(1);
		}
	}
	
	// Randomly take a new direction and speed
	/*Vector vecNewVelocity = RandomVector( -64.0f, 64.0f );
	Vector vecVel = pPlayer->GetAbsOrigin() - GetAbsOrigin();
	if(!HasCondition(COND_ANGEL_SEEN_BY_PLAYER) && HasCondition(COND_SEE_PLAYER) && HasCondition(COND_ANGEL_PLAYER_BLINKING) && pPlayer->GetHealth() > 0)
	{
		QAngle facing = pPlayer->GetAbsAngles();

		QAngle rotate(DEG2RAD(180), 0, 0);
		QAngle newAngle;
		Vector facingVec;
		Vector newFaceVec;
		AngleVectors(facing, &facingVec);
		AngleVectors(rotate, &newFaceVec);
		VectorAngles(newFaceVec * facingVec, newAngle);
		
		
		facing.x = -facing.x;
		facing.z = -facing.z;
		SetAbsAngles(newAngle);...*/

		/*if(gpGlobals->curtime - lastActionAt > 0.7f)
		{
			vecVel.NormalizeInPlace();
			vecVel *= 64;
			vecVel.z = 0;
			SetAbsVelocity( vecVel );
			SetAbsOrigin(GetAbsOrigin() + vecVel);

			if((pPlayer->GetAbsOrigin() - GetAbsOrigin()).Length() < 64)
			{
				pPlayer->SetHealth(0);
			}
			lastActionAt = gpGlobals->curtime;
		}
	}*/
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CNPC_Angel::Spawn( void )
{
	Precache( );
//	ANGEL_WARN_TIME = 8.5f;
	//SetModel("models/stalker.mdl");
	SetModel("models/The_Sniper_9/DoctorWho/Extras/Angels/angelpoint.mdl");
	SetHullType(HULL_HUMAN);
	SetHullSizeNormal();

	SetSolid( SOLID_BBOX );
	AddSolidFlags( FSOLID_NOT_STANDABLE );
	SetMoveType( MOVETYPE_STEP );
	m_bloodColor		= DONT_BLEED;
	m_iHealth			= 100;
	m_flFieldOfView		= VIEW_FIELD_FULL;// indicates the width of this NPC's forward view cone ( as a dotproduct result )
	m_NPCState			= NPC_STATE_IDLE;
	CapabilitiesAdd( bits_CAP_SQUAD | bits_CAP_MOVE_GROUND );
	//CapabilitiesAdd( bits_CAP_INNATE_RANGE_ATTACK1);
	m_flDistTooFar	= MAX_ANGEL_VIEW_DISTANCE;


	GetSenses()->SetDistLook(MAX_ANGEL_VIEW_DISTANCE - 1);
	AddClassRelationship( CLASS_PLAYER, D_HT, 0 );//should default to this already

	//Set up blink info
	timeOfLastBlink = clock();
	timeBetweenBlinks = 7.05f;
	timeBeforeEyesClosed = 0.5f;
	eyeCloseTime = 0.05f;//works as long as we're above 20 fps - doesn't really work at all though...
	blinkTime = 0.9f;	

	lastActionAt = 0;

	NPCInit();
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CNPC_Angel::Precache( void )
{
	//PrecacheModel("models/stalker.mdl");
	PrecacheModel("models/The_Sniper_9/DoctorWho/Extras/Angels/angelidle.mdl");
	PrecacheModel("models/The_Sniper_9/DoctorWho/Extras/Angels/angelpoint.mdl");
	PrecacheModel("models/The_Sniper_9/DoctorWho/Extras/Angels/angelattack.mdl");

	PrecacheModel("sprites/redglow1.vmt");
	PrecacheModel("sprites/orangeglow1.vmt");
	PrecacheModel("sprites/yellowglow1.vmt");

	PrecacheScriptSound( "Angel.Approaching" );

	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :  - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CNPC_Angel::CreateBehaviors()
{
	AddBehavior( &m_ActBusyBehavior );

	return BaseClass::CreateBehaviors();
}

void CNPC_Angel::OnScheduleChange()
{
	BaseClass::OnScheduleChange();
}

//=========================================================
// start task
//=========================================================
void CNPC_Angel::StartTask( const Task_t *pTask )
{
	//if(pTask->iTask == TASK_WAIT) DevMsg("Waiting for 5 seconds...");
	//else if(pTask->iTask == TASK_WALK_PATH_TIMED) DevMsg("Walking pathf");
	switch ( pTask->iTask )
	{
	case TASK_GET_PATH_TO_ENEMY_LOS:
		{
			if ( GetEnemy() != NULL )
			{
				BaseClass:: StartTask( pTask );
				return;
			}
		}
			/*Vector posLos;
			if (GetTacticalServices()->FindLos(m_vLaserCurPos, m_vLaserCurPos, MIN_STALKER_FIRE_RANGE, MAX_STALKER_FIRE_RANGE, 1.0, &posLos))
			{ AI_NavGoal_t goal( posLos, ACT_RUN, AIN_HULL_TOLERANCE );
				GetNavigator()->SetGoal( goal ); }
			else TaskFail(FAIL_NO_SHOOT);
			break;
		}*/
	case TASK_FACE_ENEMY:
		{
			if ( GetEnemy() != NULL )
			{
				BaseClass:: StartTask( pTask );
				return;
			}
			//GetMotor()->SetIdealYawToTarget( m_vLaserCurPos );
			break;
		}
	default: 
		BaseClass:: StartTask( pTask );
		break;
		
	}
}

//=========================================================
// RunTask
//=========================================================
void CNPC_Angel::RunTask( const Task_t *pTask )
{
	switch ( pTask->iTask )
	{
	case TASK_ANNOUNCE_ATTACK:
	{
		// Stop waiting if enemy facing me or lost enemy
		CBaseCombatCharacter* pBCC = GetEnemyCombatCharacterPointer();
		if	(!pBCC || pBCC->FInViewCone( this ))
		{
			TaskComplete();
		}

		if ( IsWaitFinished() )
		{
			TaskComplete();
		}
		break;
	}
	case TASK_FACE_ENEMY:
		{
			if ( GetEnemy() != NULL )
			{
				BaseClass:: RunTask( pTask );
				return;
			}
			//GetMotor()->SetIdealYawToTargetAndUpdate( m_vLaserCurPos );

			if ( FacingIdeal() )
			{
				TaskComplete();
			}
			break;
		}
	case TASK_WALK_PATH:
			BaseClass::RunTask( pTask );
			break;
	default:
		{
			BaseClass::RunTask( pTask );
			break;
		}
	}
}

float CNPC_Angel::GetIdealSpeed() { return 10.0f; }

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
static int modelTest = 0;
int CNPC_Angel::SelectSchedule( void )
{
	//I've not seen the player at all yet so don't move
	if(!HasCondition(COND_ANGEL_KNOWS_PLAYER))
	{
		return 	SCHED_ANGEL_IDLE;
	}
	else
	{
		//Don't move if the player can see me
		if(HasCondition(COND_ANGEL_SEEN_BY_PLAYER))
		{
			return SCHED_ANGEL_IDLE;
		}
		else
		{	
			if(HasCondition(COND_ANGEL_IN_ATTACK_RANGE)) return SCHED_ANGEL_ATTACK_PLAYER;
			
			return SCHED_ANGEL_STALK_PLAYER;
		}
	}
	return BaseClass::SelectSchedule();
}


//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
int CNPC_Angel::TranslateSchedule( int scheduleType ) 
{
	switch( scheduleType )
	{
	case SCHED_FAIL_ESTABLISH_LINE_OF_FIRE:
		{	
			return SCHED_COMBAT_STAND;
			break;
		}
	case SCHED_FAIL_TAKE_COVER:
		{
			return SCHED_RUN_RANDOM;
			break;
		}
	}

	return BaseClass::TranslateSchedule( scheduleType );
}

//------------------------------------------------------------------------------
// Purpose : Update beam more often then regular NPC think so it doesn't
//			 move so jumpily over the ground
// Input   :
// Output  :
//------------------------------------------------------------------------------
void CNPC_Angel::AngelThink(void)
{
	//DrawAttackBeam();
	if (gpGlobals->curtime >= m_flNextNPCThink)
	{
		NPCThink();
		m_flNextNPCThink = GetNextThink();
	}

	if ( m_flNextChangeTime < gpGlobals->curtime )
	{
 
		// Randomly change it again within one to three seconds
		m_flNextChangeTime = gpGlobals->curtime + random->RandomFloat( 1.0f, 3.0f );
	}

	Vector velFacing = GetAbsVelocity();
	QAngle angFacing;
	VectorAngles( velFacing, angFacing );
 	SetAbsAngles( angFacing );
 
	// Think at 20Hz
	SetNextThink( gpGlobals->curtime + 0.05f );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void CNPC_Angel::NotifyDeadFriend( CBaseEntity *pFriend )
{
	BaseClass::NotifyDeadFriend(pFriend);
}

#define ZIG_ZAG_SIZE 3600

void CNPC_Angel::AddZigZagToPath(void) 
{
	// If already on a detour don't add a zigzag
	if (GetNavigator()->GetCurWaypointFlags() & bits_WP_TO_DETOUR)
	{
		return;
	}

	// If enemy isn't facing me or occluded, don't add a zigzag
	if (HasCondition(COND_ENEMY_OCCLUDED) || !HasCondition ( COND_ENEMY_FACING_ME ))
	{
		return;
	}

	Vector waypointPos = GetNavigator()->GetCurWaypointPos();
	Vector waypointDir = (waypointPos - GetAbsOrigin());

	// If the distance to the next node is greater than ZIG_ZAG_SIZE
	// then add a random zig/zag to the path
	if (waypointDir.LengthSqr() > ZIG_ZAG_SIZE)
	{
		// Pick a random distance for the zigzag (less that sqrt(ZIG_ZAG_SIZE)
		float distance = random->RandomFloat( 30, 60 );

		// Get me a vector orthogonal to the direction of motion
		VectorNormalize( waypointDir );
		Vector vDirUp(0,0,1);
		Vector vDir;
		CrossProduct( waypointDir, vDirUp, vDir);

		// Pick a random direction (left/right) for the zigzag
		if (random->RandomInt(0,1))
		{
			vDir = -1 * vDir;
		}

		// Get zigzag position in direction of target waypoint
		Vector zigZagPos = GetAbsOrigin() + waypointDir * 60;

		// Now offset 
		zigZagPos = zigZagPos + (vDir * distance);

		// Now make sure that we can still get to the zigzag position and the waypoint
		AIMoveTrace_t moveTrace1, moveTrace2;
		GetMoveProbe()->MoveLimit( NAV_GROUND, GetAbsOrigin(), zigZagPos, MASK_NPCSOLID, NULL, &moveTrace1);
		GetMoveProbe()->MoveLimit( NAV_GROUND, zigZagPos, waypointPos, MASK_NPCSOLID, NULL, &moveTrace2);
		if ( !IsMoveBlocked( moveTrace1 ) && !IsMoveBlocked( moveTrace2 ) )
		{
			GetNavigator()->PrependWaypoint( zigZagPos, NAV_GROUND, bits_WP_TO_DETOUR );
		}
	}
}

//------------------------------------------------------------------------------
// Purpose :
// Input   :
// Output  :
//------------------------------------------------------------------------------
CNPC_Angel::CNPC_Angel(void)
{
}


//------------------------------------------------------------------------------
//
// Schedules
//
//------------------------------------------------------------------------------

AI_BEGIN_CUSTOM_NPC( npc_angel, CNPC_Angel )

	DECLARE_CONDITION(COND_ANGEL_SEEN_BY_PLAYER)
	DECLARE_CONDITION(COND_ANGEL_KNOWS_PLAYER)
	DECLARE_CONDITION(COND_ANGEL_KNOWN_TO_PLAYER)
	DECLARE_CONDITION(COND_ANGEL_PLAYER_BLINKING)
	DECLARE_CONDITION(COND_ANGEL_IN_ATTACK_RANGE)
	DECLARE_CONDITION(COND_ANGEL_IN_KILL_RANGE)
	DECLARE_CONDITION(COND_ANGEL_SEES_ANGEL)
	DECLARE_CONDITION(COND_ANGEL_NEAR_LIGHT)
	DECLARE_CONDITION(COND_ANGEL_WARNING_PLAYER)

	DECLARE_TASK(TASK_ANGEL_SCREAM)

	DECLARE_ACTIVITY(ACT_ANGEL_WORK)

	DEFINE_SCHEDULE
	(
		SCHED_ANGEL_IDLE,

		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_WAIT_INDEFINITE			0"
		""
		"	Interrupts"
		"		COND_ANGEL_KNOWS_PLAYER"
	)

	//=========================================================
	// > SCHED_ANGEL_CHASE_PLAYER
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_ANGEL_STALK_PLAYER,

		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_ANGEL_IDLE"
		"		TASK_SET_TOLERANCE_DISTANCE		24"
		"		TASK_GET_PATH_TO_ENEMY			0"
		"		TASK_FACE_ENEMY					0"
		"		TASK_WALK_PATH_TIMED			0.05"
		"		TASK_WAIT_FOR_MOVEMENT_STEP		0"
		"		TASK_STOP_MOVING				0"
		"		TASK_WAIT						8.0"
		"		TASK_SET_SCHEDULE				SCHEDULE:SCHED_ANGEL_STALK_PLAYER"
		""
		"	Interrupts"
		"		COND_ANGEL_SEEN_BY_PLAYER"
		"		COND_ENEMY_DEAD"
		"		COND_TASK_FAILED"
	)

	DEFINE_SCHEDULE
	(
		SCHED_ANGEL_ATTACK_PLAYER,

		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_ANGEL_IDLE"
		"		TASK_SET_TOLERANCE_DISTANCE		24"
		"		TASK_GET_PATH_TO_ENEMY			0"
		"		TASK_FACE_ENEMY					0"
		"		TASK_WALK_PATH_TIMED			0.05"
		"		TASK_WAIT_FOR_MOVEMENT_STEP		0"
		"		TASK_STOP_MOVING				0"
		"		TASK_WAIT						5.0"
		"		TASK_SET_SCHEDULE				SCHEDULE:SCHED_ANGEL_STALK_PLAYER"
		""
		"	Interrupts"
		"		COND_ANGEL_SEEN_BY_PLAYER"
		"		COND_ENEMY_DEAD"
		"		COND_TASK_FAILED"
	)

	DEFINE_SCHEDULE
	(
	SCHED_ANGEL_ACQUIRE_PLAYER,

	"	Tasks"
	"		TASK_STOP_MOVING				0"
	"		TASK_FACE_ENEMY					0"
	"		TASK_WAIT_RANDOM				0.5"
	"		TASK_ANGEL_SCREAM				0"
	"		TASK_SET_SCHEDULE				SCHEDULE:SCHED_ANGEL_STALK_PLAYER"
	""
	"	Interrupts"
	)

	DEFINE_SCHEDULE	
	(
	SCHED_ANGEL_PATROL,

	"	Tasks"
	"		TASK_STOP_MOVING				0"
	//"		TASK_WAIT						0.5"// This makes them look a bit more vigilant, instead of INSTANTLY patrolling after some other action.
	//"		TASK_WAIT_RANDOM				0.5"
	"		TASK_WANDER						18000600" 
	"		TASK_FACE_PATH					0"
	"		TASK_WALK_PATH					0"
	"		TASK_WAIT_FOR_MOVEMENT			0"
	"		TASK_STOP_MOVING				0"
	"		TASK_FACE_REASONABLE			0"
	"		TASK_SET_SCHEDULE				SCHEDULE:SCHED_ANGEL_PATROL"
	""
	"	Interrupts"
	//"		COND_NEW_ENEMY"
	//"		COND_CAN_RANGE_ATTACK1"
	//"		COND_SEE_ENEMY"
	)

AI_END_CUSTOM_NPC()
