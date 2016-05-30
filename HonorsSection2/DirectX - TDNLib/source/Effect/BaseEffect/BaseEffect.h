#pragma once
#include "TDNLIB.h"
#include "../AnimationPanel.h"
#include "BaseEffect.h"

/*ベースエフェクト*/

class BaseEffect
{
public:
	//BaseEffect(char* name, int size, int panelNum, int flame, int upCount, bool isRoop = false);
	BaseEffect();
	~BaseEffect();

	virtual void Update();
	virtual void Render();
	virtual void Action(int x = 0, int y = 0);
	virtual void Stop();

	AnimationPanel* GetPanel() { return m_pic; }

protected:
	AnimationPanel* m_pic;
	Vector2 m_pos;
	int m_picSize;
	RS state;

};



/**********************************************/
//	継承先エフェクト
/**********************************************/

class PlusEffect :public BaseEffect
{
public:
	PlusEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/plusEffect.png", 256, 256, 15, 1, 4, false);
	};
	~PlusEffect() {};

	void Update() { BaseEffect::Update(); };
	void Render() { BaseEffect::Render(); };
	void Action(int x = 0, int y = 0) { BaseEffect::Action(x, y); };

private:
};

class HitEffect :public BaseEffect
{
public:
	HitEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/eraseEffect.png", 256, 256, 15, 1, 4, false);
		state = RS::COPY;
	};
	~HitEffect() {};

	void Update() { BaseEffect::Update(); };
	void Render() { BaseEffect::Render(); };
	void Action(int x = 0, int y = 0) { BaseEffect::Action(x, y); };

private:
};


class PutEffect :public BaseEffect
{
public:
	PutEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/nodeEffect.png", 256, 256, 15, 2, 4, false);
	};
	~PutEffect() {};

	void Update() { BaseEffect::Update(); };
	void Render() { BaseEffect::Render(); };
	void Action(int x = 0, int y = 0) { BaseEffect::Action(x, y); };

private:
};



class NoticeEffect :public BaseEffect
{
public:
	NoticeEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/bikkuri.png", 256, 256, 12, 2, 7, true, 60);
		state = RS::COPY;
	};
	~NoticeEffect() {};

	void Update() { BaseEffect::Update(); };
	void Render() { BaseEffect::Render(); };
	void Action(int x = 0, int y = 0) { BaseEffect::Action(x, y); };

private:
};

class InEffect :public BaseEffect
{
public:
	InEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/ineffect.png", 256, 256, 15, 1, 4, false);
		
		m_pic->GetPic()->SetScale(2.0f);
		
		//m_pic = new AnimationPanel("Data/Effect/in2.png", 256, 256, 15, 1, 4, false);
		//state = RS::COPY;
	};
	~InEffect() {};

	void Update() { BaseEffect::Update(); };
	void Render() { BaseEffect::Render(); };
	void Action(int x = 0, int y = 0) { BaseEffect::Action(x, y); };

private:
};

class InEffectMini :public BaseEffect
{
public:
	InEffectMini()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/in2.png", 256, 256, 15, 1, 4, false);
		//state = RS::COPY;
	};
	~InEffectMini() {};

	void Update() { BaseEffect::Update(); };
	void Render() { BaseEffect::Render(); };
	void Action(int x = 0, int y = 0) { BaseEffect::Action(x, y); };

private:
};

class DogEffect :public BaseEffect
{
public:
	DogEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/DogEffect.png", 256, 256, 15, 1, 4, false);
		//state = RS::COPY;
	};
	~DogEffect() {};

	void Update() { BaseEffect::Update(); };
	void Render() { BaseEffect::Render(); };
	void Action(int x = 0, int y = 0) { BaseEffect::Action(x, y); };

private:
};

class BurnEffect :public BaseEffect
{
public:
	BurnEffect()
	{
		m_picSize = 256;
		m_pic = new AnimationPanel("Data/Effect/burn.png", 256, 256, 24, 1, 4, false);
		//state = RS::COPY;
	};
	~BurnEffect() {};

	void Update() { BaseEffect::Update(); };
	void Render() { BaseEffect::Render(); };
	void Action(int x = 0, int y = 0) { BaseEffect::Action(x, y); };

private:
};
