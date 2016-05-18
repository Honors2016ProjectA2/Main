#pragma once
#include	"TDNLIB.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//		IEX拡張クラス
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class tdn2DObjEx	: public tdn2DObj
{
public:
	tdn2DObjEx(char* filename) : tdn2DObj(filename){} //基底クラスにデータを引き渡している
	~tdn2DObjEx(){}

	//回転座標事前適用
	void RollPosCC(s32 DstX, s32 DstY, s32 DstW, s32 DstH, float roll);
	void Render(s32 SrcX, s32 SrcY, s32 width, s32 height, u32 dwFlags = NULL, COLOR color = 0xFFFFFFFF, float z = .0f);

	//中心点を指定して描画する
	//OverCenter（上中央
	void RenderOC(s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, float roll, u32 dwFlags = NULL, COLOR color = 0xFFFFFFFF, float z = .0f);
	//CenterCenter（中心
	void RenderCC(s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, float roll, u32 dwFlags = NULL, COLOR color = 0xFFFFFFFF, float z = .0f);
	//UnderCenter（下中央
	void RenderUC(s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, float roll, u32 dwFlags = NULL, COLOR color = 0xFFFFFFFF, float z = .0f);
	//基本（左上
	void Render( s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height,float roll, u32 dwFlags = NULL, COLOR color = 0xFFFFFFFF, float z = .0f);

private:
	TLVERTEX	cv[4];

public:
	void GetPos(int id, int& x, int& y){
		x = (int)cv[id].sx; y = (int)cv[id].sy;
	}
};