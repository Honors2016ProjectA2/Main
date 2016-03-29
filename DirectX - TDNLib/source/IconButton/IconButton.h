#pragma once

//*****************************************************************************************************************************
//
//		いつものボタン
//
//*****************************************************************************************************************************

enum class EN_DIS_TYPE
{
	ENABLE,				// 稼働中だよ
	DISABLE_BLACK,		// 使えない敵なメッセージ
	DISABLE_WHITE,		// 何か選択中的なメッセージ
};

class IconButton
{
public:
	// カーソルが範囲内に入ったときのアクション
	enum IN_ACTION
	{
		UP_SCALE = 0x1,		// スケールを少し上げる
		PUT_WHITE = 0x1 << 1	// 少し白を上乗せする
	};

	IconButton() :m_pCollision(nullptr), m_EnDisType(EN_DIS_TYPE::ENABLE){}
	void Initialize(int ID, char *TexPath, int dstX, int dstY, BYTE InAction, char* se_ID);
	~IconButton();
	void Update(const Vector2 &CursorPos);
	void Render();
	// 引数の座標が自分の範囲内であるかを返す
	//bool CheckPos(const Vector2 &CursorPos){ return(CursorPos.x >= m_dstX && CursorPos.x <= m_dstY&&CursorPos.y >= m_dstY && CursorPos.y <= m_dstH); }

	void SetEnable(EN_DIS_TYPE type)
	{
		m_EnDisType = type;
		m_In = false;
	}
	bool isIn(){ return m_In; }
	int GetID(){ return m_ID; }


	// 判定の設定(登録)
	void SetCollisionSqure(const Vector2 MinPos, const Vector2 MaxPos)
	{
		SAFE_DELETE(m_pCollision);
		m_pCollision = new Collision2D::Squre(MinPos, MaxPos);
	}
	void SetCollisionCircle(const Vector2 CenterPos, float Radius)
	{
		SAFE_DELETE(m_pCollision);
		m_pCollision = new Collision2D::Circle(CenterPos, Radius);
	}

private:
	int m_ID;				// 番号
	int m_dstX;				// 画像左座標
	int m_dstY;				// 画像上座標
	//int m_dstW;				// 画像右座標
	//int m_dstH;				// 画像下座標
	//int m_srcW;
	//int m_srcH;
	bool m_In;				// 範囲内
	BYTE m_InActionFlag;		//
	tdn2DObj *m_pButton;		// 画像実体
	char m_SE_ID[64];			// 範囲内に入った際のSEのID
	EN_DIS_TYPE m_EnDisType;	// 有効かどうか

	class Collision2D
	{
	public:
		class Base
		{
		public:
			virtual bool Collision(const Vector2 &pos) = 0;
			virtual void RenderDebug() = 0;
		};

		class Squre : public Base
		{
		private:
			Vector2 m_MinPos;	// 左上
			Vector2 m_MaxPos;	// 右下
			Squre();
		public:
			Squre(const Vector2 MinPos, const Vector2 MaxPos) :m_MinPos(MinPos), m_MaxPos(MaxPos){}
			bool Collision(const Vector2 &pos);
			void RenderDebug();
		};

		class Circle : public Base
		{
		private:
			Vector2 m_CenterPos;	// 円の中心
			float m_Radius;			// 半径
			Circle(){}
		public:
			Circle(const Vector2 CenterPos, float Radius) :m_CenterPos(CenterPos), m_Radius(Radius){}
			bool Collision(const Vector2 &pos);
			void RenderDebug();
		};
	};
	Collision2D::Base *m_pCollision;			// 判定委譲くん

};

class IconButtonManager
{
public:

	static const int NOT_IN_BUTTON = -1;	// ボタン範囲内がなかった

	IconButtonManager();
	~IconButtonManager();
	void Update(const Vector2 &CursorPos);	// 引数には、マウス等のカーソルの座標を入れる
	void Render();

	// データ追加
	//int Push(char *TexPath, int dstX, int dstY, BYTE InAction, char *se_ID = nullptr);
	void TextLoad(char *filename);

	// 範囲内であれば、その画像をプッシュしたときに返ってきた数値が返ってくる。何も範囲内でなければ
	int GetInButtonNo();

	void SetEnDis(UINT ID, EN_DIS_TYPE type);	// Enable,Disable。ボタンの有効化と無効化

	// リスト消去
	void Clear();

private:
	std::vector<IconButton*> m_List;
	int m_Number;
};