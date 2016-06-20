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

	void TipsSelect();  // ó‹µ‚É‡‚í‚¹‚ÄŠG‚ð•Ï‚¦‚é

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
	// ƒqƒ“ƒg
	tdn2DAnim* m_typs[TIPS_TYPE::END];
	int m_selectTips;

	struct MovieInfo
	{
		tdnMovie *pMovie;	// “®‰æ—¬‚·—p
		tdn2DObj *pImage;	// ‚»‚Ì“®‰æ‚ð2DObj‚Æ‚µ‚Äˆ—‚·‚é•Ï”
		Vector2 pos;		// “®‰æ‘œ‚ÌÀ•W
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