#pragma once
#include "TDNLIB.h"


class Tips
{
public:
	Tips();
	~Tips();
	void Init();
	bool Update();
	void Render();

	void TipsSelect();  // 状況に合わせて絵を変える

private:
	enum class STATE
	{
		START, EXECUTE, END
	};
	STATE m_state;


	enum  TIPS_TYPE
	{
		MEAT, MEAT_TYPE, HOUSE,KUSA_GAGE, END
	};
	// ヒント
	tdn2DAnim* m_typs[TIPS_TYPE::END];
	int m_selectTips;

	struct MovieInfo
	{
		tdnMovie *pMovie;	// 動画流す用
		tdn2DObj *pImage;	// その動画を2DObjとして処理する変数
		Vector2 pos;		// 動画像の座標
		MovieInfo(char *filename, const Vector2 &pos) :pos(pos), pMovie(nullptr), pImage(nullptr)
		{
			pMovie = new tdnMovie(filename);
			pMovie->Play();
			pImage = new tdn2DObj(pMovie->GetTexture());
		}
		~MovieInfo(){ SAFE_DELETE(pMovie); SAFE_DELETE(pImage); }
		void Update(){ pMovie->LoopUpdate(); }
		void Render(){ pImage->Render((int)pos.x, (int)pos.y); }
	};
	std::vector<MovieInfo*> m_MovieList;

public:
	


};