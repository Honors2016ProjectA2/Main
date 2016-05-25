
#include "TDNLIB.h"
#include "particle_2d.h"

//*****************************************************************************
//		セット
//*****************************************************************************
void Particle_2d_data::Set(LPPARTICLE_2d pd)
{
	memcpy(&pdata, pd, sizeof(PARTICLE_2d));
	CurFrame = 0;
	CurAlpha = 0;
	bFlags = 1;
}

//*****************************************************************************
//		更新
//*****************************************************************************
void Particle_2d_data::Update()
{
	float	work;
	int	a1, r1, g1, b1, a2, r2, g2, b2;

	if (bFlags == 0) return;

	//	情報更新
	pdata.Pos_x += pdata.Move_x;
	pdata.Pos_y += pdata.Move_y;
	pdata.Move_x+= pdata.Power_x;
	pdata.Move_y += pdata.Power_y;
	pdata.angle += pdata.rotate;
	pdata.scale *= pdata.stretch;

	//	カラー計算
	if (CurFrame < pdata.aFrame) CurAlpha = .0f;
	else {
		if (CurFrame < pdata.mFrame){
			work = (CurFrame - pdata.aFrame) / (float)(pdata.mFrame - pdata.aFrame);
			a1 = (pdata.aColor >> 24);
			r1 = ((pdata.aColor >> 16) & 0xFF);
			g1 = ((pdata.aColor >> 8) & 0xFF);
			b1 = ((pdata.aColor) & 0xFF);
		}
		else {
			work = (pdata.eFrame - CurFrame) / (float)(pdata.eFrame - pdata.mFrame);
			a1 = (pdata.eColor >> 24);
			r1 = ((pdata.eColor >> 16) & 0xFF);
			g1 = ((pdata.eColor >> 8) & 0xFF);
			b1 = ((pdata.eColor) & 0xFF);
		}

		a2 = (pdata.mColor >> 24);
		r2 = ((pdata.mColor >> 16) & 0xFF);
		g2 = ((pdata.mColor >> 8) & 0xFF);
		b2 = ((pdata.mColor) & 0xFF);
		//	カラー設定
		CurAlpha = ((a2 - a1)*work + a1) / 255.0f;
		r = ((r2 - r1)*work + r1) / 255.0f;
		g = ((g2 - g1)*work + g1) / 255.0f;
		b = ((b2 - b1)*work + b1) / 255.0f;
	}

	//	フレーム進行
	CurFrame++;
	if (CurFrame >= pdata.eFrame) bFlags = 0;
}

//*****************************************************************************
//		描画
//*****************************************************************************
//------------------------------------------------------
//		ポリゴン生成
//------------------------------------------------------
inline bool Particle_2d_data::SetVertex(TLVERTEX* v, int uindex, int vindex)
{
	bool bRender = false;

	bRender = true;

	for (int i = 0; i < 4; i++)
	{
		v[i].sz = 0;
		v[i].rhw = 1.0f;
	}

	//v[0].sx = -32;
	//v[0].sy = -32;

	//v[1].sx = 32;
	//v[1].sy = -32;

	//v[2].sx = -32;
	//v[2].sy = 32;

	//v[3].sx = 32;
	//v[3].sy = 32;

	
	
	float scale_2 = pdata.scale * 0.5f;

	v[0].sx = pdata.Pos_x - scale_2;
	v[0].sy = pdata.Pos_y - scale_2;

	v[1].sx = pdata.Pos_x + scale_2;
	v[1].sy = pdata.Pos_y - scale_2;

	v[2].sx = pdata.Pos_x - scale_2;
	v[2].sy = pdata.Pos_y + scale_2;

	v[3].sx = pdata.Pos_x + scale_2;
	v[3].sy = pdata.Pos_y + scale_2;

	

	if (bRender == false) return false;
	//	タイプ設定
	v[0].tu = v[2].tu = (float)(pdata.type % uindex) * (1.0f / uindex) + 0.001f;
	v[1].tu = v[3].tu = v[0].tu + (1.0f / uindex);
	v[0].tv = v[1].tv = (float)(pdata.type / vindex) * (1.0f / vindex) + 0.001f;
	v[3].tv = v[2].tv = v[0].tv + (1.0f / vindex);

	//	色設定
	v[0].color = v[1].color = v[2].color = v[3].color = ((DWORD)(CurAlpha*255.0f) << 24) | ((DWORD)(r*255.0f) << 16) | ((DWORD)(g*255.0f) << 8) | ((DWORD)(b*255.0f));

	return true;
}

//------------------------------------------------------
//		固定機能描画
//------------------------------------------------------
void	Particle_2d_data::Render(int uindex, int vindex)
{
	TLVERTEX	v[4];

	if (SetVertex(v, uindex, vindex) == false) return;
	//	合成設定
	if (pdata.flag == RS::SUB) tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	else                      tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//	レンダリング
	
	tdnSystem::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(TLVERTEX));
}


// 移動
void Particle_2d_data::Move_pos(float x, float y)
{
	pdata.Pos_x += x;
	pdata.Pos_y += y;
}

///
//
///

void Particle_2d::Initialize(char* filename, int nParticles, int uindex, int vindex)
{
	this->uindex = (uindex <= 0) ? 1 : uindex;
	this->vindex = (vindex <= 0) ? 1 : vindex;

	//	パーティクルバッファ確保
	this->nParticles = nParticles;
	this->ParticleData = new Particle_2d_data[nParticles];
	//	全リセット
	Reset();
	//	テクスチャ設定
	this->lpTexture = new tdn2DObj(filename);
}

//------------------------------------------------------
//		解放
//------------------------------------------------------
void Particle_2d::Release()
{
	delete[] ParticleData;
	ParticleData = nullptr;
	delete lpTexture;
	lpTexture = nullptr;
}

//*****************************************************************************
//		データ設定
//*****************************************************************************
//------------------------------------------------------
//		構造体指定
//------------------------------------------------------
void Particle_2d::Set(LPPARTICLE_2d pd)
{
	for (int i = 0; i<nParticles; i++){
		if (ParticleData[i].bActive()) continue;
		ParticleData[i].Set(pd);
		break;
	}
}

//------------------------------------------------------
//		データ個別指定
//------------------------------------------------------
void Particle_2d::Set(int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor,
	float Pos_x, float Pos_y, float Move_x, float Move_y, float Power_x, float Power_y, float rotate, float stretch, float scale, u8 flag)
{
	PARTICLE_2d pd;

	pd.type = type;
	pd.aFrame = aFrame;
	pd.aColor = aColor;
	pd.eFrame = eFrame;
	pd.eColor = eColor;
	pd.mFrame = mFrame;
	pd.mColor = mColor;

	pd.Pos_x = Pos_x;
	pd.Pos_y = Pos_y;
	pd.Move_x = Move_x;
	pd.Move_y = Move_y;
	pd.Power_x = Power_x;
	pd.Power_y = Power_y;
	pd.rotate = rotate;
	pd.stretch = stretch;

	pd.scale = scale;
	pd.angle = .0f;

	pd.flag = flag;

	Set(&pd);
}

//------------------------------------------------------
//		データ個別指定
//------------------------------------------------------
void	Particle_2d::Set(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha,
	float Pos_x, float Pos_y, float Move_x, float Move_y, float Power_x, float Power_y,
	float r, float g, float b, float scale, u8 flag)
{
	COLOR	color;
	DWORD	aa, ea, ma;

	aa = ((DWORD)(aAlpha*255.0f) << 24);
	ea = ((DWORD)(eAlpha*255.0f) << 24);
	ma = ((DWORD)(mAlpha*255.0f) << 24);
	color = ((DWORD)(r*255.0f) << 16) | ((DWORD)(g*255.0f) << 8) | (DWORD)(b*255.0f);
	Set(type, aFrame, aa | color, eFrame, ea | color, mFrame, ma | color, Pos_x, Pos_y, Move_x, Move_y, Power_x, Power_y, .0f, 1.0f, scale, flag);
}

//*****************************************************************************
//		更新
//*****************************************************************************
void Particle_2d::Update()
{
	for (int i = 0; i<nParticles; i++){
		if (ParticleData[i].bActive() == FALSE) continue;
		ParticleData[i].Update();
	}
}

//*****************************************************************************
//		リセット
//*****************************************************************************
void Particle_2d::Reset()
{
	for (int i = 0; i < nParticles; i++)
	{
		if (ParticleData[i].bActive() == FALSE) continue;
		ParticleData[i].SetFlag(0);
	}
}
void Particle_2d::Reset(int num)
{
	nParticles = num;
	delete[] ParticleData;
	ParticleData = new Particle_2d_data[nParticles];

	Reset();
}

//*****************************************************************************
//		描画
//*****************************************************************************
//------------------------------------------------------
//		固定機能描画
//------------------------------------------------------
void Particle_2d::Render()
{
	int		i;
	float	scale = -1.0f;

	tdnRenderState::Set(RS::ADD, NULL, lpTexture->GetTexture());
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	tdnSystem::GetDevice()->SetFVF(D3DFVF_TLVERTEX);
	//	アルファブレンド設定
	tdnSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	tdnSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	tdnSystem::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	for (i = 0; i<nParticles; i++){
		//	パーティクルレンダリング
		if (ParticleData[i].bActive()) ParticleData[i].Render(uindex, vindex);
	}

	tdnSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	tdnSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}



// パーティクル全体を移動
void Particle_2d::Move_pos(float x, float y)
{
	for (int i = 0; i<nParticles; i++){
		if (ParticleData[i].bActive() == FALSE) continue;
		ParticleData[i].Move_pos(x, y);
	}
}
