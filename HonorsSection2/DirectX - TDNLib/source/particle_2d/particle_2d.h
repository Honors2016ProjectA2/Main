
#ifndef PARTICLE_2D
#define PARTICLE_2D

typedef struct Tag_particle_2d
{
public:
	int		type;			//	�`

	int		aFrame;			//	�o���t���[��
	COLOR	aColor;			//	�o���J���[

	int		eFrame;			//	���Ńt���[��
	COLOR	eColor;			//	�o���J���[

	int		mFrame;			//	�ō��t���[��
	COLOR	mColor;			//	�ō��J���[

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

	//	�X�V
	void Update();

	//	�`��
	inline bool SetVertex(TLVERTEX* v, int uindex, int vindex);
	void Render(int uindex, int vindex);

	// �ړ�
	inline void Move_pos(float x, float y);
};



class Particle_2d
{
protected:
	int	nParticles;					//	�p�[�e�B�N���ő吔
	Particle_2d_data*	ParticleData;	//	�p�[�e�B�N���f�[�^
	tdn2DObj* lpTexture;				//	�e�N�X�`��

	int uindex, vindex; // �e�N�X�`���̕����� type�ō��ォ��E�ɐ�����
public:
	Matrix CurViewProjection;

	Particle_2d() :nParticles(0), ParticleData(nullptr), lpTexture(nullptr), uindex(1), vindex(1){}
	~Particle_2d(){ Release(); }

	//	�������E���
	void Initialize(char* filename, int nParticles, int uindex, int vindex);
	void Release();

	//	�o��
	void Set(LPPARTICLE_2d pd);
	void	Set(int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor,
		float Pos_x, float Pos_y, float Move_x, float Move_y, float Power_x, float Power_y, float rotate, float stretch, float scale, u8 flag);
	void	Set(int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha,
		float Pos_x, float Pos_y, float Move_x, float Move_y, float Power_x, float Power_y,
		float r, float g, float b, float scale, u8 flag);

	//	�X�V
	void Update();
	void Reset();
	void Reset(int num);

	//	�`��
	void Render();

	// �ړ�
	void Move_pos(float x, float y);
};

#endif // !PARTICLE_2D
