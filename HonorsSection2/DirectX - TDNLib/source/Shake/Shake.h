#pragma once

	//�h��֘A<Singleton>
class ShakeManager
{
public:
	// ���̎擾
	static ShakeManager *GetInstance(){ static ShakeManager i; return &i; }

	void Initialize();
	void Update();
	void Set();
	Vector2 move;
	int timer;
	int waitTimer;
	int beforeFloor;
private:
	// �V���O���g���̍�@
	ShakeManager(){}
	ShakeManager(const ShakeManager&){}
	ShakeManager &operator=(const ShakeManager&){}
};

#define ShakeMgr (ShakeManager::GetInstance())