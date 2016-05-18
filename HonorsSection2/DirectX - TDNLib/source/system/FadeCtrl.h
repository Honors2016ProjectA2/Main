#pragma once
//*****************************************************************************************************************************
//
//		�t�F�[�h�C���t�F�[�h�A�E�g
//
//*****************************************************************************************************************************

class	FadeControl
{
private:
	static int	mode;		//	���[�h
	static float	count;		//	���[�h�p�J�E���^
	static float	param;	//	���[�h�p�p�����[�^
	static int	stop;		//	�����x�ݒ�p
	static bool	isFade;		//	�t�F�[�h�����ǂ���
	static bool isEndFade;	//	�t�F�[�h�I���t���[�����ǂ���
public:
	enum MODE{
		FADE_IN, WHITE_IN, FADE_OUT, WHITE_OUT
	};

	static void Setting( int mode, float speed, float stop = -1.0f, float start = -1.0f );
	static void Update();
	static void Render();
	static bool IsFade(){
		return isFade;
	}
	static bool IsEndFade(){
		return isEndFade;
	}
};