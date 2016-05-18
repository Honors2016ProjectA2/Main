//--------- include ---------
#include "UIMNG.h"
#include "TDNLIB.h"
#include "IEX_Expansion.h"
#include "../Stage/StageMNG.h"
#include "../Data/DataMNG.h"
#include "../Sheep/Sheep.h"
#include "../Enemy/watchman.h"
#include "../Sound/SoundManager.h"

//*****************************************
//   UIManager class
//*****************************************

//-------- constructor,destructor ----------

UIManager::UIManager()
{
	for (int i = 0; i < SOURCE_MAX; i++)
		src[i] = nullptr;
	for (int i = 0; i < ARROW_MAX; i++)
		arrow[i] = nullptr;
	for (int i = 0; i < DIFPOP_MAX; i++)
		difPop[i] = nullptr;
	for (int i = 0; i < WARNING_MAX; i++)
		warning[i] = nullptr;
	for (int i = 0; i < LOCK_MAX; i++)
		lockCount[i] = nullptr;
	time = nullptr;
	score = nullptr;
	speedUp = nullptr;
	mouse = nullptr;
	sppedUpFlg = false;
}

UIManager::~UIManager()
{
	for (int i = 0; i < SOURCE_MAX; i++)
		SAFE_DELETE(src[i]);
	SAFE_DELETE(time);
	SAFE_DELETE(score);
	SAFE_DELETE(speedUp);
	SAFE_DELETE(mouse);
	for (int i = 0; i < ARROW_MAX; i++)
		SAFE_DELETE(arrow[i]);
	for (int i = 0; i < DIFPOP_MAX; i++)
		SAFE_DELETE(difPop[i]);
	for (int i = 0; i < WARNING_MAX; i++)
		SAFE_DELETE(warning[i]);
	for (int i = 0; i < LOCK_MAX; i++)
		SAFE_DELETE(lockCount[i]);
}

//--------- public method ----------

void UIManager::Init()
{
	//-------- image delete -----------
	for (int i = 0; i < SOURCE_MAX; i++)
	{
		SAFE_DELETE(src[i]);
	}

	//-------- image load -------------
	src[TIME_TITLE]   = new tdn2DObjEx("DATA/UI/Time.png");
	src[MOUSE]        = new tdn2DObjEx("DATA/UI/icon.png");
	src[SHINNYO]      = new tdn2DObjEx("DATA/UI/Sinsyou.png");
	src[NUMBER]       = new tdn2DObjEx("DATA/UI/Score&Time.png");
	src[DIF_POP]      = new tdn2DObjEx("DATA/UI/Puras Mainasu.png");
	src[ARROW_RIGHT]  = new tdn2DObjEx("DATA/UI/Yajirusi migi.png");
	src[ARROW_LEFT]   = new tdn2DObjEx("DATA/UI/Yajirusi hidari.png");
	src[WARNING]      = new tdn2DObjEx("DATA/UI/フキダシ.png");
	src[SPEED_UP]     = new tdn2DObjEx("DATA/UI/Speed UP!.png");
	src[SPEED_ARROW]  = new tdn2DObjEx("DATA/UI/Speed Yajirusi.png");
	src[BALLOON]      = new tdn2DObjEx("DATA/UI/hukidasi.png");

	//--------- instance delete ------------
	for (int i = 0; i < ARROW_MAX; i++)
		SAFE_DELETE(arrow[i]);
	for (int i = 0; i < DIFPOP_MAX; i++)
		SAFE_DELETE(difPop[i]);
	for (int i = 0; i < WARNING_MAX; i++)
		SAFE_DELETE(warning[i]);
	for (int i = 0; i < LOCK_MAX; i++)
		SAFE_DELETE(lockCount[i]);
	SAFE_DELETE(time);
	SAFE_DELETE(score);
	SAFE_DELETE(speedUp);
	SAFE_DELETE(mouse);

	//--------- instance set ----------
	time = new UI_Time;
	score = new UI_Score;
	mouse = new UI_Mouse(src[MOUSE], Vector2(1280 / 2 + 199 / 2 + 20, 720 * 2 / 3 + 40));

	time->Init();
	score->Init();
	sppedUpFlg = false;
}

void UIManager::Update()
{
	time->Update();
	score->Update();
	if (mouse != nullptr)
	{
		mouse->Update();
		if (mouse->IsFinish)
			SAFE_DELETE(mouse);
	}

	if (speedUp != nullptr)
	{
		speedUp->Update();
		if (speedUp->IsFinish)
			SAFE_DELETE(speedUp);
	}
	for (int i = 0; i < ARROW_MAX; i++)
	{
		if (arrow[i]== nullptr)continue;
		arrow[i]->Update();
	}
	for (int i = 0; i < DIFPOP_MAX; i++)
	{
		if (difPop[i] == nullptr)continue;
		difPop[i]->Update();
		if (difPop[i]->IsFinish)
			SAFE_DELETE(difPop[i]);
	}
	for (int i = 0; i < WARNING_MAX; i++)
	{
		if (warning[i] == nullptr)continue;
		warning[i]->Update();
		if (warning[i]->IsFinish)
			SAFE_DELETE(warning[i]);
	}
	for (int i = 0; i < LOCK_MAX; i++)
	{
		if (lockCount[i] == nullptr)continue;
		lockCount[i]->Update();
		if (lockCount[i]->IsFinish)
			SAFE_DELETE(lockCount[i]);
	}
}

void UIManager::Render_Back()
{
	for (int i = 0; i < ARROW_MAX; i++)
	{
		if (arrow[i] == nullptr)continue;
		arrow[i]->Render();
	}
	for (int i = 0; i < LOCK_MAX; i++)
	{
		if (lockCount[i] == nullptr)continue;
		lockCount[i]->Render(src[NUMBER],src[BALLOON]);
	}
	if (mouse != nullptr)
		mouse->Render();
}

void UIManager::Render_Front()
{
	time->Render(src[TIME_TITLE], src[NUMBER], nullptr, Vector2(11, 10));
	if (speedUp != nullptr)
		speedUp->Render(Vector2((float)time->endPosX, 10));
	score->Render(src[NUMBER], src[SHINNYO], Vector2(1224, 10));
	for (int i = 0; i < DIFPOP_MAX; i++)
	{
		if (difPop[i] == nullptr)continue;
		difPop[i]->Render();
	}
	for (int i = 0; i < WARNING_MAX; i++)
	{
		if (warning[i] == nullptr)continue;
		warning[i]->Render();
	}
}

void UIManager::Render_End()
{
	for (int i = 0; i < DIFPOP_MAX; i++)
	{
		if (difPop[i] == nullptr)continue;
		difPop[i]->Render();
	}
}

void UIManager::Reflection(StageManager* stage, SheepManager* shinnyo, Watchman_mng* watchman, DataManager* data)
{
	// time,score check
	time->SetNum(data->GetTime());
	score->SetNum(data->GetScore());

	if (sppedUpFlg)
	{
		sppedUpFlg = false;
	}

	//if (data->PopTrg())
	//	SetPopup(data->GetDiffTime(), data->GetPos());
	SetArrow(stage, shinnyo);
	SetWarning(watchman, stage);
}

void UIManager::SetPopup(int val, Vector2& pos)
{
	UI_DifPop::PopType type = UI_DifPop::PLUS;
	UI_DifPop::PopNum num = UI_DifPop::NUM_0;

	switch (val)
	{
		case 5:
			type = UI_DifPop::PLUS;
			num = UI_DifPop::NUM_0;
			break;
		case 10:
			type = UI_DifPop::PLUS;
			num = UI_DifPop::NUM_1;
			break;
		case 15:
			type = UI_DifPop::PLUS;
			num = UI_DifPop::NUM_2;
			break;
		case -5:
			type = UI_DifPop::MINUS_1;
			num = UI_DifPop::NUM_0;
			break;
		case -10:
			type = UI_DifPop::MINUS_1;
			num = UI_DifPop::NUM_1;
			break;
		case -15:
			type = UI_DifPop::MINUS_1;
			num = UI_DifPop::NUM_2;
			break;
		case -20:
			type = UI_DifPop::MINUS_2;
			num = UI_DifPop::NUM_0;
			break;
		case -50:
			type = UI_DifPop::MINUS_2;
			num = UI_DifPop::NUM_1;
			break;
		case -100:
			type = UI_DifPop::MINUS_2;
			num = UI_DifPop::NUM_2;
			break;
	}
	for (int i = 0; i < DIFPOP_MAX; i++)
	{
		if (difPop[i] == nullptr)
		{
			difPop[i] = new UI_DifPop(src[DIF_POP], pos, type, num);
			break;
		}
	}
}

void UIManager::SetSpeedUp()
{
	SAFE_DELETE(speedUp);
	speedUp = new UI_SpeedUp(src[SPEED_UP], src[SPEED_ARROW]);
	se->Play("SPEED_UP");
}

void UIManager::SetArrow(StageManager* stage, SheepManager* shinnyo)
{
	//for (int floor = 0; floor < stage->STAGE_MAX; floor++)
	//{
	//	// shutter arrow
	//	// open check
	//	if (stage->IsApperd(floor) && !stage->IsLocked(floor))
	//	{
	//		// append arrow
	//		if (arrow[floor] != nullptr)
	//		{
	//			arrow[floor]->SetImage(
	//				shinnyo->Get_right_left(floor) ?
	//				src[ARROW_RIGHT] : src[ARROW_LEFT]);
	//		}
	//		else
	//		{
	//			arrow[floor] = shinnyo->Get_right_left(floor) ?
	//				new UI_Arrow(src[ARROW_RIGHT], stage->GetArrowPos(floor)) :
	//				new UI_Arrow(src[ARROW_LEFT], stage->GetArrowPos(floor));
	//		}

	//		if (stage->IsOpen(floor))
	//			arrow[floor]->FadeIn();
	//		else
	//			arrow[floor]->FadeOut();
	//	}
	//	else
	//	{
	//		// erase arrow
	//		if (arrow[floor] != nullptr)
	//			SAFE_DELETE(arrow[floor]);
	//	}
	//}
}

void UIManager::SetWarning(Watchman_mng* watchman, StageManager* stage)
{
	for (int floor = 0; floor < STAGE_MAX; floor++)
	{
		// warning
		int CreateCounter = watchman->Create_counter(floor);	// 敵が出てくる残り時間
		if ((CreateCounter < 140 && watchman->Get_type(floor) > 0) ||
			(CreateCounter < 170 && watchman->Get_type(floor) <= 0))
		{
			if (warning[floor + 3] == nullptr)
			{
				//SAFE_DELETE(warning[floor + (watchman->Get_right_left(floor) ? 0 : 3)]);
				warning[floor + 3] =
					new UI_Warning(src[WARNING], stage->GetWarningPos(floor, true), watchman->Get_type(floor)>0, true);
				se->Play(watchman->Get_type(floor) > 0 ? "BALLOON_R" : "BALLOON_N");
			}
		}
	}
}

//**************************************************
//  Number class
//*************************************************

//--------- constructor,destructor --------------

NumberUI::NumberUI(int sw,int sh):
NUM_W(50), NUM_H(50), NUM_SX(0), NUM_SY(0), NUM_SW(64), NUM_SH(64), NUM_SR(8), NUM_SC(2),
UNIT_W(34), UNIT_H(34), UNIT_SX(0), UNIT_SY(0), UNIT_SW(sw), UNIT_SH(sh)
{
	num = 0;
	endPosX = 0;
}
	
NumberUI::~NumberUI()
{

}

//-------- pubic method -------------

void NumberUI::Init()
{
	num = 0;
	endPosX = 0;
}

void NumberUI::Render_Left(tdn2DObj*number, tdn2DObj* unit, Vector2 pos)
{
	int digitCount = 0;
	int nomalDigit = 3;
	int numbuf = num;
	int zeroFlg = false;
	int div = 1;
	//-------- 左座標基準右揃え --------
	for (int i = 0; numbuf > 0; i++)
	{
		digitCount++;
		div *= 10;
		numbuf /= 10;
	}
	if (digitCount < nomalDigit)
	{
		while (digitCount++<nomalDigit)
			div *= 10;
	}
	// 数字描画
	if (number != nullptr)
	{
		for (numbuf = num, digitCount = 0; div > 0; digitCount++, div /= 10)
		{
			int digNum = numbuf / div;
			if (digNum > 0 || zeroFlg || div <= 1)
			{
				zeroFlg = true;
				number->Render((int)pos.x + (NUM_W*digitCount), (int)pos.y - 8, NUM_W, NUM_H,
							   NUM_SX + (digNum % NUM_SR * NUM_SW), NUM_SY + (digNum / NUM_SR * NUM_SH), NUM_SW, NUM_SH);
			}
			numbuf %= div;
		}
	}
	// 単位描画
	if (unit != nullptr)
	{
		unit->Render((int)pos.x + (NUM_W*digitCount), (int)pos.y, UNIT_W, UNIT_H,
					 UNIT_SX, UNIT_SY, UNIT_SW, UNIT_SH);
	}
	// 最終描画位置記憶
	endPosX = (int)pos.x + (NUM_W*digitCount) + UNIT_SW;
}

void NumberUI::Render_Right(tdn2DObj*number, tdn2DObj* unit, Vector2 pos)
{
	int digitCount = 0;
	int nomalDigit = 3;
	int numbuf = num;
	int div = 1;
	//-------- 右座標基準右揃え --------
	// 単位描画
	if (unit != nullptr)
	{
		unit->Render((int)pos.x, (int)pos.y, UNIT_W, UNIT_H,
					 UNIT_SX, UNIT_SY, UNIT_SW, UNIT_SH);
	}
	// 数字描画
	if (number != nullptr)
	{
		for (numbuf = num, digitCount = 0; numbuf / div > 0 || digitCount < 3; digitCount++, div *= 10)
		{
			int digNum = numbuf / div;
			if (digNum > 0 || div <= 1)
			{
				digNum %= 10;
				number->Render((int)pos.x - NUM_W*(digitCount + 1), (int)pos.y - 8, NUM_W, NUM_H,
							   NUM_SX + (digNum % NUM_SR * NUM_SW), NUM_SY + (digNum / NUM_SR * NUM_SH), NUM_SW, NUM_SH);
			}
		}
	}
	// 最終描画位置記憶
	endPosX = (int)pos.x - NUM_W*(digitCount + 1) + UNIT_SW;
}

void NumberUI::SetNum(int value)
{
	num = value;
}

//**************************************
//  BaseUI class
//**************************************

//--------- constructor,destructor -----------

BaseUI::BaseUI(tdn2DObjEx* image, Vector2& pos, int w, int h, int sx, int sy, int sw, int sh,
			   int sr, int sc, int frameMax, int roopTime):
			   W(w), H(h), SX(sx), SY(sy), SW(sw), SH(sh),
			   SR(sr), SC(sc)
{
	this -> image = image;
	this->pos = pos;
	animeFrameMax = frameMax;
	animeRoopTime = roopTime;
	frame = 0;
	offsetBlockR = 0;
	offsetBlockC = 0;
	timer = 0;
	state = 0;
	IsFinish = false;
}

BaseUI::BaseUI(const BaseUI& r) :
W(r.W), H(r.H), SX(r.SX), SY(r.SY), SW(r.SW), SH(r.SH),
SR(r.SR), SC(r.SC)
{
	image = r.image;
	pos = r.pos;
	frame = r.frame;
	animeFrameMax = r.animeFrameMax;
	animeRoopTime = r.animeRoopTime;
	offsetBlockR = r.offsetBlockR;
	offsetBlockC = r.offsetBlockC;
	timer = r.timer;
	state = r.state;
	IsFinish = r.IsFinish;
}

BaseUI& BaseUI::operator =(const BaseUI& r)
{
	image = r.image;
	pos = r.pos;
	frame = r.frame;
	animeFrameMax = r.animeFrameMax;
	animeRoopTime = r.animeRoopTime;
	offsetBlockR = r.offsetBlockR;
	offsetBlockC = r.offsetBlockC;
	timer = r.timer;
	state = r.state;
	IsFinish = r.IsFinish;

	return *this;
}

BaseUI::~BaseUI()
{

}

//----------- public class ------------

void BaseUI::Update()
{
	offsetBlockR = frame / animeRoopTime % SR;
	offsetBlockC = frame / animeRoopTime / SR;

	frame++;
	if (frame > animeRoopTime*animeFrameMax)
		frame = 0;
}

void BaseUI::Render()
{
	if (image == nullptr)return;

	image->RenderCC((int)pos.x, (int)pos.y, W, H,
					SX + offsetBlockR*SW, SY + offsetBlockC*SH, SW, SH,
					0);
}

void BaseUI::SetImage(tdn2DObjEx* image)
{
	this->image = image;
}

//**************************************
//  UI_ classes
//**************************************

//--------- UI_Time class ---------------

//--------- constructor,destructor -----------

UI_Time::UI_Time() :
NumberUI(34, 34),
LABEL_W(132), LABEL_H(34),
LABEL_SX(0), LABEL_SY(0), LABEL_SW(132), LABEL_SH(34)
{
	trueNum = 0;
	addNum = 0;
}

UI_Time::~UI_Time()
{

}

//--------- public method ------------

void UI_Time::Init()
{
	trueNum = 0;
	addNum = 0;
}

void UI_Time::Update()
{
	if (num != trueNum)
	{
		num += addNum;
		if ((addNum > 0 && num > trueNum)||
			(addNum < 0 && num < trueNum))
			num = trueNum;
	}
}

void UI_Time::Render(tdn2DObj* label, tdn2DObj* number, tdn2DObj* unit, Vector2 pos)
{
	label->Render((int)pos.x, (int)pos.y, LABEL_W, LABEL_H,
				  LABEL_SX, LABEL_SY, LABEL_SW, LABEL_SH);
	Vector2 p = pos;
	p.x += LABEL_W-40;
	Render_Left(number, unit, p);
}

void UI_Time::SetNum(int value)
{
	trueNum = value;
	addNum = trueNum > num ? trueNum - num : num - trueNum;
	if (trueNum != num&&addNum < 30)
		addNum = addNum>0 ? 1 : -1;
	else
		addNum /= 30;
}

//-------- UI_Score class ----------

//--------- constructor,destructor -----------

UI_Score::UI_Score():
NumberUI(51, 51)
{
	trueNum = 0;
	addNum = 0;
}

UI_Score::~UI_Score()
{

}

//--------- public method ------------

void UI_Score::Init()
{
	trueNum = 0;
	addNum = 0;
}

void UI_Score::Update()
{
	if (num != trueNum)
	{
		num += addNum;
		if ((addNum > 0 && num > trueNum)||
			(addNum < 0 && num < trueNum))
			num = trueNum;
	}
}

void UI_Score::Render(tdn2DObj* number,tdn2DObj* unit, Vector2 pos)
{
	Render_Right(number, unit, pos);
}

void UI_Score::SetNum(int value)
{
	trueNum = value;
	addNum = trueNum > num ? trueNum - num : num - trueNum;
	if (trueNum != num&&addNum < 30)
		addNum = addNum>0 ? 1 : -1;
	else
		addNum /= 30;
}

//-------- UI_LockCount class ----------

//--------- constructor,destructor -----------

UI_LockCount::UI_LockCount(Vector2& pos) :
NumberUI(0, 0),
BAL_W(147),BAL_H(102),BAL_SX(0),BAL_SY(0),BAL_SW(147),BAL_SH(102),
SCALE_SPEED(0.05f)
{
	this->pos = pos;
	scale = 0.05f;
	state = FADE_IN;
	timer = 0;
	IsFinish = false;
}

UI_LockCount::~UI_LockCount()
{

}

//-------- public method ---------

void UI_LockCount::Update()
{
	switch (state)
	{
		case UI_LockCount::NONE:
			break;
		case UI_LockCount::FADE_IN:
			scale += SCALE_SPEED;
			if (scale >= 1.0f)
			{
				scale = 1.0f;
				state = BalloonState::COUNT;
			}
			break;
		case UI_LockCount::FADE_OUT:
			scale -= SCALE_SPEED;
			if (scale <= 0.0f)
			{
				scale = 0.0f;
				state = BalloonState::NONE;
				IsFinish = true;
			}
			break;
		case WAIT:
			timer++;
			if (timer > 40)
				state = BalloonState::FADE_OUT;
			break;
		case COUNT:
			if (num <= 0)
				state = BalloonState::WAIT;
			break;
	}
}

void UI_LockCount::FadeIn()
{
	state = BalloonState::FADE_IN;
}

void UI_LockCount::FadeOut()
{
	state = BalloonState::FADE_OUT;
}

void UI_LockCount::Render(tdn2DObj* number, tdn2DObjEx* balloon)
{
	balloon->RenderCC((int)pos.x, (int)pos.y, (int)(BAL_W*scale), (int)(BAL_H*scale),
					  BAL_SX, BAL_SY, BAL_SW, BAL_SH,
					  0);
	if (scale <= 0.8f)return;
	Render_Right(number, nullptr, Vector2((float)(pos.x + BAL_W / 4), (float)(pos.y - BAL_H / 4)));
}
//-------- UI_DifPop class ----------

//--------- constructor,destructor -----------

UI_DifPop::UI_DifPop(tdn2DObjEx* image, Vector2& pos, UI_DifPop::PopType type, UI_DifPop::PopNum num) :
BaseUI(image, pos, 128, 128, 128 * (int)num, 128 * (int)type, 128, 128, 1, 1, 1, 1)
{
	alpha = 0xFF;
	moveY = 10;
}

UI_DifPop::UI_DifPop(const UI_DifPop& r) :
BaseUI(r.image, Vector2(r.pos), r.W, r.H, r.SX, r.SY, r.SW, r.SH, r.SR, r.SC, r.animeFrameMax, r.animeRoopTime)
{
	alpha = r.alpha;
	moveY = r.moveY;
}

UI_DifPop& UI_DifPop::operator =(const UI_DifPop& r)
{
	alpha = r.alpha;
	moveY = r.moveY;
	return *this;
}

//------------ public method -----------

void UI_DifPop::Update()
{
	switch (state)
	{
		case 1:
			// 透明化
			alpha -= 0x04;
			if (alpha <= 0x00)
			{
				alpha = 0x00;
				IsFinish = true;
				state++;
			}
		case 0:
			// 移動
			pos.y -= (float)moveY--;
			if (moveY < 0)
				moveY = 0;
			if (state <= 0 && timer>15)
			{
				state++;
			}
			else
				timer++;
			break;
		case 2:
			break;
	}
}

void UI_DifPop::Render()
{
	if (image == nullptr)return;

	image->RenderCC((int)pos.x, (int)pos.y, W, H, SX, SY, SW, SH,
					0, 0UL, ARGB(alpha, 255, 255, 255));
	//image->RenderCC((int)pos.x, (int)pos.y, W, H, SX, SY, SW, SH,
	//				0);
}

//-------- UI_Arrow class ----------

//--------- constructor,destructor -----------

UI_Arrow::UI_Arrow(tdn2DObjEx* image, Vector2& pos) :
BaseUI(image, pos, 128, 128, 0, 0, 128, 128, 4, 2, 7, 7),
FADE_SPEED(0x08)
{
	state = AlphaState::NONE;
	alpha = 0x7F;
}

UI_Arrow::UI_Arrow(const UI_Arrow& r):
BaseUI(r.image, Vector2(r.pos), r.W, r.H, r.SX, r.SY, r.SW, r.SH, r.SR, r.SC, r.animeFrameMax, r.animeRoopTime),
FADE_SPEED(r.FADE_SPEED)
{
	alpha = r.alpha;
}

UI_Arrow& UI_Arrow::operator =(const UI_Arrow& r)
{
	alpha = r.alpha;
	return *this;
}

void UI_Arrow::Update()
{
	BaseUI::Update();
	switch (state)
	{
		case AlphaState::NONE:
			break;
		case AlphaState::FADE_IN:
			alpha += FADE_SPEED;
			if (alpha >= 0xFF)
			{
				alpha = 0xFF;
				state = AlphaState::NONE;
			}
			break;
		case AlphaState::FADE_OUT:
			alpha -= FADE_SPEED;
			if (alpha <= 0x7F)
			{
				alpha = 0x7F;
				state = AlphaState::NONE;
			}
			break;
	}
}

void UI_Arrow::Render()
{
	if (image == nullptr)return;

	image->RenderCC((int)pos.x, (int)pos.y, W, H,
					SX + offsetBlockR*SW, SY + offsetBlockC*SH, SW, SH,
					0, RS::COPY, ARGB(alpha, 0xFF, 0xFF, 0xFF));
}

void UI_Arrow::FadeIn()
{
	state = AlphaState::FADE_IN;
}

void UI_Arrow::FadeOut()
{
	state = AlphaState::FADE_OUT;
}

//-------- UI_Warning class ----------

//--------- constructor,destructor -----------

UI_Warning::UI_Warning(tdn2DObjEx* image, Vector2& pos, bool IsRedColor, bool IsRight) :
BaseUI(image, pos, 128, 128, IsRedColor ? 64 : 0, IsRight ? 0 : 64, 64, 64, 1, 1, 1, 1),
FLASH_FRAME(IsRedColor ? 16 : 32)
{
	timer = IsRedColor ? 160 : 190;
	renderFlg = true;
}

UI_Warning::UI_Warning(const UI_Warning& r) :
BaseUI(r.image, Vector2(r.pos), r.W, r.H, r.SX, r.SY, r.SW, r.SH, r.SR, r.SC, r.animeFrameMax, r.animeRoopTime),
FLASH_FRAME(r.FLASH_FRAME)
{
	renderFlg = r.renderFlg;
}

UI_Warning& UI_Warning::operator =(const UI_Warning& r)
{
	renderFlg = r.renderFlg;
	return *this;
}

void UI_Warning::Update()
{
	switch (state)
	{
		case 0:
			if (frame++ > FLASH_FRAME)
			{
				frame = 0;
				renderFlg = renderFlg ? false : true;
			}
			if (timer-- <= 0)
			{
				IsFinish = true;
				state++;
			}
			break;
	}
}

void UI_Warning::Render()
{
	if (image == nullptr)return;
	if (!renderFlg)return;

	image->RenderCC((int)pos.x, (int)pos.y, W, H,
					SX, SY, SW, SH,
					0);

}

//-------- UI_SpeedUp class ----------

//--------- constructor,destructor -----------

UI_SpeedUp::UI_SpeedUp(tdn2DObjEx* label, tdn2DObjEx* arrow) :
BaseUI(label, Vector2(0, 0), 423 / 3, 104 / 3, 0, 0, 423, 104, 1, 1, 1, 1),
A_W(85 / 3), A_H(115 / 3), A_SX(0), A_SY(0), A_SW(85), A_SH(115),
FLASH_FRAME(4)
{
	timer = 120;
	imageArrow = arrow;
	renderFlg = true;
}

UI_SpeedUp::UI_SpeedUp(const UI_SpeedUp& r):
BaseUI(r.image, Vector2(r.pos), r.W, r.H, r.SX, r.SY, r.SW, r.SH, r.SR, r.SC, r.animeFrameMax, r.animeRoopTime),
A_W(r.A_W), A_H(r.A_H), A_SX(r.A_SX), A_SY(r.A_SY), A_SW(r.A_SW), A_SH(r.A_SH),
FLASH_FRAME(r.FLASH_FRAME)
{
	imageArrow = r.imageArrow;
	renderFlg = r.renderFlg;
}

UI_SpeedUp& UI_SpeedUp::operator =(const UI_SpeedUp& r)
{
	imageArrow = r.imageArrow;
	renderFlg = r.renderFlg;
	return *this;
}

//------- public method --------------

void UI_SpeedUp::Update()
{
	switch (state)
	{
		case 0:
			if (frame++ > FLASH_FRAME)
			{
				frame = 0;
				renderFlg = renderFlg ? false : true;
			}
			if (timer-- <= 0)
			{
				IsFinish = true;
				state++;
			}
			break;
	}
}

void UI_SpeedUp::Render(Vector2& pos)
{
	if (image == nullptr || imageArrow == nullptr)return;
	image->Render((int)pos.x, (int)pos.y, W, H,
					SX, SY, SW, SH,
					0);
	if (!renderFlg)return;

	imageArrow->Render((int)pos.x + W + 5, (int)pos.y - 3, A_W, A_H,
					A_SX, A_SY, A_SW, A_SH,
					0);
}

//-------- UI_Mouse class ----------

//--------- constructor,destructor -----------

UI_Mouse::UI_Mouse(tdn2DObjEx* image, Vector2& pos) :
BaseUI(image, pos, 128, 128, 0, 0, 128, 128, 1, 1, 1, 1),
FLASH_FRAME(30), ALPHA_SPEED(0x04)
{
	timer = 30 * 60;
	alpha = 0xFF;
	renderFlg = true;
}

UI_Mouse::UI_Mouse(const UI_Mouse& r) :
BaseUI(r.image, Vector2(r.pos), r.W, r.H, r.SX, r.SY, r.SW, r.SH, r.SR, r.SC, r.animeFrameMax, r.animeRoopTime),
FLASH_FRAME(r.FLASH_FRAME), ALPHA_SPEED(r.ALPHA_SPEED)
{
	alpha = r.alpha;
	renderFlg = r.renderFlg;
}

UI_Mouse& UI_Mouse::operator =(const UI_Mouse& r)
{
	alpha = r.alpha;
	renderFlg = r.renderFlg;
	return *this;
}

void UI_Mouse::Update()
{
	switch (state)
	{
		case 0:
			if (frame++ > FLASH_FRAME)
			{
				frame = 0;
				renderFlg = renderFlg ? false : true;
			}
			if (timer-- <= 0)
			{
				state++;
			}
			break;
		case 1:
			alpha -= ALPHA_SPEED;
			if (alpha <= 0x00)
			{
				alpha = 0x00;
				IsFinish = true;
				state++;
			}
			break;
	}
}

void UI_Mouse::Render()
{
	if (image == nullptr)return;

	image->Render((int)pos.x, (int)pos.y, W, H,
				  renderFlg ? SX : SX + SW, SY, SW, SH,
				  0, RS::COPY, ARGB(alpha, 0xFF, 0xFF, 0xFF));

}
