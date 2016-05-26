#include "Collision.h"
#include "../Data/DataMNG.h"
#include "../CurvePoint/CurvePoint.h"
#include "../Stage/StageMNG.h"
#include "../Sound/SoundManager.h"
#include "../UI/UIManager.h"
#include "../Bokusou/Bokusou.h"
#include "../Number/Number.h"
#include "../Shake/Shake.h"
#include "Effect\EffectManager.h"

namespace{
	int GOAL_X = 0;	// 羊のゴール座標(テキストで読む)

	const int DOG_SIZE = 64;	// 犬のサイズ
	const int CENTER = 1280/2;
}

CollisionManager::CollisionManager()
{
	// テキストからパラメータ取得
	std::ifstream ifs("DATA/Text/Param/collision.txt");

	char skip[64];	// 読み飛ばし用変数

	// 羊ゴール座標読み込み
	ifs >> skip;
	ifs >> GOAL_X;
}

void CollisionManager::Update(SheepManager* sinnMNG, DataManager* dataMNG, StageManager* stageMNG)
{
	for (auto &sinIterator : *sinnMNG->Get_list()){

		Vector2 sPos = sinIterator->GetCenterPos();

		// 羊と牧草花の判定
		for (auto &kusaIterator : *BokusouMgr->GetList())
		{
			if (kusaIterator->GetFloor() != sinIterator->Get_floor()) continue;	// レーン違う

			Vector2 kPos = kusaIterator->GetCenterPos();

			// 草範囲内
			if (sPos.x < kPos.x && sPos.x > kPos.x - 128)
			{
				// 当たったやつで太った羊作成
				if (kusaIterator->GetMode() == BOKUSOU_MODE::BORN)
					sinnMNG->CreateFatSheep(sinIterator);

				// 草消す
				kusaIterator->Erase();
			}
		}

		//しんにょう脱走（加点&Time上昇判定
		if (EscapedShinnnyo(sinIterator)){
			if (sinIterator->col_check) continue;

			sinIterator->col_check = true;
			dataMNG->AddTime(sinIterator->Get_floor(),
				stageMNG->GetPopupPos(sinIterator->Get_floor(),
				sinIterator->Get_pos()->x > 640 ? true : false));		//加点、Time上昇

			//A列車 羊がゴールした瞬間
			EffectMgr.AddEffect((int)sinIterator->Get_pos()->x+220, (int)sinIterator->Get_pos()->y+64, EFFECT_TYPE::INEFFECT_MINI);


			// スコア加算処理
			SetScore(dataMNG, sinIterator->Get_floor(), sinIterator->TokutenBairitsu());

			continue;
		}

		// 羊と犬
		for (auto& dogIterator : *stageMNG->GetCurvePointList(sinIterator->Get_floor())){
			if (ExclamationPointAndCurvePoint(sinIterator, dogIterator)){
				sinIterator->SetCurve(dogIterator->GetDir());	// 羊に曲がれ命令を出す！
				//if (manIterator.col_check)continue;
				//manIterator.col_check = true;
				//if (manIterator.Get_type() != 1)// ！さんノーマルなら
				//stageMNG->CurvePointLock(manIterator.Get_floor());
				//uiMNG->SetPopup(dataMNG->GetDiffTime(), dataMNG->GetPos());
			}
		}

		//しんにょう　と！（ダメージ判定
		for (auto& manIterator : *EnemyMgr->GetList()){
			if (ShinnnyoAndExclamationPoint(sinIterator, manIterator))
			{
				if (sinIterator->col_check) continue;

				//A列車 狼と羊が当たった瞬間
				EffectMgr.AddEffect((int)sinIterator->Get_pos()->x + 96, (int)sinIterator->Get_pos()->y+64, EFFECT_TYPE::HIT);

				sinIterator->col_check = true;
				dataMNG->SubTime_Kill(sinIterator->Get_floor(), *sinIterator->Get_pos());		//時間を減少させる
				se->Play("DAMAGE",sinIterator->GetCenterPos());
			}

		}

	}	// 羊のfor分

	// 太った羊ループ
	int FatSheepIndex(0);
	for (auto &fatIt : *sinnMNG->GetFatList())
	{
		// 脱出判定
		if (EscapedFatSheep(fatIt))
		{
			//A列車 太った羊がゴールした瞬間
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x-96, (int)fatIt->GetCenterPos().y , EFFECT_TYPE::PLUS);
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x+96, (int)fatIt->GetCenterPos().y , EFFECT_TYPE::INEFFECT);


			SetScore(dataMNG, fatIt->GetFloor(), 10000);	// 10000倍
			se->Play("太った羊IN");

			// ゆらす！！
			ShakeMgr->Set();

			// 消去
			fatIt->Erase();
			continue;
		}

		int HitCount(0);	// 太った羊にあたってる数
		for (auto &sheepIt : *sinnMNG->Get_list())
		{
			if (fatIt->GetFloor() != sheepIt->Get_floor()) continue;	// レーン違う

			Vector2 sPos = sheepIt->GetCenterPos();
			Vector2 fPos = fatIt->GetCenterPos();

			// 太ってる羊範囲内
			if (sPos.x < fPos.x && sPos.x > fPos.x - 192)
			{
				// 当たった数カウント
				HitCount++;

				// 太ってない羊をずらす
				if (fPos.x - sPos.x < 128)
				{
					sheepIt->SetPos(fPos - Vector2(128, 0));
					//sheepIt->SetPosX(fPos.x - 128);
				}
			}
		}
		// 当たった数に応じて太ってる羊の移動量作成
		fatIt->SetMove(HitCount * sinnMNG->FatSheepAccel());

		FatSheepIndex++;
	}

	//犬と狼
	for(auto& manIterator : *EnemyMgr->GetList()){
		for (auto& dogIterator : *stageMNG->GetCurvePointList(manIterator->GetFloor()))
		{
			if (ExclamationDogAndWolf(dogIterator, manIterator)){

			}
		}
	}
}

bool CollisionManager::ShinnnyoAndExclamationPoint(Sheep::Base* sinn, Enemy::Base* enemy)
{
	Vector2 sinnPos, wolfPos = enemy->GetCenterPos();
	sinn->Get_pos2(sinnPos);
	Vector2 diff = sinnPos - wolfPos;
	if( enemy->GetFloor() != sinn->Get_floor() ) return false;	//階層が違う
	int size = (sinn->Get_size() + enemy->GetWidth())/2;
	if( diff.x*diff.x < size*size ){
		sinn->Be_crushed();		//しんにょうに捕まった事を教える
		return true;
	}
	return false;
}

bool CollisionManager::EscapedShinnnyo(Sheep::Base* sinn)
{
	Vector2 pos;
	sinn->Get_pos2(pos);

	return (pos.x >= GOAL_X);
}

bool CollisionManager::EscapedFatSheep(FatSheep *fat)
{
	return (fat->GetCenterPos().x >= 1400);
}

bool CollisionManager::ExclamationPointAndCurvePoint(Sheep::Base* sheep, CurvePoint* cp)
{
	// 犬置いてたら
	if (cp->IsOpening())
	{
		Vector2 SheepPos;
		Vector2 DogPos;
		sheep->Get_pos2(SheepPos);
		SheepPos.x += sheep->Get_size();
		cp->Get_pos2(DogPos);

		// 羊座標のXが犬の座標とその前方範囲内
		if (SheepPos.x > DogPos.x - DOG_SIZE && SheepPos.x < DogPos.x){
			return true;
		}
	}
	return false;
}

bool CollisionManager::ExclamationDogAndWolf(CurvePoint *cp, Enemy::Base* enemy)
{
	Vector2 WolfPos;
	Vector2 DogPos;
	WolfPos = enemy->GetCenterPos();
	cp->Get_pos2(DogPos);
	if ((DogPos.x - WolfPos.x)*(DogPos.x - WolfPos.x) < cp->GetWidth() * cp->GetWidth()){
		if (cp->IsOpening()){
			//man->Enter();
			return true;
		}
	}
	return false;
}


void CollisionManager::DebugRender(SheepManager* sinnMNG, DataManager* dataMNG, StageManager* stageMNG)
{
	for (int i = 0; i < STAGE_MAX; i++)
	{
		// 犬あたり判定
		for (auto it : *stageMNG->GetCurvePointList(i))
		{
			if(it->IsOpening())tdnPolygon::Rect((int)it->GetPos().x - DOG_SIZE, (int)it->GetPos().y, DOG_SIZE, (int)it->GetWidth(), RS::COPY, 0x0fff0000);
		}
	}
}

void CollisionManager::SetScore(DataManager *dataMNG, int floor, float bairitsu)
{
	// ★UIさんに知らせる
	int add = dataMNG->AddScore(floor, bairitsu);// データマネージャーのスコア加算関数で加算したスコアを返すから、それをUIに渡す
	UIMNG.AddScore(add);
	UIMNG.ConboCount();
	// ナンバーエフェクト発動
	NumberEffect.AddNumber(1100, STAGE_POS_Y[floor] + 32, add);
}