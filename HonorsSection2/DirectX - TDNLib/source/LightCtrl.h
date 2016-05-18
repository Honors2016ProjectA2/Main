#pragma once
class MousePointer;

class LightCtrl
{
public:
	LightCtrl();
	~LightCtrl();
	void Init();
	void Update();
	void Render();
	void DataReceive(MousePointer* mp);

private:
	static const int MAX_FLOOR = 3;
	float alpha[MAX_FLOOR];
	int pointerFloor = 0;
};