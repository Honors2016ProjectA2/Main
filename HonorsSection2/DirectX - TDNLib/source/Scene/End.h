#pragma once
#include	"IEX_Expansion.h"
class StageManager;

class End
{
public:
	End();
	~End();
	void Init();
	bool Update();
	void Render();
private:
	tdn2DObjEx* timeUp = nullptr;
	tdn2DObjEx* exPointR = nullptr;
	tdn2DObjEx* exPointL = nullptr;
	int state = 0;
	float timeUpScale = .0f;

	struct ExPointData{
		tdn2DObjEx* obj;
		Vector2 pos;
		int animNum;
		int animFrame;
		int moveAngle;
		int myFloor;
		enum{
			RIGHT, LEFT
		};
		void Set(const Vector2 pos, const int moveAngle, tdn2DObjEx* obj, const int myFloor);
		void Update();
		void Render(const int floor);
	}* data = nullptr;

	int waitTimer = 0;

	int floorNum;

	void TextPop();
	void ExPointDash();

public:
	void DataReceive(StageManager* stage);
};