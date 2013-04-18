//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef NPC_ANGEL_H
#define NPC_ANGEL_H
#ifdef _WIN32
#pragma once
#endif

#include "ai_basenpc.h"
#include "entityoutput.h"
#include "ai_behavior.h"
#include "ai_behavior_actbusy.h"
#include <time.h>

class CBeam;
class CSprite;
class CScriptedTarget;

typedef CAI_BehaviorHost<CAI_BaseNPC> CAI_BaseAngel;

class CNPC_Angel : public CAI_BaseAngel
{
	DECLARE_CLASS( CNPC_Angel, CAI_BaseAngel );

public:
	float			m_flNextNPCThink;
	CAI_Schedule*	WakeUp(void);//	Dormancy
	void			GoDormant(void);

public:
	void			Spawn( void );
	void			Precache( void );
	bool			CreateBehaviors();
	float			MaxYawSpeed( void );
	Class_T			Classify ( void );
	float			GetIdealSpeed();
	void			PrescheduleThink();
	
	void			StartTask( const Task_t *pTask );
	void			RunTask( const Task_t *pTask );
	virtual int		SelectSchedule ( void );
	virtual int		TranslateSchedule( int scheduleType );
	void			OnScheduleChange();
	void			AngelThink(void);
	void			NotifyDeadFriend( CBaseEntity *pFriend );

	void			AddZigZagToPath(void);

	CNPC_Angel(void);

	DECLARE_DATADESC();
	DEFINE_CUSTOM_AI;

private:
	float lastActionAt;
	CAI_ActBusyBehavior		m_ActBusyBehavior;
	double timeSinceLastBlink;//per frame
	clock_t timeOfLastBlink;
	double timeBetweenBlinks;//from start of blink to start of next blink
	double timeBeforeEyesClosed;//How long after blinking has started are our eyes full closed?
	double eyeCloseTime;//How long are the eyes closed for?
	double blinkTime;//duration of blink
	bool isBlinking;
	bool eyesClosed;
	bool blinkActionPerformed;//Have we reacted to this blink (don't want multiple thinks per blink

	bool m_bActive;
	bool hasSeenPlayer;
	bool seenByPlayer;
	bool warningGiven;
	float m_flNextChangeTime;
	float timeOfWarning;
	//float ANGEL_WARN_TIME;
};

#endif // NPC_ANGEL_H
