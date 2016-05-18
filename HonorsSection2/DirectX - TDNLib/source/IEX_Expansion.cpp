#include	"IEX_Expansion.h"

//*****************************************************************************************************************************
//
//		2D関連（tdn3DObj拡張クラス
//
//*****************************************************************************************************************************

void tdn2DObjEx::RollPosCC(s32 DstX, s32 DstY, s32 DstW, s32 DstH, float roll)
{
	TLVERTEX	v[4];

	float cx,cy;
	cx = (float)DstX;
	cy = (float)DstY;

	v[0].sx = v[2].sx = cx-DstW*0.5f;
	v[1].sx = v[3].sx = cx+DstW*0.5f -0.5f;
	v[0].sy = v[1].sy = cy-DstH*0.5f;
	v[2].sy = v[3].sy = cy+DstH*0.5f -0.5f;

	for(int i = 0;i < 4;i++){
		cv[i].sx = (v[i].sx - cx) * cosf(roll) - (v[i].sy - cy) * sinf(roll) + cx;
		cv[i].sy = (v[i].sx - cx) * sinf(roll) + (v[i].sy - cy) * cosf(roll) + cy;
	}
}

void tdn2DObjEx::Render(s32 SrcX, s32 SrcY, s32 width, s32 height, u32 dwFlags, COLOR color, float z)
{
	cv[0].tu = cv[2].tu = (float)(SrcX+0.5f)/(float)this->m_width;
	cv[1].tu = cv[3].tu = (float)(SrcX + width) / (float)this->m_width;
	cv[0].tv = cv[1].tv = (float)(SrcY + 0.5f) / (float)this->m_height;
	cv[2].tv = cv[3].tv = (float)(SrcY + height) / (float)this->m_height;

	cv[0].color = cv[1].color = cv[2].color = cv[3].color = color;
	cv[0].sz    = cv[1].sz    = cv[2].sz    = cv[3].sz    = z;
	cv[0].rhw   = cv[1].rhw   = cv[2].rhw   = cv[3].rhw   = 1.0f;

	tdnPolygon::Render2D( cv, 2, this, dwFlags );
}

void tdn2DObjEx::RenderOC( s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, float roll, u32 dwFlags, COLOR color, float z )
{

	TLVERTEX	v[4];

	//原点設定
	float cx = (float)DstX;
	float cy = (float)DstY;

	//頂点設定
	v[0].sx = v[2].sx = cx-DstW*0.5f;
	v[1].sx = v[3].sx = cx+DstW*0.5f -0.5f;
	v[0].sy = v[1].sy = cy;
	v[2].sy = v[3].sy = cy+DstH -0.5f;

	////頂点設定
	//v[0].sx = v[2].sx = (float)DstX-DstW*0.5f;
	//v[1].sx = v[3].sx = (float)(DstX+DstW*0.5f)-0.5f;
	//v[0].sy = v[1].sy = (float)DstY;
	//v[2].sy = v[3].sy = (float)(DstY+DstH)-0.5f;

	//回転処理
	for(int i = 0;i < 4;i++){
		cv[i].sx = (v[i].sx - cx) * cosf(roll) - (v[i].sy - cy) * sinf(roll) + cx;
		cv[i].sy = (v[i].sx - cx) * sinf(roll) + (v[i].sy - cy) * cosf(roll) + cy;

	}

	//テクスチャ
	cv[0].tu = cv[2].tu = (float)(SrcX + 0.5f) / (float)this->m_width;
	cv[1].tu = cv[3].tu = (float)(SrcX + width) / (float)this->m_width;
	cv[0].tv = cv[1].tv = (float)(SrcY + 0.5f) / (float)this->m_height;
	cv[2].tv = cv[3].tv = (float)(SrcY + height) / (float)this->m_height;

	//その他
	cv[0].color = cv[1].color = cv[2].color = cv[3].color = color;
	cv[0].sz    = cv[1].sz    = cv[2].sz    = cv[3].sz    = z;
	cv[0].rhw   = cv[1].rhw   = cv[2].rhw   = cv[3].rhw   = 1.0f;

	tdnPolygon::Render2D( cv, 2, this, dwFlags );
}

void tdn2DObjEx::RenderCC( s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, float roll, u32 dwFlags, COLOR color, float z )
{
	TLVERTEX	v[4];

	float cx,cy;
	cx = (float)DstX;
	cy = (float)DstY;

	v[0].sx = v[2].sx = cx-DstW*0.5f;
	v[1].sx = v[3].sx = cx+DstW*0.5f -0.5f;
	v[0].sy = v[1].sy = cy-DstH*0.5f;
	v[2].sy = v[3].sy = cy+DstH*0.5f -0.5f;

	for(int i = 0;i < 4;i++){
		cv[i].sx = (v[i].sx - cx) * cosf(roll) - (v[i].sy - cy) * sinf(roll) + cx;
		cv[i].sy = (v[i].sx - cx) * sinf(roll) + (v[i].sy - cy) * cosf(roll) + cy;
	}

	cv[0].tu = cv[2].tu = (float)(SrcX + 0.5f) / (float)this->m_width;
	cv[1].tu = cv[3].tu = (float)(SrcX + width) / (float)this->m_width;
	cv[0].tv = cv[1].tv = (float)(SrcY + 0.5f) / (float)this->m_height;
	cv[2].tv = cv[3].tv = (float)(SrcY + height) / (float)this->m_height;

	cv[0].color = cv[1].color = cv[2].color = cv[3].color = color;
	cv[0].sz    = cv[1].sz    = cv[2].sz    = cv[3].sz    = z;
	cv[0].rhw   = cv[1].rhw   = cv[2].rhw   = cv[3].rhw   = 1.0f;

	tdnPolygon::Render2D( cv, 2, this, dwFlags );
}

void tdn2DObjEx::RenderUC( s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height,float roll, u32 dwFlags, COLOR color, float z )
{
	TLVERTEX	v[4];

	float cx,cy;
	cx = (float)DstX;
	cy = (float)DstY;

	v[0].sx = v[2].sx = cx-DstW*0.5f;
	v[1].sx = v[3].sx = cx+DstW*0.5f -0.5f;
	v[0].sy = v[1].sy = cy-DstH;
	v[2].sy = v[3].sy = cy -0.5f;

	for(int i = 0;i < 4;i++){
		cv[i].sx = (v[i].sx - cx) * cosf(roll) - (v[i].sy - cy) * sinf(roll) + cx;
		cv[i].sy = (v[i].sx - cx) * sinf(roll) + (v[i].sy - cy) * cosf(roll) + cy;

	}

	cv[0].tu = cv[2].tu = (float)(SrcX + 0.5f) / (float)this->m_width;
	cv[1].tu = cv[3].tu = (float)(SrcX + width) / (float)this->m_width;
	cv[0].tv = cv[1].tv = (float)(SrcY + 0.5f) / (float)this->m_height;
	cv[2].tv = cv[3].tv = (float)(SrcY + height) / (float)this->m_height;

	cv[0].color = cv[1].color = cv[2].color = cv[3].color = color;
	cv[0].sz    = cv[1].sz    = cv[2].sz    = cv[3].sz    = z;
	cv[0].rhw   = cv[1].rhw   = cv[2].rhw   = cv[3].rhw   = 1.0f;

	tdnPolygon::Render2D( cv, 2, this, dwFlags );
}

void tdn2DObjEx::Render( s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height,float roll, u32 dwFlags, COLOR color, float z )
{

	TLVERTEX	v[4];

	float cx,cy;
	cx = (float)DstX + (float)width / 2;
	cy = (float)DstY + (float)height / 2;

	v[0].sx = v[2].sx = (float)DstX;
	v[1].sx = v[3].sx = (float)(DstX+DstW)-0.5f;
	v[0].sy = v[1].sy = (float)DstY;
	v[2].sy = v[3].sy = (float)(DstY+DstH)-0.5f;

	for(int i = 0;i < 4;i++){
		cv[i].sx = (v[i].sx - cx) * cosf(roll) - (v[i].sy - cy) * sinf(roll) + cx;
		cv[i].sy = (v[i].sx - cx) * sinf(roll) + (v[i].sy - cy) * cosf(roll) + cy;

	}

	cv[0].tu = cv[2].tu = (float)(SrcX+0.5f)/(float)this->m_width;
	cv[1].tu = cv[3].tu = (float)(SrcX+width)/(float)this->m_width;
	cv[0].tv = cv[1].tv = (float)(SrcY+0.5f)/(float)this->m_height;
	cv[2].tv = cv[3].tv = (float)(SrcY+height)/(float)this->m_height;

	cv[0].color = cv[1].color = cv[2].color = cv[3].color = color;
	cv[0].sz    = cv[1].sz    = cv[2].sz    = cv[3].sz    = z;
	cv[0].rhw   = cv[1].rhw   = cv[2].rhw   = cv[3].rhw   = 1.0f;

	tdnPolygon::Render2D( cv, 2, this, dwFlags );
}