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
#include "../Niku/Niku.h"
#include "../PostEffect/PostEffect.h"

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

		// プレイヤーに掴まれてたら
		if (sinIterator->isCaught())
		{
			// 肉まだ作ってなかったら
			if (!NikuMgr->GetYakiniku() && !NikuMgr->GetNiku())
			{
				// 羊と火の判定
				if ((sPos - (YAKINIKU_AREA + Vector2(128, 128))).LengthSq() < 60 * 60)
				{
					// 焼肉開始
					NikuMgr->StartYakiniku(sinIterator->GetType());

					// 火エフェクト
					EffectMgr.AddEffect((int)YAKINIKU_AREA.x + 128, (int)YAKINIKU_AREA.y +128, EFFECT_TYPE::BURN);

					// 羊消す
					sinIterator->Erase();
				}
			}
			continue;
		}

		// 羊と牧草花の判定
		for (auto &kusaIterator : *BokusouMgr->GetList())
		{
			if (kusaIterator->GetFloor() != sinIterator->Get_floor()) continue;	// レーン違う

			Vector2 kPos = kusaIterator->GetCenterPos();

			// 草範囲内
			if (sPos.x < kPos.x && sPos.x > kPos.x - 128)
			{
				// 当たったやつで太った羊作成
				if (kusaIterator->GetMode() == BOKUSOU_MODE::BORN){
					sinnMNG->CreateFatSheep(sinIterator);
					EffectMgr.AddEffect((int)sPos.x + 50, (int)sPos.y - 50, EFFECT_TYPE::ClEAR);
					EffectMgr.AddEffect((int)sPos.x - 100, (int)sPos.y, EFFECT_TYPE::PUSH);
					EffectMgr.AddEffect((int)sPos.x + 50, (int)sPos.y - 50, EFFECT_TYPE::EAT);
					// SEの再生
					se->Play("ふとる", kusaIterator->GetCenterPos());
				}

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
			EffectMgr.AddEffect((int)sinIterator->Get_pos()->x, (int)sinIterator->Get_pos()->y+64, EFFECT_TYPE::INEFFECT_MINI);
			//// プロジェクションのポジションを入手
			//Vector2 projPos = Math::GetProjPos(Vector2((float)sinIterator->Get_pos()->x + 220,
			//	(float)sinIterator->Get_pos()->y + 64));
			//// 放射ブラ―発動！
			//PostEffectMgr.SetRadialBlur(projPos, 15);

			// スコア加算処理
			SetScore(dataMNG, sinIterator->Get_floor(), sinIterator->TokutenBairitsu());

			continue;
		}

		// 羊と犬
		for (auto& dogIterator : *stageMNG->GetDogList(sinIterator->Get_floor())){
			if (ExclamationPointAndCurvePoint(sinIterator, dogIterator)){
				sinIterator->SetCurve(dogIterator->GetDir());	// 羊に曲がれ命令を出す！
				//if (manIterator.col_check)continue;
				//manIterator.col_check = true;
				//if (manIterator.Get_type() != 1)// ！さんノーマルなら
				//stageMNG->CurvePointLock(manIterator.Get_floor());
				//uiMNG->SetPopup(dataMNG->GetDiffTime(), dataMNG->GetPos());
			}
		}

		/*
		// 羊と火のダメージ判定
		for (auto &fireIt : *stageMNG->GetFireList(sinIterator->Get_floor()))
		{
			// まずおいてなかったらthrough
			if (!fireIt->IsOpening()) continue;

			Vector2 fPos;
			Vector2 sPos2;
			sinIterator->Get_pos2(sPos2);
			sPos.x += sinIterator->Get_size();
			fireIt->Get_pos2(fPos);


			// 火にあたった後の状態
			if (fireIt->GetMode() == CurvePoint::FIRE_MODE::HITED)
			{
				// 羊座標のXが犬の座標とその前方範囲内
				if (sPos.x > fPos.x - DOG_SIZE && sPos.x < fPos.x)
				{
					sinIterator->SetCurve(fireIt->GetDir());	// 羊に曲がれ命令を出す！
				}
				break;
			}

			// 判定
			if ((sPos - fireIt->GetCenterPos()).LengthSq() < 64 * 64)
			{
				// 火の最終段階なら
				if (fireIt->GetMode() == CurvePoint::FIRE_MODE::ENABLE && !sinIterator->col_check)
				{

					sinIterator->Be_crushed();		// しぬ
					sinIterator->col_check = true;

					se->Play("焼けた", *sinIterator->Get_pos());
					se->Play("悲鳴", sinIterator->GetCenterPos());

					NikuMgr->CreateNiku(sinIterator->GetCenterPos().x, sinIterator->Get_floor());

					// ヒット
					fireIt->Hit();
				}
				// それ以外
				//else fireIt->Change();// 炎消す
			}

		}
		*/

		//羊VS狼（ダメージ判定
		for (auto& manIterator : *EnemyMgr->GetList())
		{
			// 飛ばす
			if (manIterator->GetMode() != Enemy::Wolf::MODE::RUN || manIterator->GetFloor() != sinIterator->Get_floor()) continue;

			if (ShinnnyoAndExclamationPoint(sinIterator, manIterator))
			{
				// リアル羊じゃなかったら
				if (sinIterator->GetType() != SHEEP_TYPE::REAL)
				{
					if (!sinIterator->col_check)
					{
						sinIterator->Be_crushed();		//しんにょうに捕まった事を教える

						//A列車 狼と羊が当たった瞬間
						EffectMgr.AddEffect((int)sinIterator->Get_pos()->x + 96, (int)sinIterator->Get_pos()->y + 64, EFFECT_TYPE::HIT);

						sinIterator->col_check = true;

						// UIに羊が死んだ数
						UIMNG.AddSheepKillCount();

						UIMNG.AddFlame(-20);// 減点にして頂点

						// SEの再生
						se->Play("DAMAGE", sinIterator->GetCenterPos());
					}
				}
				else
				{
					if (!manIterator->col_check)
					{
						// 狼が死ぬ
						manIterator->Kill();

						//A列車 狼と羊が当たった瞬間
						EffectMgr.AddEffect((int)sinIterator->Get_pos()->x + 96, (int)sinIterator->Get_pos()->y + 64, EFFECT_TYPE::HIT);

						// 241(つよい)点
						UIMNG.AddScore(241);
						// ナンバーエフェクト発動
						NumberEffect.AddNumber((int)manIterator->GetCenterPos().x, STAGE_POS_Y[manIterator->GetFloor()] + 32, 241, Number_Effect::COLOR_TYPE::WHITE);

						manIterator->col_check = true;
						se->Play("リアルHIT", sinIterator->GetCenterPos());

						// UIに狼やっつけた
						UIMNG.AddWorfHappyCount();

						// 敵管理に狼カウント
						EnemyMgr->CheckChangeSpeed(UIMNG.GetWorfHappyCount());
					}
				}
			}
		}


	}	// 羊のfor分

	// 太った羊ループ
	for (auto &fatIt : *sinnMNG->GetFatList())
	{
		////羊VS狼（ダメージ判定
		//for (auto& manIterator : *EnemyMgr->GetList())
		//{
		//	// 飛ばす
		//	if (manIterator->GetMode() != Enemy::Wolf::MODE::RUN || manIterator->GetFloor() != fatIt->GetFloor()) continue;
		//	Vector2 fPos = fatIt->GetCenterPos(), wolfPos = manIterator->GetCenterPos();
		//	Vector2 diff = fPos - wolfPos;
		//	int size = (120 + 240) / 2;
		//	if (diff.x*diff.x < size*size){
		//		//A列車 狼と羊が当たった瞬間
		//		EffectMgr.AddEffect((int)fatIt->GetCenterPos().x + 96, (int)fatIt->GetCenterPos().y + 64, EFFECT_TYPE::HIT);
		//		fatIt->Erase();
		//		se->Play("DAMAGE", manIterator->GetCenterPos());
		//	}
		//}

		// 脱出判定
		if (EscapedFatSheep(fatIt))
		{
			//A列車 太った羊がゴールした瞬間
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x-96, (int)fatIt->GetCenterPos().y , EFFECT_TYPE::PLUS);
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x-256, (int)fatIt->GetCenterPos().y , EFFECT_TYPE::INEFFECT);
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x - 306, (int)fatIt->GetCenterPos().y+40, EFFECT_TYPE::DON);

			// プロジェクションのポジションを入手
			Vector2 projPos = Math::GetProjPos(Vector2(
				(float)fatIt->GetCenterPos().x - 96,
				(float)fatIt->GetCenterPos().y + 64));
			// 放射ブラ―発動！
			PostEffectMgr.SetRadialBlur(projPos, 18);


			// 羊のタイプに応じて分岐
			float Bairitsu;
			switch (fatIt->GetType())
			{
			case SHEEP_TYPE::NOMAL:
				Bairitsu = 1000.0f;
				break;
			case SHEEP_TYPE::GOLD:
				Bairitsu = 5000.0f;
				break;
			case SHEEP_TYPE::REAL:
				Bairitsu = 10000.0f;
				break;
			}
			SetScore(dataMNG, fatIt->GetFloor(), Bairitsu);	// 10000倍
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
			if (fatIt->GetFloor() != sheepIt->Get_floor() || !sheepIt->isPushOK()) continue;	// レーン違う

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
					//sheepIt->SetPos(fPos - Vector2(128, 0));
					sheepIt->SetPosX(fPos.x - 128);
				}

				// 押してるモードにする
				sheepIt->Be_Push();
			}
			else sheepIt->Be_Walk();
		}
		// 当たった数に応じて太ってる羊の移動量作成
		fatIt->SetAccel((HitCount * sinnMNG->FatSheepAccel()));
	}

	// 太った狼と羊
	for (auto &fatIt : *EnemyMgr->GetFatList())
	{
		// 脱出判定
		if (fatIt->GetCenterPos().x >= 1400)
		{
			//A列車 太った羊がゴールした瞬間
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x - 96, (int)fatIt->GetCenterPos().y, EFFECT_TYPE::PLUS);
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x - 256, (int)fatIt->GetCenterPos().y, EFFECT_TYPE::INEFFECT);

			// プロジェクションのポジションを入手
			Vector2 projPos = Math::GetProjPos(Vector2(
				(float)fatIt->GetCenterPos().x - 96,
				(float)fatIt->GetCenterPos().y + 64));
			// 放射ブラ―発動！
			PostEffectMgr.SetRadialBlur(projPos, 15);

			// 太ってるタイプに応じて時間を設定
			int AddTime;

			switch (fatIt->GetSheepType())
			{
			case SHEEP_TYPE::NOMAL:
				switch (fatIt->GetType())
				{
				case FAT_WOLF_TYPE::SMALL:
					AddTime = 1;
					break;
				case FAT_WOLF_TYPE::MIDDLE:
					AddTime = 5;
					break;
				case FAT_WOLF_TYPE::LARGE:
					AddTime = 10;
					break;
				}
				break;
			case SHEEP_TYPE::GOLD:
				switch (fatIt->GetType())
				{
				case FAT_WOLF_TYPE::SMALL:
					AddTime = 3;
					break;
				case FAT_WOLF_TYPE::MIDDLE:
					AddTime = 8;
					break;
				case FAT_WOLF_TYPE::LARGE:
					AddTime = 15;
					break;
				}
				break;
			case SHEEP_TYPE::REAL:
				switch (fatIt->GetType())
				{
				case FAT_WOLF_TYPE::SMALL:
					AddTime = 5;
					break;
				case FAT_WOLF_TYPE::MIDDLE:
					AddTime = 10;
					break;
				case FAT_WOLF_TYPE::LARGE:
					AddTime = 20;
					break;
				}
				break;
			}

			// UIに時間を足す
			//UIMNG.AddTimer(AddTime);
			Vector3 startPos =
				Vector3(fatIt->GetCenterPos().x-128, fatIt->GetCenterPos().y, 0.0f);
			Vector3 centerPos = Vector3(400, 600, 0);
			Vector3 center2Pos = Vector3(100, 300, 0);
			Vector3 endPos =	Vector3(720, 40, 0);

			UIMNG.AddSendPower("Data/power.png", startPos, centerPos, center2Pos, endPos, 48, AddTime);

			// UIに狼やっつけた
			UIMNG.AddWorfHappyCount();

			// 敵管理に狼カウント
			EnemyMgr->CheckChangeSpeed(UIMNG.GetWorfHappyCount());

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
			if (fatIt->GetFloor() != sheepIt->Get_floor() || !sheepIt->isPushOK()) continue;	// レーン違う

			Vector2 sPos = sheepIt->GetCenterPos();
			Vector2 fPos = fatIt->GetCenterPos();

			// 太ってる羊範囲内
			if (sPos.x < fPos.x && sPos.x > fPos.x - (fatIt->GetRadius() + 64))
			{
				// 当たった数カウント
				HitCount++;

				float vx = fPos.x - sPos.x;

				// 太ってない羊をずらす
				if (vx < fatIt->GetRadius())
				{
					//sheepIt->SetPos(fPos - Vector2(128, 0));
					sheepIt->SetPosX(fPos.x - fatIt->GetRadius());
				}

				// 押してるモードにする
				sheepIt->Be_Push();
			}
			else sheepIt->Be_Walk();
		}
		// 当たった数に応じて太ってる羊の移動量作成
		fatIt->SetAccel((HitCount * sinnMNG->FatSheepAccel()));
	}

	//犬と狼
	for(auto& manIterator : *EnemyMgr->GetList()){
		Vector2 wPos = manIterator->GetCenterPos();
		for (auto& dogIterator : *stageMNG->GetDogList(manIterator->GetFloor()))
		{
			if (ExclamationDogAndWolf(dogIterator, manIterator)){

			}
		}

		// 肉
		Niku *pNiku = NikuMgr->GetNiku();
		if (pNiku)
		{
			// まだ設置してなかったらでていけぇ！！
			if (!pNiku->isSeted()) continue;

			Vector2 nPos = pNiku->GetCenterPos();

			if ((nPos - wPos).LengthSq() < 128 * 128)
			{
				// 肉食ってるモードにする
				manIterator->ChangeMode(Enemy::Wolf::MODE::NIKU);

				// 肉の状態に応じて太る狼のタイプ設定
				switch (pNiku->GetType())
				{
				case YAKINIKU_MODE::NAMA:
				case YAKINIKU_MODE::KOGETA:
					manIterator->SetFatType(FAT_WOLF_TYPE::SMALL);
					break;
				case YAKINIKU_MODE::RARE:
					manIterator->SetFatType(FAT_WOLF_TYPE::MIDDLE);
					break;
				case YAKINIKU_MODE::MEDIAM:
					manIterator->SetFatType(FAT_WOLF_TYPE::LARGE);
					break;
				}

				// 何の羊の肉かで分岐
				manIterator->SetSheepType(pNiku->GetSheepType());

				// Delicious！！！
				EffectMgr.AddEffect((int)pNiku->GetCenterPos().x + 8, (int)pNiku->GetCenterPos().y + 32, EFFECT_TYPE::DELICIOUS);
				EffectMgr.AddEffect((int)pNiku->GetCenterPos().x, (int)pNiku->GetCenterPos().y , EFFECT_TYPE::HAPPY);
				EffectMgr.AddEffect((int)pNiku->GetCenterPos().x, (int)pNiku->GetCenterPos().y, EFFECT_TYPE::HAPPY);

				// SEの再生
				se->Play("パクモグ", manIterator->GetCenterPos());

				manIterator->SetCenterPos(pNiku->GetCenterPos());

				// 肉消す
				pNiku->Erase();
			}
		}
	}
}

bool CollisionManager::ShinnnyoAndExclamationPoint(Sheep::Base* sinn, Enemy::Wolf* enemy)
{
	Vector2 sinnPos, wolfPos = enemy->GetCenterPos();
	sinn->Get_pos2(sinnPos);
	Vector2 diff = sinnPos - wolfPos;
	int size = (sinn->Get_size() + enemy->GetWidth())/2;
	if( diff.x*diff.x < size*size ){
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

bool CollisionManager::ExclamationPointAndCurvePoint(Sheep::Base* sheep, CurvePoint::Base* cp)
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

bool CollisionManager::ExclamationDogAndWolf(CurvePoint::Base *cp, Enemy::Wolf* enemy)
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
		for (auto it : *stageMNG->GetDogList(i))
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