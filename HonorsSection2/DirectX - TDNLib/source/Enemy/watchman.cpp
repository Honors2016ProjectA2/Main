#include "TDNLIB.h"
#include "IEX_Expansion.h"
#include "../Stage/StageMNG.h"
#include "watchman.h"
#include "../Data/DataMNG.h"

Watchman::Watchman(tdn2DObj *file[3], int floor, int size, int pngsize, int speed, int type) :
animepos(0, 0),
SIZE(size),
PNGSIZE(pngsize),
floor(floor),
type(type)
{
	for (int i = 0; i < 3; i++)
	{
		files[i] = file[i];
	}

	frame = 0;
	animeframe = 0;

	pos.y = 670.0f - 235.0f * floor;
	pos.x = (float)1280, move = -speed, obj = files[2];

	process = WALK;

	erase = false;

	alpha = 0xffffffff;

	update[0] = &Watchman::Normal;
	update[1] = &Watchman::Crusher;
	update[2] = &Watchman::Wheel;

	col_check = false;
}

Watchman::~Watchman()
{
}

//**************************************************

bool Watchman::Normal()
{
	if(frame > 5)
		frame = 0, animeframe++;
	if (animeframe > 9)
		animeframe = 0;

	if (process == ENTER)
	{
		obj = files[1], move = 0, alpha -= 0x0f000000;
		if (alpha <= 0x00ffffff) return false;
	}

	return true;
}
bool Watchman::Crusher()
{
	if (frame > 6)
		frame = 0, animeframe++;
	if (animeframe > 3)
		animeframe = 0;

	return true;
}
bool Watchman::Wheel()
{
	if (frame > 5)
		frame = 0, animeframe++;

	if (process == ENTER)
	{
		if (alpha <= 0x08ffffff)
			alpha = 0x08ffffff;
		obj = files[1], move = 0, alpha -= 0x08000000;
		if (animeframe > 5) return false;
	}
	else if (animeframe > 7)
		animeframe = 0;

	return true;
}

bool Watchman::Update()
{
	if (erase) return false;

	frame++;

	if(!(this->*update[type])()) return false;

	pos.x += move;

	return true;
}

void Watchman::Get_frame_pos(Vector2 &pos, int frame, int max)
{
	if (max == 0)
		return;
	pos.y = (float)(int)(frame / max);
	pos.x = frame - pos.y * max;

	pos.x *= PNGSIZE;
	pos.y *= PNGSIZE;
}

void Watchman::Render()
{
	Get_frame_pos(animepos, animeframe, ((type == TYPE::CRUSHER) ? 2 : 4));
	obj->SetARGB(alpha);
	obj->Render((int)pos.x, (int)pos.y - SIZE, SIZE, SIZE, (int)animepos.x, (int)animepos.y, PNGSIZE, PNGSIZE, RS::COPY);
}

//**************************************************

// manager

//**************************************************

Watchman_mng::Watchman_mng()
{
	files[0] = new tdn2DObj("DATA/CHR/「！」右移動.png");
	files[1] = new tdn2DObj("DATA/CHR/「！」奥移動.png");
	files[2] = new tdn2DObj("DATA/CHR/「！」左移動.png");

	files2[0] = new tdn2DObj("DATA/CHR/!-kai2.png");
	files2[1] = 0;
	files2[2] = new tdn2DObj("DATA/CHR/!-kai.png");

	files3[0] = new tdn2DObj("DATA/CHR/kuruma2.png");
	files3[1] = new tdn2DObj("DATA/CHR/kuruma back.png");
	files3[2] = new tdn2DObj("DATA/CHR/kuruma.png");

	Reset();

	/*
		第一
		2 4 6
		5 6 7
		4 8 10

		第二
		2 2.5	190
		3 4.5 5	160
		2 3   4	130
	*/
	/*
	// 出現間隔
	// 第一
	creater[2].randnumber[0][0] = 120;
	creater[2].randnumber[0][1] = 240;
	creater[2].randnumber[0][2] = 360;

	creater[1].randnumber[0][0] = 300;
	creater[1].randnumber[0][1] = 360;
	creater[1].randnumber[0][2] = 420;

	creater[0].randnumber[0][0] = 240;
	creater[0].randnumber[0][1] = 480;
	creater[0].randnumber[0][2] = 600;

	// 第二
	creater[2].randnumber[1][0] = 120;
	creater[2].randnumber[1][1] = 150;
	creater[2].randnumber[1][2] = 150;

	creater[1].randnumber[1][0] = 180;
	creater[1].randnumber[1][1] = 270;
	creater[1].randnumber[1][2] = 300;

	creater[0].randnumber[1][0] = 120;
	creater[0].randnumber[1][1] = 180;
	creater[0].randnumber[1][2] = 240;

	// 出現割合 (パーセンテージ)
	// 第一
	creater[2].percentage[0][0] = 50;// タイヤ
	creater[2].percentage[0][1] = 30;// ゴリラ

	creater[1].percentage[0][0] = 33;
	creater[1].percentage[0][1] = 33;

	creater[0].percentage[0][0] = 0;
	creater[0].percentage[0][1] = 0;

	// 第二
	creater[2].percentage[1][0] = 45;
	creater[2].percentage[1][1] = 45;

	creater[1].percentage[1][0] = 40;
	creater[1].percentage[1][1] = 40;

	creater[0].percentage[1][0] = 50;
	creater[0].percentage[1][1] = 20;

	MENS_DATA[0] = new Mens_data(files, 96, 128, 2);
	MENS_DATA[1] = new Mens_data(files2, 200, 256, 3);
	MENS_DATA[2] = new Mens_data(files3, 200, 256, 12);
	*/

	std::fstream fst("DATA/Text/Param/tekidata.txt");
	assert(fst);	// ファイルがない！

	fst >> creater[2].randnumber[0][0];
	fst >> creater[2].randnumber[0][1];
	fst >> creater[2].randnumber[0][2];

	fst >> creater[1].randnumber[0][0];
	fst >> creater[1].randnumber[0][1];
	fst >> creater[1].randnumber[0][2];

	fst >> creater[0].randnumber[0][0];
	fst >> creater[0].randnumber[0][1];
	fst >> creater[0].randnumber[0][2];

	// 第二
	fst >> creater[2].randnumber[1][0];
	fst >> creater[2].randnumber[1][1];
	fst >> creater[2].randnumber[1][2];

	fst >> creater[1].randnumber[1][0];
	fst >> creater[1].randnumber[1][1];
	fst >> creater[1].randnumber[1][2];

	fst >> creater[0].randnumber[1][0];
	fst >> creater[0].randnumber[1][1];
	fst >> creater[0].randnumber[1][2];

	/* 出現割合 (パーセンテージ) */
	// 第一
	fst >> creater[2].percentage[0][0];/* タイヤ */
	fst >> creater[2].percentage[0][1];/* ゴリラ */

	fst >> creater[1].percentage[0][0];
	fst >> creater[1].percentage[0][1];

	fst >> creater[0].percentage[0][0];
	fst >> creater[0].percentage[0][1];

	// 第二
	fst >> creater[2].percentage[1][0];
	fst >> creater[2].percentage[1][1];

	fst >> creater[1].percentage[1][0];
	fst >> creater[1].percentage[1][1];

	fst >> creater[0].percentage[1][0];
	fst >> creater[0].percentage[1][1];

	int speed;
	fst >> speed;
	MENS_DATA[0] = new Mens_data(files, 200, 128, speed);
	fst >> speed;
	MENS_DATA[1] = new Mens_data(files2, 200, 256, speed);
	fst >> speed;
	MENS_DATA[2] = new Mens_data(files3, 200, 256, speed);

	creater[0].created = true;
	creater[1].created = true;
	creater[2].created = true;
	sp = 0;
}

Watchman_mng::~Watchman_mng()
{
	for (int i = 0; i < 3; i++)
	{
		SAFE_DELETE(files[i]);
		SAFE_DELETE(files2[i]);
		SAFE_DELETE(files3[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		SAFE_DELETE(MENS_DATA[i]);
	}
}

//**************************************************

void Watchman_mng::create(int floor)
{
	watchmans.push_back(Watchman(MENS_DATA[creater[floor].type]->files, floor, MENS_DATA[creater[floor].type]->size, MENS_DATA[creater[floor].type]->pngsize, MENS_DATA[creater[floor].type]->speed, creater[floor].type));
	creater[floor].created = true;
}

void Watchman_mng::Update()
{
	if (dmp && dmp->GetScore() >= 230)
	{
		creater[2].num = 1;
		creater[0].pnum = creater[1].pnum = creater[2].pnum = 1;
	}
	else if (dmp && dmp->GetScore() >= 170)
		creater[1].num = 1;
	else if (dmp && dmp->GetScore() >= 110)
		creater[0].num = 1;

	if (sp)
	{
		Mens_data **m = MENS_DATA;
		//Creater *c = creater;

		for (int i = 0; i < 3; i++)
		{
			if (creater[i].created)
			{
				creater[i].created = false;
				creater[i].timer = 0;
				creater[i].createtime = creater[i].randnumber[creater[i].num][tdnRandom::Get(0, 2)];
				{
					int per = rand() % 100;

					if ((per -= creater[i].percentage[creater[i].pnum][0]) < 0)
						creater[i].type = 2;
					else if ((per -= creater[i].percentage[creater[i].pnum][1]) < 0)
						creater[i].type = 1;
					else 
						creater[i].type = 0;
				}
			}

			creater[i].timer++;
			if (creater[i].timer > creater[i].createtime)
			{
				create(i);
			}
		}
	}

	for (std::list<Watchman>::iterator it = watchmans.begin(); it != watchmans.end();)
	{
		if (!it->Update() || it->Get_pos()->x < -2048 || it->Get_pos()->x > 2048)
		{
			it = watchmans.erase(it); 
			continue;
		}
		it++;
	}
}

void Watchman_mng::Render()
{
	for (std::list<Watchman>::iterator it = watchmans.begin(); it != watchmans.end(); it++)
	{
		it->Render();
	}
}

void Watchman_mng::Reset()
{
	watchmans.clear();
	for (int i = 0; i < 3; i++)
	{
		creater[i].created = false;
		creater[i].timer = 0;
		creater[i].num = 0;
		creater[i].createtime = 1200;
		creater[i].type = 0;
		creater[i].pnum = 0;
	}
}
