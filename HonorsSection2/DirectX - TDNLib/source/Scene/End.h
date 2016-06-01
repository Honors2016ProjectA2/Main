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
	tdn2DObj* m_sheepPic = nullptr;
	tdn2DObj* m_wolfPic = nullptr;
	int state = 0;
	float timeUpScale = .0f;

	struct ExPointData{
		tdn2DObj* obj;
		Vector2 pos;
		int animNum;
		int animFrame;
		int moveAngle;
		int myFloor;
		enum{
			RIGHT, LEFT
		};
		void Set(const Vector2 pos, const int moveAngle, tdn2DObj* obj, const int myFloor);
		void Update();
		void Render(const int floor);
	}* data = nullptr;

	int waitTimer = 0;



	void TextPop();
	void ExPointDash();

public:
	int floorNum;

	void DataReceive(StageManager* stage);
};