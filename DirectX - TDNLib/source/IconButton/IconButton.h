#pragma once

//*****************************************************************************************************************************
//
//		いつものボタン
//
//*****************************************************************************************************************************

// ボタン有効・無効フラグ定数
enum class EN_DIS_TYPE
{
	ENABLE,				// 稼働中だよ
	DISABLE,			// 普通に描画されるが使えない
	DISABLE_BLACK,		// 使えない的なメッセージ
	DISABLE_WHITE,		// 何か選択中的なメッセージ
	DISABLE_VANISH		// 消える
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

	// 初期化・解放・更新・描画
	IconButton() :m_pCollision(nullptr), m_EnDisType(EN_DIS_TYPE::ENABLE), m_SEReceive(-1){}
	void Initialize(int ID, char *TexPath, int dstX, int dstY, BYTE InAction, char* se_ID);
	~IconButton();
	void Update(const Vector2 &CursorPos);
	void Render();

	// ボタンの有効・無効の設定
	void SetEnable(EN_DIS_TYPE type)
	{
		m_EnDisType = type;

		// 無効化するときはm_In(カーソルが範囲内に入っているかのフラグ)をOFFにする
		if(type != EN_DIS_TYPE::ENABLE) m_In = false;
	}

	// ゲッタ・セッタ
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

	int m_SEReceive;		// 1フレーム毎にSEを呼ばないように使う用


	// 判定委譲クラス
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
			Squre(){}			// デフォルトコンストラクタ封印
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
			Circle(){}				// デフォルトコンストラクタ封印
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

	// 初期化・解放・更新・描画
	IconButtonManager();
	~IconButtonManager();
	void Update(const Vector2 &CursorPos);	// 引数には、マウス等のカーソルの座標を入れる
	void Render();

	// データ追加
	void TextLoad(char *filename);

	// 範囲内であれば、その画像をプッシュしたときに返ってきた数値が返ってくる。何も範囲内でなければ-1
	int GetInButtonNo();

	// Enable,Disable。ボタンの有効化と無効化
	void SetEnDis(UINT ID, EN_DIS_TYPE type);

	// リスト消去
	void Clear();

private:
	std::vector<IconButton*> m_List;	// ボタン情報格納可変長配列
};