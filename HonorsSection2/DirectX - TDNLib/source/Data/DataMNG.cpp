//-------- include --------
#include "DataMNG.h"
#include "TDNLIB.h"

//***********************************************
//   DataManager class
//***********************************************

//----------- static field --------------

const int DataManager::TIME_FRAME[DataManager::LEVEL_MAX]
{
	60, 30, 10, 5
};
const int DataManager::LEVEL_UP_SCORE[DataManager::LEVEL_MAX]
{
	30, 90, 110, -1
};

//--------- constructor,destructor ------------

DataManager::DataManager()
{
	frame = 0;
	time = 0;
	score = 0;
	diffTime = 0;
	pos = Vector2(0, 0);
	diffScore = 0;
	level = 0;
	popFlg = false;
}

DataManager::~DataManager()
{

}

//------------- public method -----------------

void DataManager::Init()
{
	time = 30;
	score = 0;
	diffTime = 0;
	diffScore = 0;
	frame = 0;
	level = 0;
	pos = Vector2(0, 0);
	popFlg = false;
}

void DataManager::Update()
{
	if (time > 0 && frame++ > TIME_FRAME[level])
	{
		frame = 0;
		time -= 1;
	}
}

void DataManager::Reflection(UIManager* ui)
{
	if (level < LEVEL_MAX - 1 && score >= LEVEL_UP_SCORE[level])
	{
		level++;
	}
}

Vector2 DataManager::GetPos()
{
	return pos;
}

bool DataManager::PopTrg()
{
	bool flg = popFlg;
	popFlg = false;
	return flg;
}

int DataManager::GetTime()const
{
	return time;
}

int DataManager::GetDiffTime()const
{
	return diffTime;
}

void DataManager::AddTime(int floor, Vector2& pos)
{
	popFlg = true;
	this->pos = pos;
	int add = *m_pAddScores[floor];
	diffTime = add;
	time += add;
}

void DataManager::SubTime_Kill(int floor, Vector2& pos)
{
	popFlg = true;
	this->pos = pos;
	int sub = 0;
	switch (floor)
	{
		case 0:
			sub = 5;
			break;
		case 1:
			sub = 10;
			break;
		case 2:
			sub = 15;
			break;
	}
	diffTime = -sub;
	time -= sub;
	if (time < 0)
		time = 0;
}

void DataManager::SubTime_Invade(int floor, Vector2& pos)
{
	popFlg = true;
	this->pos = pos;
	int sub = 0;
	switch (floor)
	{
		case 0:
			sub = 10;
			break;
		case 1:
			sub = 20;
			break;
		case 2:
			sub = 50;
			break;
	}
	diffTime = -sub;
	time -= sub;
	if (time < 0)
		time = 0;
}


int DataManager::GetScore()const
{
	return score;
}

int DataManager::GetDiffScore()const
{
	return diffScore;
}

int DataManager::AddScore(int floor)
{
	int add = *m_pAddScores[floor];
	diffScore = add;
	score += add;

	return add;	// ‘«‚µ‚½”‚ð•Ô‚·(‚±‚Ìî•ñ‚ÍCollision‚©‚çUI‚É“`‚í‚é)
}

//void DataManager::SubScore(int floor)
//{
//	diffScore = -value;
//	score -= value;
//}