#pragma once
#include "../Sheep/Sheep.h"
#include "../Enemy/watchman.h"
#include "../CurvePoint/CurvePoint.h"
class DataManager;
class StageManager;
class UIManager;
class FatSheep;

class CollisionManager
{
public:
	static CollisionManager *GetInstance(){ static CollisionManager inst; return &inst; }
	void Update(SheepManager* sinnMNG, DataManager* dataMNG, StageManager* stageMNG);
	void DebugRender(SheepManager* sinnMNG, DataManager* dataMNG, StageManager* stageMNG);
private:
	bool ShinnnyoAndExclamationPoint(Sheep::Base* sinn, Enemy::Wolf* enemy);			//����ɂ傤�@�ƁI�i�_���[�W����
	bool EscapedShinnnyo(Sheep::Base* sinn);									//����ɂ傤�E���i���_&Time�㏸����
	bool EscapedFatSheep(FatSheep *fat);			// ���H���đ������r���E�o
	bool ExclamationPointAndCurvePoint(Sheep::Base* sheep, CurvePoint::Base* curve_point);	//�r�ƌ�
	bool ExclamationDogAndWolf(CurvePoint::Base *cp, Enemy::Wolf* enemy);
	void SetScore(DataManager *dataMNG, int floor, float bairitsu);

	// �V���O���g���̍�@
	CollisionManager();
	CollisionManager(const CollisionManager&){}
	CollisionManager &operator =(const CollisionManager&){}
};

// ���ڂ�
#define CollisionMgr (CollisionManager::GetInstance())