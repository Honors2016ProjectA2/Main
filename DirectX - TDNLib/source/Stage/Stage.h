#pragma once

//*****************************************************************************************************************************
//
//		人間の配置情報とか読み込むクラス<Singleton>
//
//*****************************************************************************************************************************

namespace StageSetCheck
{
	class Base
	{
	public:
		virtual bool Check(Vector3 &pos) = 0;		// 配置不可なエリアの判定と、その座標を書き換える
	};

	class Stage1
	{
	public:
		bool Check(Vector3 &pos);
	};

	class Stage2
	{
	public:
		bool Check(Vector3 &pos);
	};
}

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