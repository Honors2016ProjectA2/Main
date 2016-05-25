
#ifndef PARTICLE_2D
#define PARTICLE_2D

typedef struct Tag_particle_2d
{
public:
	int		type;			//	形

	int		aFrame;			//	出現フレーム
	COLOR	aColor;			//	出現カラー

	int		eFrame;			//	消滅フレーム
	COLOR	eColor;			//	出現カラー

	int		mFrame;			//	最高フレーム
	COLOR	mColor;			//	最高カラー

	float		Pos_x, Pos_y;
	float		Move_x, Move_y;
	float		Power_x, Power_y;
	float		rotate;
	float		stretch;

	float	angle;
	float	scale;

	u8		flag;
} PARTICLE_2d, *LPPARTICLE_2d;



class Particle_2d_data
{
protected:
	u8		bFlags;
	float	r, g, b;
	PARTICLE_2d	pdata;
	int		CurFrame;
	float	CurAlpha;

public:

	inline BOOL bActive(){ return (bFlags != 0); }

	void Set(LPPARTICLE_2d pd);
	void SetFlag(u8 flag){ bFlags = flag; }

	//	更新
	void Update();

	//	描画
	inline bool SetVertex(TLVERTEX* v, int uindex, int vindex);
	void Render(int uindex, int vindex);

	// 移動
	inline void Move_pos(float x, float y);
};



class Particle_2d
{
protected:
	int	nParticles;					//	パーティクル最大数
	Particle_2d_data*	ParticleData;	//	パーティクルデータ
	tdn2DObj* lpTexture;				//	テクスチャ

	int uindex, vindex; // テクスチャの分割数 typeで左上から右に数える
public:
	Matrix CurViewProjection;

	Particle_2d() :nParticles(0), ParticleData(nullptr), lpTexture(nullptr), uindex(1), vindex(1){}
	~Particle_2d(){ Release(); }

	//	初期化・解放
	void Initialize(char* filename, int nParticles, int uindex, int vindex);
	void Release();

	//	出現
	void Set(LPPARTICLE_2d pd);
	void	Set(int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor,
		float Pos_x, float Pos_y, float Move_x, float Move_y, float Power_x, float Power_y, float rotate, float stretch, float scale, u8 flag);
	void	Set(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha,
		float Pos_x, float Pos_y, float Move_x, float Move_y, float Power_x, float Power_y,
		float r, float g, float b, float scale, u8 flag);

	//	更新
	void Update();
	void Reset();
	void Reset(int num);

	//	描画
	void Render();

	// 移動
	void Move_pos(float x, float y);
};

#endif // !PARTICLE_2D
