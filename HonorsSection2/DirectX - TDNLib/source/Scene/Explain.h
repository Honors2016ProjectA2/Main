#pragma once
#include	"IEX_Expansion.h"

class MousePointer;

class Explain : public BaseScene
{
public:
	~Explain();
	bool Initialize();
	bool Update();
	void Render();
private:
	MousePointer* pointer = nullptr;
	tdn2DObjEx* nextIcon[2];
	int state = 0;
	float scaleTheta = .0f;
	float gameScale = .0f;
	float howToScale = .0f;
	struct Picture{
		tdn2DObjEx* obj;
		float scale;
		bool openFlg;
		bool closeFlg;

		void Init();
		bool Open();
		bool Close();
		void Render();
	}game, howTo;

	void FadeStart();
	void Text1();
	void Text2();
	void FadeEnd();
	void ScaleMove();
};