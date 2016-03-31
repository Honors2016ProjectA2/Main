#ifndef FADE_H_
#define	FADE_H_

//=======================================================================
//						�t�F�[�h�N���X
//=======================================================================


class Fade
{
public:
	//------------------------------------------------------
	//	�萔
	//------------------------------------------------------
	enum FLAG{ FADE_NONE, FADE_IN, FADE_OUT, MAX };

	//------------------------------------------------------
	//	��{�p�����[�^
	//------------------------------------------------------
	static FLAG mode;				/*	�t�F�[�h�C���A�t�F�[�h�A�E�g�A�������Ȃ�	*/
	static unsigned char speed;		/*	�t�F�[�h���鑬�x							*/
	static unsigned char limit;		/*	�ǂ��܂Ńt�F�[�h���邩(0�`255�͈̔�)		*/
	static unsigned char alpha;		/*	���l(0�`255)								*/
	static unsigned long color;		/*	�J���[(0x00000000 �` 0x00ffffff)			*/

	//void None();
	//void In();
	//void Out();
	//static void(Fade::*Fade_mode_funk[FLAG::MAX])();

	//------------------------------------------------------
	//	������
	//------------------------------------------------------
	static void Initialize();


	//------------------------------------------------------
	//	�ݒ�
	//------------------------------------------------------
	static void Set(FLAG Mode, unsigned char Speed, unsigned long Color, unsigned char StartAlpha, unsigned char Limit);		/*	�t���ݒ��			*/
	static void Set(FLAG Mode, unsigned char Speed, unsigned long Color);														/*	�F�ݒ�ł����		*/
	static void Set(FLAG Mode, unsigned char Speed);																			/*	�ȈՔ�				*/


	//------------------------------------------------------
	//	�X�V�ƕ`��
	//------------------------------------------------------
	static void Update();
	static void Render();

	//------------------------------------------------------
	//	Get_Set
	//------------------------------------------------------
	static bool isFadeStop(){ return (mode == FADE_NONE); }
};

#endif
