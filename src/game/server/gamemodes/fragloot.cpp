/* (c) Mykyta Polishyk. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "fragloot.h"
#include <game/server/entities/scorepickup.h>
#include <game/server/entities/character.h>
#include <game/server/player.h>
#include <game/server/gamecontext.h>


CGameControllerFragloot::CGameControllerFragloot(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	// Exchange this to a string that identifies your game mode.
	m_pGameType = "Fragloot";
}

void CGameControllerFragloot::Tick()
{
	// this is the main part of the gamemode, this function is run every tick
	IGameController::Tick();
}

int CGameControllerFragloot::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon)
{
	// do scoreing
	if(!pKiller || Weapon == WEAPON_GAME)
		return 0;
	if(pKiller == pVictim->GetPlayer())
		pVictim->GetPlayer()->m_Score--; // suicide
	else
	{
		if(IsTeamplay() && pVictim->GetPlayer()->GetTeam() == pKiller->GetTeam())
        {
            pKiller->m_Score--; // teamkill
        }
		else
        {
            // Spawing score pickup
			CScorePickup *pScorePickup = new CScorePickup(&GameServer()->m_World, GameServer()->Server()->ClientName(pVictim->GetPlayer()->GetCID()), pKiller->GetCID());
			pScorePickup->m_Pos = pVictim->m_Pos;
        }
	}
	if(Weapon == WEAPON_SELF)
		pVictim->GetPlayer()->m_RespawnTick = Server()->Tick()+Server()->TickSpeed()*3.0f;
	return 0;
}
