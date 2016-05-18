#include	"LightCtrl.h"
#include	"MousePointer.h"
#include	"IEX_Expansion.h"

namespace{
	namespace BRIGHTNESS{
		const float MAX = 0.1f;
		const float MIN = 0.0f;
	}
	const int ALPHA_DOWN_TIME = 15;
	const float ALPHA_CHANGE = BRIGHTNESS::MAX/ALPHA_DOWN_TIME;

	const int FLOOR_SIZE = 720/3;
}

LightCtrl::LightCtrl()
{
}

LightCtrl::~LightCtrl()
{
}

void LightCtrl::Init()
{
	for(int i=0; i<MAX_FLOOR; i++){
		alpha[i] = BRIGHTNESS::MIN;
	}
}

void LightCtrl::Update()
{
	for(int i=0; i<MAX_FLOOR; i++){
		//マウス位置の階層で明るく、それ以外は暗く
		if( i == pointerFloor ){
			alpha[i] -= ALPHA_CHANGE;
			if( alpha[i] < BRIGHTNESS::MIN ){
				alpha[i] = BRIGHTNESS::MIN;
			}
		}else{
			alpha[i] += ALPHA_CHANGE;
			if( alpha[i] > BRIGHTNESS::MAX ){
				alpha[i] = BRIGHTNESS::MAX;
			}
		}
	}
}

void LightCtrl::Render()
{
	tdnPolygon::Rect( 0, 0,           1280,FLOOR_SIZE, RS::COPY, ARGB((int)(alpha[2]*255), 0, 0, 0) );
	tdnPolygon::Rect( 0, FLOOR_SIZE,  1280,FLOOR_SIZE, RS::COPY, ARGB((int)(alpha[1]*255), 0, 0, 0) );
	tdnPolygon::Rect( 0, FLOOR_SIZE*2,1280,FLOOR_SIZE, RS::COPY, ARGB((int)(alpha[0]*255), 0, 0, 0) );
}

void LightCtrl::DataReceive(MousePointer* mp)
{
	pointerFloor = mp->GetNowFloor();
}