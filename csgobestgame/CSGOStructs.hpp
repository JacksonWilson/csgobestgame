#pragma once

#include <Windows.h>
#include "SourceEngine/SDK.hpp"
#include "NetvarManager.hpp"
#include "XorStr.hpp"

struct CHudTexture
{
	char	szShortName[64];	//0x0000
	char	szTextureFile[64];	//0x0040
	bool	bRenderUsingFont;	//0x0080
	bool	bPrecached;			//0x0081
	char	cCharacterInFont;	//0x0082
	BYTE	pad_0x0083;			//0x0083
	int		hFont;				//0x0084
	int		iTextureId;			//0x0088
	float	afTexCoords[4];		//0x008C
	int		iPosX[4];			//0x009C
};

struct WeaponInfo_t
{
	BYTE		pad_0x0000[4];			//0x0000
	bool		bParsedScript;			//0x0004
	bool		bLoadedHudElements;		//0x0005
	char		szClassName[80];		//0x0006
	char		szPrintName[80];		//0x0056
	char		szViewModel[80];		//0x00A6
	char		szWorldModel[80];		//0x00F6
	char		szBulletType[32];		//0x0146
	char		szWorldDroppedModel[80];//0x0166
	char		szAnimationPrefix[16];	//0x01B6
	BYTE		pad_0x01C6[2];			//0x01C6
	int			iSlot;					//0x01C8
	int			iPosition;				//0x01CC
	int			iMaxClip1;				//0x01D0
	int			iMaxClip2;				//0x01D4
	int			iDefaultClip1;			//0x01D8
	int			iDefaultClip2;			//0x01DC
	int			iWeight;				//0x01E0
	int			iRumbleEffect;			//0x01E4
	bool		bAutoSwitchTo;			//0x01E8
	bool		bAutoSwitchFrom;		//0x01E9
	BYTE		pad_0x01EA[2];			//0x01EA
	int			iFlags;					//0x01EC
	char		szAmmo1[16];			//0x01F0
	char		szAmmo2[16];			//0x0200
	char		szAIAddOn[80];			//0x0210
	char		aShootSounds[17][80];	//0x0260
	char PAD_NEW[80];
	int			iAmmoType;
	int			iAmmo2Type;
	bool		bMeleeWeapon;
	bool		bBuiltRightHanded;
	bool		bAllowFlipping;
	BYTE		pad_0x07BB;
	int			iSpriteCount;
	CHudTexture* iconWeaponS;
	CHudTexture* iconWeapon;
	CHudTexture* iconAmmo;
	CHudTexture* iconAmmo2;
	CHudTexture* iconCrosshair;
	CHudTexture* iconAutoaim;
	CHudTexture* iconZoomedCrosshair;
	CHudTexture* iconZoomedAutoaim;
	CHudTexture* iconWeaponSmall;
	bool		bShowUsageHint;
	BYTE		pad_0x07E5[3];
	__int32		iWeaponType;
	int			iTeam;
	int			iWeaponID;
	bool		bFullAuto;
	BYTE		pad_0x07F5[3];
	float		flHeatPerShot;
	int			iWeaponPrice;
	float		flArmorRatio;
	float		flMaxPlayerSpeed;
	float		flMaxPlayerSpeedAlt;
	int			iCrosshairMinDistance;
	int			iCrosshairDeltaDistance;
	float		flPenetration;
	int			iDamage;
	float		flRange;
	float		flRangeModifier;
	int			iBullets;
	float		flCycleTime;
	float		flCycleTimeAlt;
	char		szHeatEffect[80];
	float		aflSmokeColor[3];
	char		szMuzzleFlashEffect_1stPerson[80];
	char		szMuzzleFlashEffect_3rdPerson[80];
	char		szEjectBrassEffect[80];
	char		szTracerEffect[80];
	int			iTracerFrequency;
	float		flSpread;
	float		flSpreadAlt;
	float		flInaccuracyCrouch;
	float		flInaccuracyCrouchAlt;
	float		flInaccuracyStand;
	float		flInaccuracyStandAlt;
	float		flInaccuracyJump;
	float		flInaccuracyJumpAlt;
	float		flInaccuracyLand;
	float		flInaccuracyLandAlt;
	float		flInaccuracyLadder;
	float		flInaccuracyLadderAlt;
	float		flInaccuracyFire;
	float		flInaccuracyFireAlt;
	float		flInaccuracyMove;
	float		flInaccuracyMoveAlt;
	float		flRecoveryTimeStand;
	float		flRecoveryTimeCrouch;
	float		flInaccuracyReload;
	float		flInaccuracyAltSwitch;
	float		flRecoilAngle;
	float		flRecoilAngleAlt;
	float		flRecoilAngleVariance;
	float		flRecoilAngleVarianceAlt;
	float		flRecoilMagnitude;
	float		flRecoilMagnitudeAlt;
	float		flRecoilMagnitudeVariance;
	float		flRecoilMagnitudeVarianceAlt;
	int			iRecoilSeed;
	float		flFlinchVelocityModifierLarge;
	float		flFlinchVelocityModifierSmall;
	float		flTimeToIdle;
	float		flIdleInterval;
	float		recoilTable[2][128];
	int			iZoomLevels;
	int			iZoomFOV[3];
	float		fZoomTime[3];
	bool		bHideViewModelZoomed;
	char		szZoomInSound[80];
	char		szZoomOutSound[80];
	BYTE		pad_0x0F11[3];
	float		flBotAudibleRange;
	bool		bCanUseWithShield;
	char		szWrongTeamMsg[32];
	char		szAnimPrefix[16];
	char		szShieldViewModel[64];
	char		szAddonModel[80];
	char		szAddonLocation[80];
	char		szSilencerModel[80];
	BYTE		pad_0x1079[3];
	float		flAddonScale;
	float		flThrowVelocity;
	int			iKillAward;
	BYTE		pad_0x1088[8];
};

class C_CSPlayer;

class C_BaseCombatWeapon : public se::IClientEntity
{
    template<class T>
    inline T GetFieldValue(int offset)
    {
        return *(T*)((DWORD)this + offset);
    }

public:
    C_CSPlayer* GetOwner()
    {
        using namespace se;
        static int m_hOwnerEntity = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_hOwnerEntity"));
        return (C_CSPlayer*)Interfaces::EntityList()->GetClientEntityFromHandle(GetFieldValue<CHandle<C_CSPlayer>>(m_hOwnerEntity));
    }
    float NextPrimaryAttack()
    {
        static int m_flNextPrimaryAttack = GET_NETVAR(XorStr("DT_BaseCombatWeapon"), XorStr("LocalActiveWeaponData"), XorStr("m_flNextPrimaryAttack"));
        return GetFieldValue<float>(m_flNextPrimaryAttack);
    }
	int GetId()
	{
		typedef int(__thiscall* tGetId)(void*);
		return se::CallVFunction<tGetId>(this, 458)(this);
	}
	WeaponInfo_t* GetCSWpnData()
	{
		typedef WeaponInfo_t*(__thiscall* tGetCSWpnData)(void*);
		return se::CallVFunction<tGetCSWpnData>(this, 456)(this);
	}
    const char* GetName()
    {
        typedef const char* (__thiscall* tGetName)(void*);
        return se::CallVFunction<tGetName>(this, 378)(this);
    }
};


class C_CSPlayer : public se::IClientEntity
{
    template<class T>
    inline T GetFieldValue(int offset)
    {
        return *(T*)((DWORD)this + offset);
    }

    template<class T>
    T* GetFieldPointer(int offset)
    {
        return (T*)((DWORD)this + offset);
    }
public:
    static C_CSPlayer* GetLocalPlayer()
    {
        return (C_CSPlayer*)se::Interfaces::EntityList()->GetClientEntity(se::Interfaces::Engine()->GetLocalPlayer());
    }
    C_BaseCombatWeapon* GetActiveWeapon()
    {
		using namespace se;
        static int m_hActiveWeapon = GET_NETVAR(XorStr("DT_BaseCombatCharacter"), XorStr("m_hActiveWeapon"));
		IClientEntity * test = Interfaces::;
		CHandle<IClientEntity> fieldValueTest = GetFieldValue<CHandle<IClientEntity>>(m_hActiveWeapon);
        return (C_BaseCombatWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle(GetFieldValue<CHandle<IClientEntity>>(m_hActiveWeapon));
    }
    int GetHealth()
    {
        static int m_iHealth = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_iHealth"));
        return GetFieldValue<int>(m_iHealth);
    }
    bool IsAlive()
    {
        static int m_lifeState = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_lifeState"));
        return GetFieldValue<int>(m_lifeState) == 0;
    }
	bool isImmune()
	{
		static int m_bGunGameImmunity = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_bGunGameImmunity"));
		return GetFieldValue<int>(m_bGunGameImmunity) == 0;
	}
    int GetTeamNum()
    {
        static int m_iTeamNum = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_iTeamNum"));
        return GetFieldValue<int>(m_iTeamNum);
    }
    int GetFlags()
    {
        static int m_fFlags = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_fFlags"));
        return GetFieldValue<int>(m_fFlags);
    }
    se::Vector GetViewOffset()
    {
        static int m_vecViewOffset = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_vecViewOffset[0]"));
        return GetFieldValue<se::Vector>(m_vecViewOffset);
    }
    se::Vector GetOrigin()
    {
        static int m_vecOrigin = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_vecOrigin"));
        return GetFieldValue<se::Vector>(m_vecOrigin);
    }
    se::Vector GetEyePos()
    {
        return GetOrigin() + GetViewOffset();
    }
    se::Vector* ViewPunch()
    {
        static int m_viewPunchAngle = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_Local"), XorStr("m_viewPunchAngle"));
        return GetFieldPointer<se::Vector>(m_viewPunchAngle);
    }
    se::Vector* AimPunch()
    {
        static int m_aimPunchAngle = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_Local"), XorStr("m_aimPunchAngle"));
        return GetFieldPointer<se::Vector>(m_aimPunchAngle);
    }
};