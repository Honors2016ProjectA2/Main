
#include "IEX_Expansion.h"
#include "MousePointer.h"
#include "result.h"
#include "../Data/file_manager.h"
#include "../Sound/SoundManager.h"

Result::Result() :
NUMBERSIZE(128)
{
	black = new tdn2DObj("DATA/alpha.png");
	back = new tdn2DObj("DATA/Result/haikei.png");

	exfiles[RENDER::WIN] = new tdn2DObjEx("DATA/Result/in.png");
	exfiles[RENDER::LOSS] = new tdn2DObjEx("DATA/Result/out.png");
	in_out = nullptr;
	number = new tdn2DObj("DATA/Result/rezalut moji.png");
	for (int i = 0; i < 4; i++)
		ranking[i] = 0;
	score = 0;
	top3 = false;

	mouse = new tdn2DObj("DATA/Result/nextIcon.png");
	mouse_size = 1, ms_minus = -0.01f;

	process = 0;
	update[UPDATE::LOAD] = &Result::Load;
	update[UPDATE::SAVE] = &Result::Save;
	update[UPDATE::SHOW] = &Result::Show;

	process_r = 0;
	render[RENDER::WIN] = &Result::Win;
	render[RENDER::LOSS] = &Result::Loss;
}

Result::~Result()
{
	SAFE_DELETE(black);
	SAFE_DELETE(back);
	SAFE_DELETE(number);
	SAFE_DELETE(mouse);

	for (int i = 0; i < 2; i++)
	{
		SAFE_DELETE(exfiles[i]);
	}
}

//**************************************************

bool Result::Load()
{
	top3 = false;
	File_manager::Get_ranking("DATA/ranking.txt", ranking, 3);
	process = UPDATE::SAVE;

	return false;
}

bool Result::Save()
{
	ranking[3] = score;
	File_manager::Sort(ranking, 4);

	if (ranking[2] <= score)
	{
		top3 = true;
		in_out = exfiles[RENDER::WIN];
	}
	else
	{
		top3 = false;
		in_out = exfiles[RENDER::LOSS];
	}

	File_manager::Set_ranking("DATA/ranking.txt", ranking, 3);
	process = UPDATE::SHOW;

	se->Play("RESULT_POINT");
	return false;
}

bool Result::Show()
{
	if (mp->GetClickState() == mp->LEFT)
	{
		process = 0;
		process_r = 0;
		se->Play("CLICK");
		return true;
	}
	return false;
}

bool Result::Update()
{
	return (this->*update[process])();
}

//**************************************************

void Result::Number(int number, Vector2 *pos, int size, int space)
{
	int one = 0;
	Vector2 src(0, 0);

	for (int i = 0;; i++)
	{
		one = number % 10;
		src.x = (float)(one % 4);
		src.y = (float)(int)(one * 0.25f);
		src *= (float)NUMBERSIZE;
		this->number->Render((int)pos->x - i * space - size, (int)pos->y, size, size, (int)src.x, (int)src.y, NUMBERSIZE, NUMBERSIZE);
		
		if (number < 10)
		{
			return;
		}

		number = (int)(number * 0.1f);
	}
}

void Result::Win()
{
	switch (process_r)
	{
	case 0:
		se->Play("RANK_IN");
		size_ang = 0;
		process_r++;
		break;

	case 1:
		size_ang += 0.05f;
		if (size_ang >= 1)
			size_ang = 1, process_r++;
		break;

	case 2:
		break;

	default:
		break;
	}
	in_out->Render(830, 233, (int)(618 * size_ang), (int)(115 * size_ang), 0, 0, 1024, 128, 0);
}

void Result::Loss()
{
	switch (process_r)
	{
	case 0:
		se->Play("RANK_OUT");
		lose_pos = -612;
		size_ang = -PI*0.1f;
		process_r++;
		break;

	case 1:		
		lose_pos += 32;
		if (lose_pos >= 229)
			lose_pos = 229, timer = 0, process_r++;
		break;

	case 2:
		timer++;
		if (timer > 60)
			process_r++;
		break;

	case 3:
		size_ang += 0.03f;
		if (size_ang >= 0)
			size_ang = 0, process_r++;
		break;

	default:
		break;
	}

	in_out->Render(855, lose_pos, 618, 115, 0, 0, 512, 128, size_ang);
}

void Result::Rend_mouse()
{
	mouse_size += ms_minus;
	if (mouse_size < 0.75f)ms_minus = 0.01f;
	else if (mouse_size > 1.0f)ms_minus = -0.01f;
	float sx = 61 * mouse_size, sy = 103 * mouse_size;
	mouse->Render(1187 + 30 - (int)sx / 2, 591 + 51 - (int)sy / 2, (int)sx, (int)sy, 0, 0, 64, 128);
}

void Result::Render()
{
	black->SetARGB(0xc0000000);
	black->Render(0, 0, 1280, 720, 0, 0, 1, 1, RS::COPY);
	back->Render(0, 0, 1280, 720, 0, 0, 1280, 720);

	if (process != UPDATE::SHOW) return;

	Number(score, &Vector2(770, 140), 144, 72);

	Number(ranking[0], &Vector2(804, 315), 128, 64);
	Number(ranking[1], &Vector2(804, 445), 128, 64);
	Number(ranking[2], &Vector2(804, 565), 128, 64);

	(this->*render[top3])();

	Rend_mouse();
}
