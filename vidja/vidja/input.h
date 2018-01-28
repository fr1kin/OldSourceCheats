class CInput;
#ifndef __vidja_cinput_
#define __vidja_cinput_

#include "usercmd.h"

class CInput
{
public:
	virtual void Init_All(void);
	virtual void Shutdown_All(void);
	virtual void GetButtonBits(int);
	virtual void CreateMove(int,float,bool);
	virtual void ExtraMouseSample(float,bool);
	virtual void WriteUsercmdDeltaToBuffer(void *,int,int,bool);
	virtual void EncodeUserCmdToBuffer(void*,int);
	virtual void DecodeUserCmdFromBuffer(void*,int);
	virtual CGMUserCmd* GetUserCmd(int);
	virtual void MakeWeaponSelection(void *);
	virtual void KeyState(void *);
	virtual void KeyEvent(int,void*,char  const*);
	virtual void FindKey(char  const*);
	virtual void ControllerCommands(void);
	virtual void Joystick_Advanced(void);
	virtual void Joystick_SetSampleTime(float);
	virtual void IN_SetSampleTime(float);
	virtual void AccumulateMouse(void);
	virtual void ActivateMouse(void);
	virtual void DeactivateMouse(void);
	virtual void ClearStates(void);
	virtual void GetLookSpring(void);
	virtual void GetFullscreenMousePos(int *,int *,int *,int *);
	virtual void SetFullscreenMousePos(int,int);
	virtual void ResetMouse(void);
	virtual void GetLastForwardMove(void);
	virtual void Joystick_GetForward(void);
	virtual void Joystick_GetSide(void);
	virtual void Joystick_GetPitch(void);
	virtual void Joystick_GetYaw(void);
	virtual void CAM_Think(void);
	virtual void CAM_IsThirdPerson(void);
	virtual void CAM_ToThirdPerson(void);
	virtual void CAM_ToFirstPerson(void);
	virtual void CAM_StartMouseMove(void);
	virtual void CAM_EndMouseMove(void);
	virtual void CAM_StartDistance(void);
	virtual void CAM_EndDistance(void);
	virtual void CAM_InterceptingMouse(void);
	virtual void CAM_ToOrthographic(void);
	virtual void CAM_IsOrthographic(void)const;
	virtual void CAM_OrthographicSize(float &,float &)const;
	virtual void AddIKGroundContactInfo(int,float,float);
	virtual void LevelInit(void);
	virtual void ClearInputButton(int);
	virtual void CAM_SetCameraThirdData(void *,void*);
	virtual void CAM_CameraThirdThink(void);
	virtual void EnableJoystickMode(void);
	virtual void MouseWheeled(int);
	virtual void CAM_CapYaw(float);
	virtual void AdjustYaw(float,void*);
};

#endif