#pragma once
#include "TDNLIB.h"

/***********************************/
//	������`�悷��
/***********************************/

class Number
{
private:
	// �����̐F�̃^�C�v
	enum class NUM_KIND
	{
		NORMAL, HEAL
	};

public:

	// ����
	//static Number& GetInstance();
	//static void Release();

	// (TODO)��ŉ��o��ς��镪�������
	 Number();
	virtual ~Number();

	// �X�V�E�`��
	void Update();
	void Render();
	void Render(int x, int y, int num, NUM_KIND kind = NUM_KIND::NORMAL);

	// �A�N�V����
	void Action(int derayTime = 0);

	// �C���X�g�̃A�N�Z�T�[
	tdn2DAnim* GetAnim() { return m_pic; }

private:
	// ����
	//static Number* m_pInstance;

	// �����̃C���X�g
	tdn2DAnim* m_pic;
	// �����̃T�C�Y
	int m_picSize;
	int m_picScale;

	// ����
	//Number();
	//Number(const Number&);
	//Number& operator=(const Number&);




};


/***********************************/
//	���ɏオ���Ă��鐔�������o����}�l�[�W���[
/***********************************/



// ���Ŏg��������
struct NumberData
{
	enum class STATE
	{
		START,
		ARRIVAL,
		END
	};
	STATE state;

	Number* number;
	int x, y;
	int score;

	int flame;
	int endFlame;
	int alpha;

	// �I���t���O
	bool isEnd;

};

class Number_Effect
{
private:


public:

	// ����
	static Number_Effect& GetInstance();
	static void Release();
	//Number_Effect();
	virtual ~Number_Effect();

	// �X�V�E�`��
	void Update();
	void Render();

	// �����ǉ�
	void AddNumber(int x,int y,int score);

	//// NumberData
	//NumberData  

private:
	// ����
	static Number_Effect* m_pInstance;

	// �����̃f�[�^
	std::list<NumberData*> m_NumberData;
	std::list<NumberData*>::iterator m_it;

	// ����
	Number_Effect();
	Number_Effect(const Number_Effect&);
	Number_Effect& operator=(const Number_Effect&);

};

// �C���X�^���X��
#define NumberEffect	(Number_Effect::GetInstance())