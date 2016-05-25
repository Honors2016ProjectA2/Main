#pragma once

	//揺れ関連<Singleton>
class ShakeManager
{
public:
	// 実体取得
	static ShakeManager *GetInstance(){ static ShakeManager i; return &i; }

	void Initialize();
	void Update();
	void Set();
	Vector2 move;
	int timer;
	int waitTimer;
	int beforeFloor;
private:
	// シングルトンの作法
	ShakeManager(){}
	ShakeManager(const ShakeManager&){}
	ShakeManager &operator=(const ShakeManager&){}
};

#define ShakeMgr (ShakeManager::GetInstance())