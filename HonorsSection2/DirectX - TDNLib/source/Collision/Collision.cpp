#include	"Collision.h"
#include	"../Data/DataMNG.h"
#include	"../Enemy/watchman.h"
#include	"../Sheep/Sheep.h"
#include	"../CurvePoint/CurvePoint.h"
#include	"../Stage/StageMNG.h"
#include	"../CurvePoint/CurvePoint.h"
#include	"../Sound/SoundManager.h"

namespace{
	int GOAL_X = 0;	// 羊のゴール座標(テキストで読む)

	const int DOG_SIZE = 128;	// 犬のサイズ
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


bool CollisionManager::ShinnnyoAndExclamationPoint(Sheep* sinn, Watchman* man)
{
	Vector2 sinnPos, manPos;
	sinn->Get_pos2(sinnPos);
	man->Get_pos2(manPos);
	Vector2 diff = sinnPos - manPos;
	if( man->Get_floor() != sinn->Get_floor() ) return false;	//階層が違う
	int size = (sinn->Get_size() + man->Get_size())/2;
	if( diff.x*diff.x < size*size ){
		sinn->Be_caught(man->Get_type());		//しんにょうに捕まった事を教える
		return true;
	}
	return false;
}

bool CollisionManager::EscapedShinnnyo(Sheep* sinn)
{
	Vector2 pos;
	sinn->Get_pos2(pos);

	return (pos.x >= GOAL_X);
}

bool CollisionManager::ExclamationPointAndCurvePoint(Sheep* sheep, CurvePoint* cp)
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

bool CollisionManager::ExclamationDogAndWolf(CurvePoint *cp, Watchman* man)
{
	Vector2 WolfPos;
	Vector2 DogPos;
	man->Get_pos2(WolfPos);
	cp->Get_pos2(DogPos);
	if ((DogPos.x - WolfPos.x)*(DogPos.x - WolfPos.x) < cp->GetWidth() * cp->GetWidth()){
		if (cp->IsOpening()){
			man->Enter();
			return true;
		}
	}
	return false;
}

