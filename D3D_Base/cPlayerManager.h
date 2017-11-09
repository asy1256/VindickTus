#pragma once

class cCharacter;
class cBerserker;

enum Plyerkey
{
	PLAYER_BERSERKER = 0,
	PLAYER_END
};

class cPlayerManager
{
private:
	SYNTHESIZE_REF(vector<cCharacter*>, m_vPlayer, vPlayer);
	
public:
	cPlayerManager();
	~cPlayerManager();

	void SetUp();
	void Release();
	void Update();
	void Render();

	void AddPlay(Plyerkey p);
};

