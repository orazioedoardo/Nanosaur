/****************************/
/*   	MAINMENU.C		    */
/* (c)1998 Pangea Software  */
/* By Brian Greenstone      */
/****************************/

/****************************/
/*    EXTERNALS             */
/****************************/

#include "game.h"


/****************************/
/*    PROTOTYPES             */
/****************************/

static void MakeMainMenuModels(void);
static void SpinMainMenuIcons(float delta);
static void SpinToPreviousMainMenuIcon(void);
static void SpinToNextMainMenuIcon(void);
static void MoveFallingEgg(ObjNode *theNode);
static void GenerateFallingEgg(void);
static void MoveMenuBG(ObjNode *theNode);

/****************************/
/*    CONSTANTS             */
/****************************/

#define WHEEL_SEPARATION	310
#define SPIN_SPEED			2.5
#define	MENU_RING_Y			0

#define	NUM_MAINMENU_ICONS	5

/****************************/
/*    VARIABLES             */
/****************************/

static float gEggTimer = 0;



/******************* MENU INTERFACE ITEMS *************************/

enum
{
	MENU_ObjType_Quit,
	MENU_ObjType_Options,
	MENU_ObjType_Info,
	MENU_ObjType_HighScores,
	MENU_ObjType_Egg,
	MENU_ObjType_Background
};


/*********************/
/*    VARIABLES      */
/*********************/

#define	XWarp	SpecialF[0]
#define	YWarp	SpecialF[1]
#define	ZWarp	SpecialF[2]

ObjNode	*gTitleLogoObj;
ObjNode	*gMainMenuIcons[NUM_MAINMENU_ICONS];

Byte	gCurrentSelection;

static	float		gMainMenuWheelRot;
static	float		gWheelCenterZ;


/************************* DO MAIN MENU ****************************/
void DoMainMenu(void)
{
FSSpec	file;
QD3DSetupInputType	viewDef;
TQ3Point3D			cameraFrom = { 0, 00, 600.0 };

do_again:

	MakeFadeEvent(true);

			/**************/
			/* INITIALIZE */
			/**************/

			/* MAKE VIEW */

	QD3D_NewViewDef(&viewDef);
	viewDef.camera.hither 			= 50;
	viewDef.camera.yon 				= 1000;
	viewDef.camera.fov 				= 1.0;
	viewDef.camera.from 	= cameraFrom;

	viewDef.view.clearColor.r 		= 0;
	viewDef.view.clearColor.g 		= 0;
	viewDef.view.clearColor.b 		= 0;

#if TWO_MEG_VERSION
	viewDef.view.paneClip.left 		+= 0;  
	viewDef.view.paneClip.right 	+= 0;  
	viewDef.view.paneClip.top		+= 80;  
	viewDef.view.paneClip.bottom 	+= 80;  
#else
	if (gGamePrefs.mainMenuHelp)
		viewDef.view.paneClip.bottom 	+= 11;
#endif

	QD3D_SetupWindow(&viewDef, &gGameViewInfoPtr);


	SetPort(gCoverWindow);
	BackColor(blackColor);              // Source port addition: ensure the background is black
	EraseRect(&gCoverWindow->portRect); // (it might not be if we're coming from the custom settings screen)

	if (gGamePrefs.mainMenuHelp)
	{
		int w;
		const char*	s = "Use the Arrow Keys to change the Selection.  Press the Spacebar to make a Selection.";
		w = TextWidthC(s);
		MoveTo(320-(w/2), 478);
		ForeColor(whiteColor);
		DrawStringC(s);
	}


			/* LOAD ART */
			
	FSMakeFSSpec(gDataSpec.vRefNum, gDataSpec.parID, ":Models:MenuInterface.3dmf", &file);
	LoadGrouped3DMF(&file, MODEL_GROUP_MENU);
	LoadASkeleton(SKELETON_TYPE_DEINON);


			/* BUILD SCENE */
			
	MakeMainMenuModels();


			/* INIT DEMO TIMER */

	QD3D_CalcFramesPerSecond();

				/*************/
				/* MAIN LOOP */
				/*************/
				
	do
	{
				/* UPDATE FRAME */
				
		GenerateFallingEgg();
		MoveObjects();
		QD3D_DrawScene(gGameViewInfoPtr,DrawObjects);
		QD3D_CalcFramesPerSecond();					
		
				/* CHECK FOR KEY INPUT */

		UpdateInput();

		if (GetNewNeedState(kNeed_UIConfirm))								// see if select
			break;

		if (GetNewNeedState(kNeed_UILeft))									// spin left
		{
			if (gCurrentSelection == 0)
				gCurrentSelection = NUM_MAINMENU_ICONS-1;
			else
				gCurrentSelection--;
			SpinToPreviousMainMenuIcon();
		}
		else
		if (GetNewNeedState(kNeed_UIRight))									// spin right
		{
			if (++gCurrentSelection >= NUM_MAINMENU_ICONS)
				gCurrentSelection = 0;
			SpinToNextMainMenuIcon();
		}
	}while(true);

	QD3D_CalcFramesPerSecond();		// call this to prime data for demo playback/record


		/***********/
		/* CLEANUP */
		/***********/

	Render_FreezeFrameFadeOut();
		
	DeleteAllObjects();
	DeleteAll3DMFGroups();
	FreeAllSkeletonFiles(-1);
	QD3D_DisposeWindowSetup(&gGameViewInfoPtr);


			/* HANDLE SELECTION */
	
	switch(gCurrentSelection)
	{
		case	0:							// play
				return;

		case	1:							// settings
				DoSettingsScreen();
				goto do_again;

		case	2:							// HELP
				ShowHelp();
				goto do_again;

		case	3:							// quit
				CleanQuit();
				return;

		case	4:							// high scores
				ShowHighScoresScreen(0);
				goto do_again;
	}
}


/********************** MAKE MAIN MENU MODELS ****************************/

static void MakeMainMenuModels(void)
{
ObjNode	*newObj;
short	i;
float	r;

			/* MAKE SKELETON */
		
	gNewObjectDefinition.type 	= SKELETON_TYPE_DEINON;
	gNewObjectDefinition.animNum = 1;
	gNewObjectDefinition.scale = .8;
	gNewObjectDefinition.coord.x = -350;
	gNewObjectDefinition.coord.y = 00;
	gNewObjectDefinition.coord.z = 630;
	gNewObjectDefinition.slot = 10;
	gNewObjectDefinition.flags = STATUS_BIT_HIGHFILTER|STATUS_BIT_DONTCULL;
	gNewObjectDefinition.moveCall = nil;
	gNewObjectDefinition.rot = gMainMenuWheelRot + (PI2 / NUM_MAINMENU_ICONS * 0);
	gMainMenuIcons[0] = MakeNewSkeletonObject(&gNewObjectDefinition);	
	gMainMenuIcons[0]->Skeleton->AnimSpeed = .8;

			/* MAKE OPTIONS ICON */
				
	gNewObjectDefinition.group = MODEL_GROUP_MENU;
	gNewObjectDefinition.type = MENU_ObjType_Options;
	gNewObjectDefinition.coord.x += 225;
	gNewObjectDefinition.scale = 1.0;
	gNewObjectDefinition.rot = gMainMenuWheelRot + (PI2 / NUM_MAINMENU_ICONS * 1);
	newObj = MakeNewDisplayGroupObject(&gNewObjectDefinition);
	newObj->XWarp = RandomFloat()*PI2;		
	newObj->YWarp = RandomFloat()*PI2;		
	newObj->ZWarp = RandomFloat()*PI2;		
	newObj->Flag[0] = 3;
	gMainMenuIcons[1] = newObj;

			/* MAKE INFO ICON */
				
	gNewObjectDefinition.type = MENU_ObjType_Info;
	gNewObjectDefinition.coord.x += 225;
	gNewObjectDefinition.rot = gMainMenuWheelRot + (PI2 / NUM_MAINMENU_ICONS * 2);
	newObj = MakeNewDisplayGroupObject(&gNewObjectDefinition);
	newObj->XWarp = RandomFloat()*PI2;		
	newObj->YWarp = RandomFloat()*PI2;		
	newObj->ZWarp = RandomFloat()*PI2;		
	newObj->Flag[0] = 4;
	gMainMenuIcons[2] = newObj;

			/* MAKE QUIT ICON */
				
	gNewObjectDefinition.type = MENU_ObjType_Quit;
	gNewObjectDefinition.coord.x += 225;
	gNewObjectDefinition.rot = gMainMenuWheelRot + (PI2 / NUM_MAINMENU_ICONS * 3);
	newObj = MakeNewDisplayGroupObject(&gNewObjectDefinition);
	newObj->XWarp = RandomFloat()*PI2;		
	newObj->YWarp = RandomFloat()*PI2;		
	newObj->ZWarp = RandomFloat()*PI2;		
	newObj->Flag[0] = 5;
	gMainMenuIcons[3] = newObj;


			/* MAKE HIGHSCORES ICON */
				
	gNewObjectDefinition.type = MENU_ObjType_HighScores;
	gNewObjectDefinition.coord.x += 225;
	gNewObjectDefinition.rot = gMainMenuWheelRot + (PI2 / NUM_MAINMENU_ICONS * 4);
	newObj = MakeNewDisplayGroupObject(&gNewObjectDefinition);
	newObj->XWarp = RandomFloat()*PI2;		
	newObj->YWarp = RandomFloat()*PI2;		
	newObj->ZWarp = RandomFloat()*PI2;		
	newObj->Flag[0] = 6;
	gMainMenuIcons[4] = newObj;


			/* MAKE BACKGROUND */
				
	gNewObjectDefinition.type = MENU_ObjType_Background;
	gNewObjectDefinition.coord.x = 0;
	gNewObjectDefinition.coord.y = 0;
	gNewObjectDefinition.coord.z = 0;
	gNewObjectDefinition.rot = 0;
	gNewObjectDefinition.scale = 5;
	gNewObjectDefinition.slot = 50;
	gNewObjectDefinition.flags = STATUS_BIT_REFLECTIONMAP;
	gNewObjectDefinition.moveCall = MoveMenuBG;
	newObj = MakeNewDisplayGroupObject(&gNewObjectDefinition);
	newObj->Scale.y *= .5;

			/* RECALC */

	for (i = 0; i < NUM_MAINMENU_ICONS; i++)
	{
		r = gMainMenuWheelRot + (PI2 / NUM_MAINMENU_ICONS * i);
	
		gMainMenuIcons[i]->Coord.x = sin(r) * WHEEL_SEPARATION;
		gMainMenuIcons[i]->Coord.z = gWheelCenterZ + (cos(r) * WHEEL_SEPARATION - 5);
		gMainMenuIcons[i]->Coord.y = MENU_RING_Y;
			if (i == 0)								// offset skeleton rot by 90 degrees
				r += PI/2;
		gMainMenuIcons[i]->Rot.y = r;
		UpdateObjectTransforms(gMainMenuIcons[i]);
	}
}


/****************** SPIN TO PREVIOUS MAIN MENU ICON ************************/
//
// Spins all the icons counter clockwise until they're in the right position.
//

static void SpinToPreviousMainMenuIcon(void)
{
	SpinMainMenuIcons(1.0f);
}


/****************** SPIN TO NEXT MAIN MENU ICON ************************/
//
// Spins all the icons clockwise until they're in the right position.
//

static void SpinToNextMainMenuIcon(void)
{
	SpinMainMenuIcons(-1.0f);
}


/****************** SPIN MAIN MENU ICONS ************************/
// Spins all the icons until they're in the right position.
// Pass in 1.0f as delta to spin counter-clockwise; -1.0f to spin clockwise.
//
// Source port refactor. The original game would also spin one loop
// iteration too far, which is fixed.

static void SpinMainMenuIcons(float delta)
{
float	targetRot,r;
Byte	i;
Boolean	stop;

	PlayEffect(EFFECT_MENUCHANGE);										// play sound

	targetRot = gMainMenuWheelRot + delta * (PI2 / NUM_MAINMENU_ICONS);	// calc target rotation
	stop = false;

	do
	{
		gMainMenuWheelRot += delta * SPIN_SPEED * gFramesPerSecondFrac;
		if ((delta < 0 && gMainMenuWheelRot < targetRot) ||
			(delta > 0 && gMainMenuWheelRot > targetRot))
		{
			gMainMenuWheelRot = targetRot;
			stop = true;
		}

		for (i = 0; i < NUM_MAINMENU_ICONS; i++)
		{
			r = gMainMenuWheelRot + (PI2 / NUM_MAINMENU_ICONS * i);
		
			gMainMenuIcons[i]->Coord.x = sin(r) * WHEEL_SEPARATION;
			gMainMenuIcons[i]->Coord.z = gWheelCenterZ + (cos(r) * WHEEL_SEPARATION - 5);
			gMainMenuIcons[i]->Coord.y = MENU_RING_Y; // - cos(r)*100;	
			if (i == 0)										// offset skeleton rot by 90 degrees
				r += PI/2;
			gMainMenuIcons[i]->Rot.y = r;
			UpdateObjectTransforms(gMainMenuIcons[i]);
		}
	
				/* UPDATE FRAME */
				
		QD3D_CalcFramesPerSecond();					
		MoveObjects();
		QD3D_DrawScene(gGameViewInfoPtr,DrawObjects);
		UpdateInput();

	} while (!stop);
}


/********************* GENERATE FALLING EGG ***********************/

static void GenerateFallingEgg(void)
{
ObjNode	*newObj;

	gEggTimer += gFramesPerSecondFrac;
	if (gEggTimer > .2)
	{
		gEggTimer = 0;

				/* MAKE HIGHSCORES ICON */
					
		gNewObjectDefinition.group = MODEL_GROUP_MENU;
		gNewObjectDefinition.type = MENU_ObjType_Egg;
		gNewObjectDefinition.coord.x = (RandomFloat()-.5) * 700;
		gNewObjectDefinition.coord.z = ((RandomFloat()-.5) * 700) + 150;
		gNewObjectDefinition.coord.y = 400;
		gNewObjectDefinition.scale = 1.0;
		gNewObjectDefinition.rot = RandomFloat() * PI;
		gNewObjectDefinition.slot = 50;
		gNewObjectDefinition.flags = 0;
		gNewObjectDefinition.moveCall = MoveFallingEgg;
		newObj = MakeNewDisplayGroupObject(&gNewObjectDefinition);
		
		newObj->Rot.x = RandomFloat()*PI;		
		newObj->Rot.z = RandomFloat()*PI;		
	}
}


/********************* MOVE FALLING EGG **********************/

static void MoveFallingEgg(ObjNode *theNode)
{
	theNode->Rot.x += gFramesPerSecondFrac;
	theNode->Rot.y += gFramesPerSecondFrac;
	theNode->Rot.z += gFramesPerSecondFrac;

	theNode->Coord.y -= gFramesPerSecondFrac * 70;
	if (theNode->Coord.y < -250)
	{
		DeleteObject(theNode);
		return;
	}

	UpdateObjectTransforms(theNode);
}


/********************* MOVE MENU BG **********************/

static void MoveMenuBG(ObjNode *theNode)
{
	theNode->Rot.y += gFramesPerSecondFrac;

	UpdateObjectTransforms(theNode);
}

