#include "Result2.h"
#include "UI\ResultUIManager.h"

Result2::Result2()
{


	RESULT_UIMNG;
}

Result2::~Result2()
{

	RESULT_UIMNG.Release();
}

bool Result2::Update()
{
	RESULT_UIMNG.Update();

	return false;
}

void Result2::Render()
{
	RESULT_UIMNG.Render();

}
