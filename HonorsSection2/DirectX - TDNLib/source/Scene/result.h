
#ifndef RANKING_H
#define RANKING_H

class Result
{
private:
	tdn2DObj *black;
	tdn2DObj *back;
	tdn2DObjEx *exfiles[2];
	tdn2DObjEx *in_out;
	tdn2DObj *number;
	tdn2DObj *mouse;
	const int NUMBERSIZE;

	int ranking[4];
	int score;

	MousePointer *mp;

	//**************************************************

	int process;

	struct UPDATE
	{
		enum{ LOAD, SAVE, SHOW };
	};

	bool Load();
	bool Save();
	bool Show();

	bool(Result::*update[3])();

	//**************************************************

	void Number(int num,
		Vector2 *pos,/* ‰Eã */
		int size,
		int space);

	bool top3;

	int process_r;

	struct RENDER
	{
		enum{ WIN = true, LOSS = false };
	};

	int timer;
	int lose_pos;
	float size_ang;
	void Win();
	void Loss();

	void(Result::*render[2])();

	float mouse_size, ms_minus;
	void Rend_mouse();
public:
	Result();
	~Result();

	void Set_data(int score){ this->score = score; };
	bool Update();
	void Render();

	void Set_MousePointer(MousePointer *mp){ this->mp = mp; }
};

#endif // !RANKING_H
