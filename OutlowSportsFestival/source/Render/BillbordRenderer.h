#ifndef __BILLBORD_RENDERER_H__
#define __BILLBORD_RENDERER_H__

#include "Renderer.h"

class BillbordRenderer :public ForwardRenderer
{
public:
	struct Param
	{
		Vector3 pos;
		Vector2 size;

		float TU_Begin;
		float TU_End;

		float TV_Begin;
		float TV_End;

		DWORD Color;
		
	};
private:
	void CalcZ();
	void Render();
};

#endif