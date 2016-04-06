#include "TDNLIB.h"
#include "Stage.h"
#include "AI\NPC\PersonManager.h"
#include "../JudgeManager/JudgeManager.h"

//**************************************************************************************************
//		実体取得
//**************************************************************************************************
Stage &Stage::GetInstance()
{
	static Stage inst;
	return inst;
}

//**************************************************************************************************
//		人間情報読み込み
//**************************************************************************************************
// 紐づけ用構造体(Map自作) 関数内で書くとエラー
template<typename T>
struct MyMap
{
	char *str;				// 文字列 "GOAL_PERSON"
	T enum_flag;			// 定数	　CLEAR_FLAG::GOAL_PERSON

	bool Check(char argstr[]){ return (strcmp(str, argstr) == 0); }

	// ずぼらしようとしたけど無理でした
	//static bool Search(MyMap Array[], char argstr[], T *out)
	//{
	//	// ループ文でテキストからとってきた文字列に応じて定数を設定
	//	for (int i = 0; i < _countof(Array); i++)
	//	{
	//		if (Array[i].Check(str))
	//		{
	//			// 文字列一致したので、定数を設定
	//			*out = Array[i].enum_flag;
	//			return true;
	//		}
	//	}
	//	return false;	// 見つからなかった！
	//}
};

int Stage::LoadPerson()
{
	char filename[128];
	sprintf_s(filename, 128, "DATA/Text/Stage/stage%d.txt", m_StageNo);
	std::ifstream ifs(filename);

	// ここで引っかかったらステージの番号がおかしいか、ステージのテキストが存在しない
	assert(ifs);

	char skip[16];		// 読み飛ばし用変数

	// 波紋回数読み込み
	int RippleCount;
	ifs >> skip;
	ifs >> RippleCount;

	// クリア条件読み込み
	char cClearFlag[16];
	ifs >> skip;
	ifs >> cClearFlag;

	/*
	文字列と定数の紐づけ処理。以下のようなソースにならないようにする

	if (strcmp(cClearFlag, "GOAL_PERSON") == 0)
	{
	m_ClearType = CLEAR_FLAG::GOAL_PERSON;
	}
	else if (strcmp(cClearFlag,"ALL_SHED")==0)
	{
	}
	*/

	// ★mapで文字列と定数の紐づけ ★Releaseにするとうまく動作しなかった経験があるので、一旦ここは使わない方針で。長くなるけど自分で作る
	//std::map<LPSTR, CLEAR_FLAG> ClearTypeList;
	//std::map<LPSTR, PERSON_TYPE> PersonTypeList;
	//
	//ClearTypeList["GOAL_PERSON"] = CLEAR_FLAG::GOAL_PERSON;
	//ClearTypeList["ALL_SHED"] = CLEAR_FLAG::ALL_SHED;
	//
	//PersonTypeList["WAIT"] = PERSON_TYPE::WAIT;
	//PersonTypeList["START"] = PERSON_TYPE::START;
	//PersonTypeList["GOAL"] = PERSON_TYPE::GOAL;


	MyMap<CLEAR_FLAG> ClearFlagList[]=
	{
		{ "GOAL_PERSON", CLEAR_FLAG::GOAL_PERSON },
		{ "ALL_SHED", CLEAR_FLAG::ALL_SHED }
	};

	MyMap<PERSON_TYPE> PersonTypeList[]=
	{
		{ "WAIT", PERSON_TYPE::WAIT },
		{ "START", PERSON_TYPE::START },
		{ "GOAL", PERSON_TYPE::GOAL }
	};

	// ループ文でテキストからとってきた文字列に応じて定数を設定(クリア条件)
	for (int i = 0; i < _countof(ClearFlagList); i++)
	{
		if (ClearFlagList[i].Check(cClearFlag))
		{
			// ★文字列一致したので、ジャッジ管理クラスにクリア条件を
			JudgeMgr.SetClearFlag(ClearFlagList[i].enum_flag);
			break;
		}
	}


	while (!ifs.eof())
	{
	
		PERSON_TYPE type;		// 人間のタイプの定数
		char cPersonType[16];	// 文字列での人間のタイプ
		Vector3 pos;			// 座標

		// 文字列での定数を読む
		ifs >> cPersonType;

		// ループ文でテキストからとってきた文字列に応じて定数を設定(人間のタイプ)
		for (int i = 0; i < _countof(PersonTypeList); i++)
		{
			if (PersonTypeList[i].Check(cPersonType))
			{
				// 文字列一致したので、定数を設定
				type = PersonTypeList[i].enum_flag;
				break;
			}
		}

		ifs >> pos.x;
		ifs >> pos.y;
		ifs >> pos.z;

		// 人間リストに追加
		PersonMgr.AddPerson(type, pos);
	}

	return RippleCount;
}