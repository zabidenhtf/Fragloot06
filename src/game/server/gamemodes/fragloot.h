/* (c) Mykyta Polishyk. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_GAMEMODES_FRAGLOOT_H
#define GAME_SERVER_GAMEMODES_FRAGLOOT_H
#include <game/server/gamecontroller.h>

// you can subclass GAMECONTROLLER_CTF, GAMECONTROLLER_TDM etc if you want
// todo a modification with their base as well.
class CGameControllerFragloot : public IGameController
{
public:
	CGameControllerFragloot(class CGameContext *pGameServer);
	virtual void Tick();
	virtual int OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon);
};
#endif
