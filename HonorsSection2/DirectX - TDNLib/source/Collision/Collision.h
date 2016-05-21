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
	bool ShinnnyoAndExclamationPoint(Sheep* sinn, Watchman* man);			//しんにょう　と！（ダメージ判定
	bool EscapedShinnnyo(Sheep* sinn);									//しんにょう脱走（加点&Time上昇判定
	bool ExclamationPointAndCurvePoint(Sheep* sheep, CurvePoint* curve_point);	//羊と犬
	bool ExclamationDogAndWolf(CurvePoint *cp, Watchman* man);

	// シングルトンの作法
	CollisionManager();
	CollisionManager(const CollisionManager&){}
	CollisionManager &operator =(const CollisionManager&){}
};

// ずぼら
#define CollisionMgr (CollisionManager::GetInstance())