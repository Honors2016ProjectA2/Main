#pragma once

//*****************************************************************************************************************************
//
//		パート管理クラス
//
//*****************************************************************************************************************************

#include "../Scene/SceneMain.h"

// 定数の前方宣言
enum class PERSON_TYPE;
enum BUTTON_ID;

//--------------------チュートリアル
namespace TutorialContents
{
	class Base
	{
	protected:
		int step;
		// 配置として描画するメッシュ
		iex3DObj *m_HoldMesh;
		Vector3 m_HoldMeshPos;
	public:
		Base() :step(0), m_HoldMesh(nullptr), m_HoldMeshPos(Vector3(0, 0, 0)){}
		virtual void Initialize(sceneMain *pMain){}
		virtual ~Base(){}
		virtual bool Update(sceneMain *pMain) = 0;
		virtual void Render(sceneMain *pMain) = 0;
		virtual void Render2D(sceneMain *pMain) = 0;
	};

	class One :public Base
	{
	public:
		One() :Base(){}
		void Initialize(sceneMain *pMain);
		~One();
		bool Update(sceneMain *pMain);
		void Render(sceneMain *pMain);
		void Render2D(sceneMain *pMain);
	};

	class Two :public Base
	{
	public:
		Two() :Base(){}
		void Initialize(sceneMain *pMain);
		~Two();
		bool Update(sceneMain *pMain);
		void Render(sceneMain *pMain);
		void Render2D(sceneMain *pMain);
	};

	class Three :public Base
	{
	public:
		Three() :Base(){}
		void Initialize(sceneMain *pMain);
		~Three();
		bool Update(sceneMain *pMain);
		void Render(sceneMain *pMain);
		void Render2D(sceneMain *pMain);
	};
};

class TutorialManager
{
public:

	TutorialManager() :m_pTutorial(nullptr) {};
	~TutorialManager();

	// 入る
	void Initialize(sceneMain* pMain, int StageNo);

	// 実行します
	bool Update(sceneMain* pMain);

	// 描画
	void Render(sceneMain* pMain);
	void Render2D(sceneMain* pMain);

private:

	// チュートリアル委譲ポインタ
	TutorialContents::Base *m_pTutorial;

	TutorialManager(const TutorialManager&);
	TutorialManager& operator=(const TutorialManager&);
};


