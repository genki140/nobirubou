//Genki3D.H
#ifndef GENKI3D_H
#define GENKI3D_H
#include <d3dx8.h>

class Genki3D{
public:
	Genki3D();
	void SetFrame(int Frame = 0);
	bool Load(LPDIRECT3DDEVICE8 D3DDevice,char FileName[]);
	int GetFrame();
	int GetFrameMax();
	void SetState(int Index=0,double State = 0);
	double GetState(int Index=0,int Frame = -1);
	double GetState2(int Index=0);
	int GetIndexMax();
    bool RenderRange(LPDIRECT3DDEVICE8 D3DDevice,D3DXMATRIX Matrix,int Start = 0,int Goal = -1);
private:
	bool In3D;
	int FramePo,FrameMax,IndexMax;
	int					OBKi[1000];
	int					OBBy[1000];
	double				OBSt[1000][10000];
	double				OBSt2[1000];
	D3DXVECTOR3			OBPo[1000];
	LPD3DXMESH			OBMe[1000];
	D3DMATERIAL8*		OBMeMa[1000];
	LPDIRECT3DTEXTURE8*	OBMeTe[1000];
	DWORD				OBMeNu[1000];
};
#endif
