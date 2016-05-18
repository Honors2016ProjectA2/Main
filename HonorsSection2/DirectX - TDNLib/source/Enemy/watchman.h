#pragma once
#include "IEX_Expansion.h"

class Watchman
{
private:
	tdn2DObj *files[3];
	tdn2DObj *obj;
	DWORD alpha;
	Vector2 pos, animepos;
	int frame, animeframe;
	const int PNGSIZE;
	const int SIZE;
	int floor;

	int move;
	enum MODE{ WALK, ENTER };
	int process;
	bool erase;

	struct TYPE { enum { NORMAL, CRUSHER, WHEEL }; };
	int type;

	void Get_frame_pos(Vector2 &pos, int frame, int max);

	bool(Watchman::*update[3])();
	bool Normal();
	bool Crusher();
	bool Wheel();
public:
	/* true:R false:L */
	Watchman(tdn2DObj *file[3], int floor, int size, int pngsize, int speed, int type);
	~Watchman();

	bool Update();
	void Render();

	Vector2 *Get_pos(){ return &pos; }
	void Get_pos2(Vector2 &out)
	{
		out = pos;
		out.x += SIZE * 0.5f;
		out.y += SIZE;
	}
	int Get_floor(){ return floor; }
	int Get_size(){ return SIZE; }

	void Delete(){ erase = true; }

	bool col_check;

	void Enter(){ process = ENTER; if (type != TYPE::CRUSHER){ frame = animeframe = 0; } }

	int Get_type(){ return type; }
};

//**************************************************

class Watchman_mng
{
private:
	tdn2DObj *files[3];
	tdn2DObj *files2[3];
	tdn2DObj *files3[3];

	std::list<Watchman> watchmans;

	const int MENS_DATA_num = 3;
	struct Mens_data
	{
		tdn2DObj *files[3];
		int size, pngsize;
		int speed;

		Mens_data(tdn2DObj *files[3], int size, int pngsize, int speed) :
			size(size), pngsize(pngsize), speed(speed){
			for (int i = 0; i < 3; i++)
			{
				this->files[i] = files[i];
			}

		};
	}*MENS_DATA[3];

	struct Creater
	{
		int timer, createtime;
		bool created;
		int type;

		int randnumber[2][3], num;
		int percentage[2][2], pnum;
	}creater[3];
	StageManager *sp;
	DataManager *dmp;

	void create(int floor);
public:
	Watchman_mng();
	~Watchman_mng();

	void Update();
	void Render();

	/* データリセット */
	void Reset();

	std::list<Watchman> *Get_list(){ return &watchmans; }
	/* 引数の floor は そのデータが作られた階層になる */
	
	/* 作られた */
	bool Created(int floor){ return creater[floor].created; }
	/* スピード */
	int Get_speed(int floor){ return MENS_DATA[creater[floor].type]->speed; }
	/* 作成されるまでの残り時間 */
	int Create_counter(int floor){ return creater[floor].createtime - creater[floor].timer; }
	/* タイプ */
	int Get_type(int floor){ return creater[floor].type; }

	void Set_Pointers(StageManager *sm, DataManager *dm){ sp = sm, dmp = dm; }

	bool col_check;
};
