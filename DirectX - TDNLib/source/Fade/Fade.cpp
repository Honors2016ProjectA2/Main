#include	"TDNLIB.h"

#include	"Fade.h"

//*********************************************************************************************
//		�p�����[�^�̐ݒ�
//*********************************************************************************************
Fade::FLAG Fade::mode;		/*	�t�F�[�h�C���A�t�F�[�h�A�E�g�A�������Ȃ�	*/
unsigned char Fade::speed;	/*	�t�F�[�h���鑬�x	*/
unsigned char Fade::limit;	/*	�ǂ��܂Ńt�F�[�h���邩(0�`255�͈̔�)	*/
unsigned char Fade::alpha;	/*	���l(0�`255)	*/
unsigned long Fade::color;	/*	�J���[(0x����000000 �` 0x����ffffff)�����́����̕�����K��00�ɂ��Ă������ƁI	*/


//=============================================================================================
//		��	��	��
void Fade::Initialize()
{
	mode = FADE_NONE;
	speed = 1;
	alpha = 255;
	color = 0x00000000;
}
//
//=============================================================================================


//=============================================================================================
//		�t�F�[�h�ݒ�
void Fade::Set(FLAG Mode, unsigned char Speed, unsigned long Color, unsigned char StartAlpha, unsigned char Limit)
{
	mode = Mode;		// �t�F�[�h���[�h(FADE_IN or FADE_OUT)
	speed = Speed;		// �t�F�[�h�𑝌�������X�s�[�h(5�Ȃ�A���t���[���u5�v�����������)
	limit = Limit;		// �ڕWalpha�l(FADE_IN�Ȃ�u0�v�AFADE_OUT�Ȃ�u255�v����{)
	alpha = StartAlpha;	// �t�F�[�h�J�n����alpha�l(FADE_IN�Ȃ�u255�v�ADADE_OUT�Ȃ�u0�v����{)
	color = Color;		// �t�F�[�h�F

	// 0x����ffffff�́��̕�����0�ɂ��鏈��(alpha�l�̒���)
	color &= 0x00ffffff;
}


void Fade::Set(FLAG Mode, unsigned char Speed, unsigned long Color)
{
	mode = Mode;
	speed = Speed;
	color = Color;

	switch (mode)
	{
	case FADE_IN:
		alpha = 255;
		limit = 0;
		break;

	case FADE_OUT:
		alpha = 0;
		limit = 255;
		break;

	case FADE_NONE:
		break;
	}
}

void Fade::Set(FLAG Mode, unsigned char Speed)	// alpha��0�`255�͈̔�
{
	mode = Mode;
	speed = Speed;

	switch (mode)
	{
	case FADE_IN:
		limit = 0;
		break;

	case FADE_OUT:
		limit = 255;
		break;

	case FADE_NONE:
		break;
	}
}
//
//=============================================================================================



//=============================================================================================
//		�X		�V
void Fade::Update()
{
	switch (mode)
	{
	case FADE_IN:

		// �ړI���܂ł�����A�X�g�b�v
		if (alpha <= limit + speed){		// unsigned char�Ȃ̂ŃI�[�o�[�t���[�h�~
			alpha = limit;
			mode = FADE_NONE;
		}
		else
			alpha -= speed;

		break;

	case FADE_OUT:

		// �ړI���܂ł�����A�X�g�b�v
		if (alpha >= limit - speed){		// unsigned char�Ȃ̂ŃI�[�o�[�t���[�h�~
			alpha = limit;
			mode = FADE_NONE;
		}
		else
			alpha += speed;

		break;

	case FADE_NONE:
		break;	// NothingDo
	}
}


//=============================================================================================
//		�`			��
void Fade::Render()
{
	const unsigned long c = (alpha << 24) | color;	// argb�̃t�H�[�}�b�g�ɂ���

	tdnPolygon::Rect(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, RS::COPY, c);
}
//
//=============================================================================================