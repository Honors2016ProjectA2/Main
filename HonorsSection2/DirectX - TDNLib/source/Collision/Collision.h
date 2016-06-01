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
	bool ShinnnyoAndExclamationPoint(Sheep::Base* sinn, Enemy::Wolf* enemy);			//しんにょう　と！（ダメージ判定
	bool EscapedShinnnyo(Sheep::Base* sinn);									//しんにょう脱走（加点&Time上昇判定
	bool EscapedFatSheep(FatSheep *fat);			// 草食って太った羊が脱出
	bool ExclamationPointAndCurvePoint(Sheep::Base* sheep, CurvePoint::Base* curve_point);	//羊と犬
	bool ExclamationDogAndWolf(CurvePoint::Base *cp, Enemy::Wolf* enemy);
	void SetScore(DataManager *dataMNG, int floor, float bairitsu);

	// シングルトンの作法
	CollisionManager();
	CollisionManager(const CollisionManager&){}
	CollisionManager &operator =(const CollisionManager&){}
};

// ずぼら
#define CollisionMgr (CollisionManager::GetInstance())