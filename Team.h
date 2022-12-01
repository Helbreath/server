#include "Game.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <Tlhelp32.h>
#include <fstream>
#include <iostream>
#include <iosfwd>
#include <vector>
#include <sstream>
using namespace std;
extern class CGame* G_pGame;
extern class cCmd* c_cmd;
class Team
{
public:
	Team();
	virtual ~Team();
	DWORD onesec;
	bool bend;
	int endcount;
	bool bteam;
	int pjoin;
	short iteam;
	int rew;
	int ekxkill;
	int checkstop;
	void Kill(int iattacker, int target);
	void Join(int client);
	void Revive(int client);
	void DontRevive(int client);
	void RequestRevive(int client);
	void NotifyPoints();
	void End(int iteam);
	void UpdateTeamFile();
	void EnableEvent();
	void DisableEvent();
	void NotPoints(int client);
	bool MakeItems(int client, short steam);
	void CreateCape(int client, char* itemname, char color);
	void CreateBoots(int client, char* itemname, char color);
	void TeamTimer();
	void CheckAfk(int client);
	void EquipCape(int client);
	void EquipBoots(int client);
	void DeleteCape(int iClientH);
	void DeleteBoots(int iClientH);
	void Reward(int iteam);
	vector<string> m_team;
	struct {
		int kills;
		int maxkills;
	} team[4];
};
