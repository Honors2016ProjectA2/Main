#pragma once
class Sheep;
class SheepManager;
class Watchman;
class Watchman_mng;
class DataManager;
class StageManager;
class UIManager;
class CurvePoint;

class CollisionManager
{
public:
	static CollisionManager *GetInstance(){ static CollisionManager inst; return &inst; }
	void Update(SheepManager* sinnMNG, Watchman_mng* manMNG, DataManager* dataMNG, StageManager* stageMNG);
	void DebugRender(SheepManager* sinnMNG, Watchman_mng* manMNG, DataManager* dataMNG, StageManager* stageMNG);
private:
	bool ShinnnyoAndExclamationPoint(Sheep* sinn, Watchman* man);			//����ɂ傤�@�ƁI�i�_���[�W����
	bool EscapedShinnnyo(Sheep* sinn);									//����ɂ傤�E���i���_&Time�㏸����
	bool ExclamationPointAndCurvePoint(Sheep* sheep, CurvePoint* curve_point);	//�r�ƌ�
	bool ExclamationDogAndWolf(CurvePoint *cp, Watchman* man);

	// �V���O���g���̍�@
	CollisionManager();
	CollisionManager(const CollisionManager&){}
	CollisionManager &operator =(const CollisionManager&){}
};

// ���ڂ�
#define CollisionMgr (CollisionManager::GetInstance())