#pragma once

// Tipsに使えそうな情報を管理するクラス<Singleton>
class TipsCounter
{
public:
	// 実体の取得
	static TipsCounter *GetInstance(){ static TipsCounter i; return &i; }

	// デストラクタ
	~TipsCounter();

	// UIの数値を参照
	struct
	{
		int *pScore;		// スコア
		int *pWolfKill;		// うるふはっぴー
		int *pSheepKill;	// 羊死んだ数
	}m_ReferenceUI;


	int *m_NikuSheepTypes;	// 作った肉の種類(狼に何の肉を食わせたか) :ノーマル肉ばっかりなようであれば、ほかの肉を焼けることを教えてあげる
	int *m_YakinikuSteps;	// 焼けた肉の種類(生～焦げ)
	int m_DogUse;			// 犬使った数
	int m_LaneChange;		// レーン変えた数

	// カウントリセット
	void Reset();
private:

	// Singletonの作法
	TipsCounter();
	TipsCounter(const TipsCounter&){}
	TipsCounter &operator=(const TipsCounter&){}
};


#define TipsCountMgr (TipsCounter::GetInstance())