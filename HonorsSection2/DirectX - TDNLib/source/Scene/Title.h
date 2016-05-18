#pragma once
#include	"IEX_Expansion.h"

class Title : public BaseScene
{
public:
	~Title();
	bool Initialize();
	bool Update();
	void Render();
private:
	struct Object{
		tdn2DObjEx* obj;
		float scale;
		bool isSelected;
		void Bigger();
		void Smaller();
		void Init(char* fileName);
		void Render(s32 DstX, s32 DstY, s32 DstW, s32 DstH, float shake, float alpha);
	}logo, start, end;
	bool isEndGame = false;
	float shakeTheta = .0f;
	float alphaTheta = .0f;
	void ShakeUpdate();
	void AlphaUpdate();

	struct ClickBox{
		int x, y;
		int width, height;
		void Set(const int x, const int y, const int width, const int height);	//基点は中心
		bool OnMouse();
	}box[2];
	int nowSelect = -1;

	struct Performance{
		tdn2DObjEx* obj;
		int x, y;
		float roll;
		int animNum;
		int animMax;
		int animFrame;
		int animChangeFrame;
		int moveSpeed;
		int landHeight;
		int state;
		int titleSelect;
		virtual void Init(const int startX, const int startY, const float startRoll, const int startAnim);
		void SetAnimData(const int animFrame, const int animMax);
		void SetSelect(int select);
		bool Animation();
		virtual void Update();
		//ジャンプ関連
		int g;
		void Jump();
		void Run();
	}exPoint;

	struct PerformanceSinn : public Performance{
		tdn2DObjEx* normal;
		tdn2DObjEx* change;
		tdn2DObjEx* death;
		int step = 0;
		float scaleX = 1.0f;
		bool endDeath = false;
		void Init(const int startX, const int startY, const float startRoll, const int startAnim);
		void Update();
		void Reverse();
		void RunExit();
		void Death();
		void SetDeath();
	}sinn;

	struct PerformanceGorilla : public Performance{
		void Update();
	}gorilla;

	int state = 0;
	int timer = 0;

	void SelectWait();
	void Start();
	void Exit();
	void ExitEnd();

public:
	bool IsEndGame(){
		return isEndGame;
	}
};