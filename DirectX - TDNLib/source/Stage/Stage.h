#pragma once

//*****************************************************************************************************************************
//
//		人間の配置情報とか読み込むクラス<Singleton>
//
//*****************************************************************************************************************************

class Stage
{
public:

	// 実体取得
	static Stage &GetInstance();

	// テキストから人間の配置情報読み込む
	int LoadPerson();

	// ステージ番号のセッターとゲッター
	void SetStageNo(int no) { m_StageNo = no; }
	int GetStageNo(){ return m_StageNo; }

private:

	// ステージ番号
	int m_StageNo;

	// 必要達成数
	int m_ClearCount;

	// Singletonの作法
	Stage(){}
	Stage(const Stage&){}
	Stage &operator=(const Stage&){}
};

#define StageMgr (Stage::GetInstance())