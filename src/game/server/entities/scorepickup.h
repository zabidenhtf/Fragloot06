/* (c) Mykyta Polishyk. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_SCORE_PICKUP_H
#define GAME_SERVER_ENTITIES_SCORE_PICKUP_H

#include <game/server/entity.h>

const int PickupPhysSize = 32;

class CScorePickup : public CEntity
{
public:
	CScorePickup(CGameWorld *pGameWorld, const char *m_DeadPlayerName, int m_KillerID);
	~CScorePickup();

	virtual void Tick();
	virtual void Snap(int SnappingClient);
private:
    char *m_DeadPlayerName;
    int m_KillerID;
    int m_aExtraIDs[3];
	vec2 m_Vel;
	static const int ms_PhysSize = 14;
};

#endif
