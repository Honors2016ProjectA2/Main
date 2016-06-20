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

	void TipsSelect();  // �󋵂ɍ��킹�ĊG��ς���

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
	// �q���g
	tdn2DAnim* m_typs[TIPS_TYPE::END];
	int m_selectTips;

	struct MovieInfo
	{
		tdnMovie *pMovie;	// ���旬���p
		tdn2DObj *pImage;	// ���̓����2DObj�Ƃ��ď�������ϐ�
		Vector2 pos;		// ���摜�̍��W
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