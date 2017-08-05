#pragma once
#include "/Users/jhaak/Source/Repos/csgobestgame/csgobestgame/SourceEngine/IVEngineClient.hpp"
#define IN_JUMP (1 << 1)
#define IN_ATTACK (1 << 0)
#define	HITGROUP_GENERIC 0
#define	HITGROUP_HEAD 1
#define	HITGROUP_CHEST 2
#define	HITGROUP_STOMACH 3
#define HITGROUP_LEFTARM 4
#define HITGROUP_RIGHTARM 5
#define HITGROUP_LEFTLEG 6
#define HITGROUP_RIGHTLEG 7
#define HITGROUP_GEAR 10

void TriggerBot(C_CSPlayer* pLocal, se::CUserCmd* cmd);
void AimBot(C_CSPlayer* pLocal, se::CUserCmd * cmd);

se::IVEngineClient *g_EngineClient = nullptr;

typedef bool(__thiscall* CreateMove_t)(se::IClientMode*, float, se::CUserCmd*);
CreateMove_t g_fnOriginalCreateMove = nullptr;

bool __stdcall HkCreateMove(float sample_input_frametime, se::CUserCmd* pCmd)
{
	bool bRet = g_fnOriginalCreateMove(se::Interfaces::ClientMode(), sample_input_frametime, pCmd);

	//Get the Local player pointer
	C_CSPlayer* pLocal = C_CSPlayer::GetLocalPlayer();

	//BunnyHop
	if ((pCmd->buttons & IN_JUMP) && !(pLocal->GetFlags() & (int)se::EntityFlags::FL_ONGROUND))
		pCmd->buttons &= ~IN_JUMP;

	TriggerBot(pLocal, pCmd);
	AimBot(pLocal, pCmd);

	return bRet;
}

void TriggerBot(C_CSPlayer* pLocal, se::CUserCmd* cmd)
{
	se::Vector src, dst, forward;
	se::trace_t tr;
	se::Ray_t ray;
	se::CTraceFilter filter;

	se::QAngle viewangle = cmd->viewangles;

	viewangle += *(se::QAngle*)pLocal->AimPunch() * 2.f;

	se::Math::AngleVectors(viewangle, &forward);
	forward *= 8000.f;
	filter.pSkip = pLocal;
	src = pLocal->GetEyePos();
	dst = src + forward;

	ray.Init(src, dst);

	se::Interfaces::EngineTrace()->TraceRay(ray, 0x46004003, &filter, &tr);

	if (tr.m_pEnt)
	{
		C_CSPlayer* tracedEnt = (C_CSPlayer*)tr.m_pEnt;

		int hitgroup = tr.hitgroup;
		bool didHit = false;

		if (hitgroup == HITGROUP_HEAD)
			didHit = true;
		if (hitgroup == HITGROUP_CHEST)
			didHit = true;
		if (hitgroup == HITGROUP_STOMACH)
			didHit = true;
		if ((hitgroup == HITGROUP_LEFTARM || hitgroup == HITGROUP_RIGHTARM))
			didHit = true;
		if ((hitgroup == HITGROUP_LEFTLEG || hitgroup == HITGROUP_RIGHTLEG))
			didHit = true;

		if ((tracedEnt->GetTeamNum() != pLocal->GetTeamNum()) && tracedEnt->GetHealth() > 0 /*&& !tracedEnt->isImmune()*/)
		{
			if (pressedKeys[VK_XBUTTON2] && pLocal->GetVelocity()->IsZero(20)) // Mouse 5
			{
				float hitchance = 75.f + (100.f / 4);
				if (didHit)// && (!hitchance || (1.0f - pLocal->GetWeapon(g_EntityList)->GetAccuracyPenalty()) *100.f >= hitchance))
					cmd->buttons |= IN_ATTACK;
			}
		}
	}
}

bool EntityIsValid(int i, C_CSPlayer * pLocal, se::CUserCmd * cmd)
{
	se::IClientEntityList* g_EntityList = se::Interfaces::EntityList();
	C_CSPlayer *pEntity = (C_CSPlayer*)g_EntityList->GetClientEntity(i);

	if (!pEntity)
		return false;
	if (pEntity == pLocal)
		return false;
	if (pEntity->GetHealth() <= 0)
		return false;
	if (pEntity->IsDormant())
		return false;
	if (pEntity->GetTeamNum() == pLocal->GetTeamNum())
		return false;
	//if (!IsVisible(pEntity, 6, pLocal, g_EngineTrace))
		//turn false;
	if (se::Math::GetFov(cmd->viewangles, se::Math::CalcAngle(pLocal->GetEyePos(), pEntity->GetBonePosition(6))) > 2.f)
		return false;

	return true;
}

template <typename T>
T *CaptureInterface(std::string strModule, std::string strInterface)
{
	typedef T *(*CreateInterfaceFn)(const char *szName, int iReturn);
	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(strModule.c_str()), XorStr("CreateInterface"));

	return CreateInterface(strInterface.c_str(), 0);
}

void AimBot(C_CSPlayer* pLocal, se::CUserCmd * cmd)
{
	se::Ray_t ray;
	se::trace_t tr;
	se::CTraceFilter filter;
	se::Interfaces::EngineTrace()->TraceRay(ray, 0x46004003, &filter, &tr);
	C_CSPlayer* tracedEnt = (C_CSPlayer*)tr.m_pEnt;
	se::IClientEntityList* g_EntityList = se::Interfaces::EntityList();

	float m_fbestfov = 20.f;
	int m_iBestTarget = -1;

	// DROP TARGET
	if (!((tracedEnt->GetTeamNum() != pLocal->GetTeamNum()) && tracedEnt->GetHealth() > 0))
		m_iBestTarget = -1;

	se::CGlobalVarsBase* g_Globals = se::Interfaces::GlobalVars();
	C_CSPlayer* entity;

	// FIND TARGET
	if (!(cmd->buttons & IN_ATTACK) || m_iBestTarget == -1)
	{
		for (int i = 0; i <= g_Globals->maxClients; i++)
		{
			if (!EntityIsValid(i, pLocal, cmd))
				continue;

			entity = (C_CSPlayer*)g_EntityList->GetClientEntity(i);

			if (!entity)
				continue;

			se::QAngle viewangles = cmd->viewangles + *pLocal->AimPunch() * 2.f;

			float fov = se::Math::GetFov(cmd->viewangles, se::Math::CalcAngle(pLocal->GetEyePos(), entity->GetBonePosition(6)));
			if (fov < m_fbestfov)
			{
				m_fbestfov = fov;
				m_iBestTarget = i;
			}
		}
	}

	//se::IVEngineClient* g_EngineClient = se::Interfaces::Engine();
	g_EngineClient = CaptureInterface<se::IVEngineClient>(XorStr("engine.dll"), XorStr("VEngineClient014"));

	// GO TO TARGET
	if (m_iBestTarget != -1)
	{
		pLocal = (C_CSPlayer*)g_EntityList->GetClientEntity(m_iBestTarget);
		se::Vector lPlayerVecPunch = *pLocal->AimPunch();

		if (!entity)
			return;

		se::QAngle nViewAngles;
		//se::Vector src = pLocal->GetEyePos();
		se::Vector dst = entity->GetBonePosition(8);
		se::Vector src = pLocal->GetEyePos();
		se::Vector delta;

		delta = dst - src;

		delta.Normalized();
		
		delta.x -= lPlayerVecPunch.x;
		delta.y -= lPlayerVecPunch.y;

		dst = src + delta;
		se::Math::VectorAngles(delta, nViewAngles);

		//if (pressedKeys[VK_LMENU])
		//g_EngineClient->SetViewAngles(nViewAngles.Clamp());
		if (pressedKeys[VK_SPACE]) {
			cmd->viewangles = delta;
			g_EngineClient->SetViewAngles(cmd->viewangles.Clamp());
		}
		//cmd->viewangles = nViewAngles.Clamp();
	}
}

