#pragma once
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

	//TriggerBot(pLocal, pCmd);

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
	forward *= pLocal->GetActiveWeapon()->GetCSWpnData()->flRange;
	filter.pSkip = pLocal;
	src = pLocal->GetEyePos();
	dst = src + forward;

	//ray.Init(src, dst);

	//se::Interfaces::EngineTrace()->TraceRay(ray, 0x46004003, &filter, &tr);

	//if (tr.m_pEnt)
	//{
	//	C_CSPlayer* tracedEnt = (C_CSPlayer*)tr.m_pEnt;

	//	int hitgroup = tr.hitgroup;
	//	bool didHit = false;

	//	if (hitgroup == HITGROUP_HEAD)
	//		didHit = true;
	//	if (hitgroup == HITGROUP_CHEST)
	//		didHit = true;
	//	if (hitgroup == HITGROUP_STOMACH)
	//		didHit = true;
	//	if ((hitgroup == HITGROUP_LEFTARM || hitgroup == HITGROUP_RIGHTARM))
	//		didHit = true;
	//	if ((hitgroup == HITGROUP_LEFTLEG || hitgroup == HITGROUP_RIGHTLEG))
	//		didHit = true;

	//	if ((tracedEnt->GetTeamNum() != pLocal->GetTeamNum()) && tracedEnt->GetHealth() > 0 && !tracedEnt->isImmune())
	//	{
	//		if (pressedKeys[VK_XBUTTON2]) // Mouse 5
	//		{
	//			float hitchance = 75.f + (100.f / 4);
	//			if (didHit)// && (!hitchance || (1.0f - pLocal->GetWeapon(g_EntityList)->GetAccuracyPenalty()) *100.f >= hitchance))
	//				cmd->buttons |= IN_ATTACK;
	//		}
	//	}
	//}
}