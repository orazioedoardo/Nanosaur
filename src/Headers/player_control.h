//
// player_control.h
//

#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H



#define	PLAYER_MAX_WALK_SPEED	900
#define	PLAYER_WALK_ACCEL		1500
#define	PLAYER_FRICTION_ACCEL	800

#define	PLAYER_MAX_FLY_SPEED	900

#define	PLAYER_JUMP_SPEED		1200



		/* KEY CONTROL BIT FIELDS */

typedef	unsigned short KeyControlType;

enum
{
	KEYCONTROL_FORWARD		=	1,		
	KEYCONTROL_ROTRIGHT		=	(1<<1),		
	KEYCONTROL_ROTLEFT		=	(1<<2),		
	KEYCONTROL_BACKWARD		=	(1<<3),		
	KEYCONTROL_JUMP			= 	(1<<4),
	KEYCONTROL_ATTACK		=	(1<<5),	
	KEYCONTROL_ATTACKMODE	=	(1<<6),
	KEYCONTROL_PICKUP		=	(1<<7),
	KEYCONTROL_JETUP		=	(1<<8),
	KEYCONTROL_JETDOWN		=	(1<<9)
};



//==================================================

extern	void CalcPlayerKeyControls(void);
extern	void DoPlayerControl(ObjNode *theNode, float slugFactor);
extern	void DoPlayerMovement(ObjNode *theNode);
extern	void DoFrictionAndGravity(ObjNode *theNode, float friction);
extern	void StopJetPack(ObjNode *theNode);
extern	void CheckJetThrustControl(ObjNode *theNode);
extern	void DoPlayerJetControl(ObjNode *theNode);
extern	void DoPlayerJetMovement(ObjNode *theNode);



#endif
