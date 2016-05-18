#pragma once
#include	"TDNLIB.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//		IEX�g���N���X
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class tdn2DObjEx	: public tdn2DObj
{
public:
	tdn2DObjEx(char* filename) : tdn2DObj(filename){} //���N���X�Ƀf�[�^�������n���Ă���
	~tdn2DObjEx(){}

	//��]���W���O�K�p
	void RollPosCC(s32 DstX, s32 DstY, s32 DstW, s32 DstH, float roll);
	void Render(s32 SrcX, s32 SrcY, s32 width, s32 height, u32 dwFlags = NULL, COLOR color = 0xFFFFFFFF, float z = .0f);

	//���S�_���w�肵�ĕ`�悷��
	//OverCenter�i�㒆��
	void RenderOC(s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, float roll, u32 dwFlags = NULL, COLOR color = 0xFFFFFFFF, float z = .0f);
	//CenterCenter�i���S
	void RenderCC(s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, float roll, u32 dwFlags = NULL, COLOR color = 0xFFFFFFFF, float z = .0f);
	//UnderCenter�i������
	void RenderUC(s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, float roll, u32 dwFlags = NULL, COLOR color = 0xFFFFFFFF, float z = .0f);
	//��{�i����
	void Render( s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height,float roll, u32 dwFlags = NULL, COLOR color = 0xFFFFFFFF, float z = .0f);

private:
	TLVERTEX	cv[4];

public:
	void GetPos(int id, int& x, int& y){
		x = (int)cv[id].sx; y = (int)cv[id].sy;
	}
};