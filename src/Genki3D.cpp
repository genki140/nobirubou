#include<stdio.h>
#include <stdlib.h>

#include <d3dx8.h>
#include "Genki3D.h"


Genki3D::Genki3D(){In3D = false;}

bool Genki3D::Load(LPDIRECT3DDEVICE8 D3DDevice,char FileName[])
{

    LPD3DXBUFFER pD3DXMtrlBuffer;


	char str[100];
	FILE *fp;
	if((fp = fopen(FileName,"r")) == NULL)
		return false;
	else
	{
		fscanf(fp,"%s",str);
		IndexMax = atoi(str);
		fscanf(fp,"%s",str);
		FrameMax = atoi(str);

		int a,b;

		for(a=1;a<=IndexMax;a++)
		{
			fscanf(fp,"%s",str);
			OBKi[a] = atoi(str);

			fscanf(fp,"%s",str);
			OBPo[a].x = (float)atof(str);

			fscanf(fp,"%s",str);
			OBPo[a].y = (float)atof(str);

			fscanf(fp,"%s",str);
			OBPo[a].z = (float)atof(str);

			fscanf(fp,"%s",str);
			OBBy[a] = atoi(str);

			fscanf(fp,"%s",str);

			fscanf(fp,"%s",str);




			if(OBKi[a] == 9)
			{

				if( FAILED( D3DXLoadMeshFromX( str, D3DXMESH_SYSTEMMEM,D3DDevice, NULL,&pD3DXMtrlBuffer, &OBMeNu[a],&OBMe[a] ) ) )
				{
					return false;
				}

				D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
				OBMeMa[a] = new D3DMATERIAL8[OBMeNu[a]];
				OBMeTe[a]  = new LPDIRECT3DTEXTURE8[OBMeNu[a]];

				for( DWORD i=0; i<OBMeNu[a]; i++ )
				{
					OBMeMa[a][i] = d3dxMaterials[i].MatD3D;
					OBMeMa[a][i].Ambient = OBMeMa[a][i].Diffuse;
					if( FAILED( D3DXCreateTextureFromFile( D3DDevice,d3dxMaterials[i].pTextureFilename, &OBMeTe[a][i] ) ) )
					{
						OBMeTe[a][i] = NULL;
					}
				}

			}




			for(b=0;b<=FrameMax;b++)
			{
				fscanf(fp,"%s",str);
				OBSt[a][b]=atof(str); 
			}
		}		
		fclose(fp);
		In3D = true;
		return true;
	}


}

void Genki3D::SetFrame(int Frame)
{
	if(In3D)
		FramePo = Frame % (FrameMax+1);
	if(FramePo < 0)
		FramePo = FrameMax+1-FramePo;
	int a;
	for(a=0;a<=IndexMax;a++)
	{
		OBSt2[a]=OBSt[a][FramePo];
	}
}

int Genki3D::GetFrame()
{
	if(In3D)	
		return FramePo;
	else
		return 0;
}

int Genki3D::GetFrameMax()
{
	if(In3D)
		return FrameMax;
	else
		return 0;
}

int Genki3D::GetIndexMax()
{
	if(In3D)
		return IndexMax;
	else
		return 0;
}

void Genki3D::SetState(int Index,double State)
{
	if(In3D)
	{
		if ((0 < Index) && (Index < 1000))
			OBSt2[Index] = State;
	}
}

double Genki3D::GetState(int Index,int Frame)
{
	if(In3D)
	{
		if(Frame == -1)
			Frame = FramePo;
		if(Frame < 0)
			Frame = 0;
		if(Frame > FrameMax)
			Frame = FrameMax;
		if(Index < 0)
			Index = 0;
		if(Index > IndexMax)
			Index = IndexMax;
		return OBSt[Index][Frame];
	}
	else
		return 0;
}

double Genki3D::GetState2(int Index)
{
	if(In3D)
	{
		if(Index < 0)
			Index = 0;
		if(Index > IndexMax)
			Index = IndexMax;
		return OBSt2[Index];
	}
	else
		return 0;
}

bool Genki3D::RenderRange(LPDIRECT3DDEVICE8 D3DDevice,D3DXMATRIX Matrix,int Start,int Goal)
{
	if(In3D)
	{
		if (Goal == -1)
			Goal = IndexMax;
		if (Start < 0)
			Start = 0;
		if (Start > IndexMax)
			Start = IndexMax;
		if (Goal < 0)
			Goal = 0;
		if (Goal > IndexMax)
			Goal = IndexMax;
		if (Start > Goal){
			int mannnaka = Start;
			Start = Goal;
			Goal = mannnaka;
		}


		int a,b;
		D3DXMATRIX matWorld,matWorld2;
		D3DXMatrixIdentity( &matWorld );
		D3DXMatrixIdentity( &matWorld2 );

		for(a = Start;a<=Goal;a++)
		{
			if(OBKi[a] == 9)
			{

				D3DXMatrixTranslation (&matWorld, OBPo[a].x, OBPo[a].y, OBPo[a].z);

				b = OBBy[a];
				bool bo =false;

				do{

					switch (OBKi[b])
					{
					case 0:
					D3DXMatrixTranslation (&matWorld2, (float)OBSt2[b], 0, 0);
					break;
					case 1:
					D3DXMatrixTranslation (&matWorld2, 0, (float)OBSt2[b], 0);
					break;
					case 2:
					D3DXMatrixTranslation (&matWorld2, 0, 0, (float)OBSt2[b]);
					break;
					case 3:
					D3DXMatrixRotationX (&matWorld2, (float)3.141592 / 180 * (float)OBSt2[b]);
					break;
					case 4:
					D3DXMatrixRotationY (&matWorld2, (float)3.141592 / 180 * (float)OBSt2[b]);
					break;
					case 5:
					D3DXMatrixRotationZ (&matWorld2, (float)3.141592 / 180 * (float)OBSt2[b]);
					break;
					case 6:
					D3DXMatrixScaling (&matWorld2, (float)OBSt2[b], 1, 1);
					break;
					case 7:
					D3DXMatrixScaling (&matWorld2, 1, (float)OBSt2[b], 1);
					break;
					case 8:
					D3DXMatrixScaling (&matWorld2, 1, 1, (float)OBSt2[b]);
					break;
					}


					D3DXMatrixMultiply (&matWorld, &matWorld, &matWorld2);


					D3DXMatrixTranslation (&matWorld2, OBPo[b].x, OBPo[b].y, OBPo[b].z);
					D3DXMatrixMultiply (&matWorld, &matWorld, &matWorld2);


					b = OBBy[b];
					if(b == 0)
					{
						bo = true;
					}



				}while(bo==false);
				    

				D3DXMatrixMultiply (&matWorld, &matWorld,&Matrix);

				D3DDevice->SetTransform (D3DTS_WORLD, &matWorld);

				//ÉÅÉbÉVÉÖÇÃèëÇ´çûÇ›
				DWORD c;
				for (c = 0;c <= OBMeNu[a] - 1;c++)
				{
					D3DDevice->SetMaterial( &OBMeMa[a][c] );
					D3DDevice->SetTexture( 0, OBMeTe[a][c] );
					OBMe[a]->DrawSubset (c);
				}
			}
		}







		return true;
	}
	else
		return false;
}
