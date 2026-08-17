/* (c) Mykyta Polishyk. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "scorepickup.h"

CScorePickup::CScorePickup(CGameWorld *pGameWorld, const char *m_DeadPlayerName, int m_KillerID)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP)
{
    this->m_DeadPlayerName = (char*)m_DeadPlayerName;
    this->m_KillerID = m_KillerID;
	this->m_Vel = vec2(0,0);
    // Finding extra ID's for cool pickup
    for(int i = 0; i < 3; i++)
        m_aExtraIDs[i] = Server()->SnapNewID();
	GameWorld()->InsertEntity(this);
}

CScorePickup::~CScorePickup()
{
    for(int i = 0; i < 3; i++)
        Server()->SnapFreeID(m_aExtraIDs[i]);
}


void CScorePickup::Tick()
{
    // If player goes moves away from pickup
	CCharacter *pChrLeaving = GameServer()->m_World.ClosestCharacter(m_Pos, 64.0f, 0);
	if(pChrLeaving && pChrLeaving->IsAlive())
	{
	    // We removing text
	    GameServer()->SendBroadcast("", pChrLeaving->GetPlayer()->GetCID());
	}
    // If player goes close to pickup
	CCharacter *pChrHover = GameServer()->m_World.ClosestCharacter(m_Pos, 64.0f, 0);
	if(pChrHover && pChrHover->IsAlive())
	{
	    // We show for him this message
        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), "- 1 point dropped by %s -",
				m_DeadPlayerName);
	    GameServer()->SendBroadcast(aBuf, pChrHover->GetPlayer()->GetCID());
	}
	// Check if a player intersected with center
	CCharacter *pChr = GameServer()->m_World.ClosestCharacter(m_Pos, 20.0f, 0);
	if(pChr && pChr->IsAlive())
	{
	    // If yes we give for him 1 score point
	    // But if he steals we give for him extra point
	    if (pChr->GetPlayer()->GetCID() == m_KillerID){
            pChr->GetPlayer()->m_Score++;
            char aBuf[256];
            str_format(aBuf, sizeof(aBuf), "%s collected 1 point from player %s",
                    GameServer()->Server()->ClientName(pChr->GetPlayer()->GetCID()), m_DeadPlayerName);
            GameServer()->SendChat(-1, CGameContext::CHAT_ALL, aBuf);
	    }
	    else{
            pChr->GetPlayer()->m_Score+=2;
            char aBuf[256];
            str_format(aBuf, sizeof(aBuf), "%s collected 1 point from player %s (BONUS: 1 Extra point for steal)",
                    GameServer()->Server()->ClientName(pChr->GetPlayer()->GetCID()), m_DeadPlayerName);
            GameServer()->SendChat(-1, CGameContext::CHAT_ALL, aBuf);
	    }

	    GameServer()->m_World.RemoveEntity(this);
	}

	// Updating physics
    m_Vel.y += GameServer()->m_World.m_Core.m_Tuning.m_Gravity;
    GameServer()->Collision()->MoveBox(&m_Pos, &m_Vel, vec2(ms_PhysSize, ms_PhysSize), 0.5f);
}

void CScorePickup::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient))
		return;

    // Constructing pickup
	CNetObj_Pickup *pP1 = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	CNetObj_Pickup *pP2 = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_aExtraIDs[0], sizeof(CNetObj_Pickup)));
	CNetObj_Pickup *pP3 = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_aExtraIDs[1], sizeof(CNetObj_Pickup)));
	CNetObj_Pickup *pP4 = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_aExtraIDs[2], sizeof(CNetObj_Pickup)));
	if(!pP1 || !pP2 || !pP3 || !pP4)
		return;

    // Left top corner
	pP1->m_X = (int)m_Pos.x-8;
	pP1->m_Y = (int)m_Pos.y-8;
	pP1->m_Type = POWERUP_HEALTH;

    // Right top corner
	pP2->m_X = (int)m_Pos.x+8;
	pP2->m_Y = (int)m_Pos.y-8;
	pP2->m_Type = POWERUP_ARMOR;

    // Left bottom corner
	pP3->m_X = (int)m_Pos.x-8;
	pP3->m_Y = (int)m_Pos.y+8;
	pP3->m_Type = POWERUP_ARMOR;

    // Right bottom corner
	pP4->m_X = (int)m_Pos.x+8;
	pP4->m_Y = (int)m_Pos.y+8;
	pP4->m_Type = POWERUP_HEALTH;
}
