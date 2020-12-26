#define STRICT
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <D3DX8.h>
#include <tchar.h>
#include "D3DApp.h"
#include "D3DFile.h"
#include "D3DFont.h"
#include "D3DUtil.h"
#include "DXUtil.h"
#include "DIUtil.h"
#include "DSUtil.h"
#include "show.h"

#define Null (bool)0
#define null (bool)0
#define False (bool)0
#define True (bool)1
#define rad (float)(3.14159265358979/180)
#define Rad (float)(3.14159265358979/180)
#define RAD (float)(3.14159265358979/180)

#define indexmax (int)10000

#define Frame_FrameLate (int)60 //フレームレート数調整値

int Frame_FrameLateN =0;


int gamezyoutai=0;
int titlezyoutai=0;//799;

bool suittiok;
int suittikind;

	float axisang=0;
	float axisang2=0;

int goal=0;
int over=0;

int kuria=1;
int stage=1;
int stageselect=1;
float stagea;

float cameralx,cameraly;

int blockk[indexmax];
float blockx[indexmax],blocky[indexmax];
float lx,ly,mx,my;
int anime=0;
int udedamageanime=0;
int damageanime=0;
bool stand=false,jump=false,tune=false;
int rope=0;
float ropex[1000],ropey[1000];
bool ropemuki[1000];
float ropea,roper,ropema;
float maeropex,maeropey;
int ropelevel=0;
int ropezyoutai=0;


struct MYVERTEX
{
    D3DXVECTOR3 p;
    FLOAT       u, v; 
};
#define D3DFVF_MYVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)




float rnd(float rndmax){float random=(float)rand();random/=RAND_MAX;random*=rndmax;return random;}

float angle(float x,float y){float abcde;if((x==0) && (y>=0))return 0.0f;if((x==0) && (y<=0))return 180.0f;if((x>=0) && (y==0))return 90.0f;if((x<=0) && (y==0))return 270.0f;abcde=(float)(180.0/3.14159265358979*atan(x/y));if(y<0)abcde+=180;else if((x<0) && (y>0))abcde+=360;return abcde;}

float reangle(float angle){while((angle < 0) || (angle >= 360)){if(angle < 0)angle+=360;if(angle >= 360)angle-=360;}return angle;}

bool Load(char File_Name[100]){

	int a;
	for(a=0;a<indexmax;a++)blockk[a]=0;

	lx=0;
	ly=0;
	mx=0;
	my=0;
	anime=0;
	stand=false;
	jump=false;
	tune=false;
	rope=0;
	ropea=0;
	roper=0;
	ropema=0;
	maeropex=0;
	maeropey=0;
	ropelevel=0;
	ropezyoutai=0;

	char name[100]="Data/Stage/";
	_tcscat( name, File_Name );
	char str[100];
	FILE *fp;
	if((fp = fopen(name,"r")) == NULL)
		return false;
	else{
		int i;
		lx=0;
		ly=0.5f;
		mx=0;
		my=0;
		for(i=0;i<indexmax;i++)
			blockk[i]=0;
		for(i=0;i<indexmax;i++){
		fscanf(fp,"%s",str);
		blockk[i] = atoi(str);
			if (blockk[i]==1000000){
				blockk[i]=0;
				i=indexmax;}
			else{
				fscanf(fp,"%s",str);
				blockx[i]=(float)atoi(str);
				fscanf(fp,"%s",str);
				blocky[i]=(float)atoi(str);}}
		fclose(fp);
		//スタートを作る
//		for (DWORD i=0;i<indexmax;i++)
//			if(blockk[i]==0)
//				blockk[i]=1000000;
		return true;}
}




bool te(float* a,float* r,float x1,float y1,float x2,float y2,float bx,float by)//手とブロックとの当たり判定
{
	float bx1,by1,bx2,by2;

	float atarix,atariy;
	float atarix1=1000000,atariy1;
	float atarix2=1000000,atariy2;
	float atarix3=1000000,atariy3;
	float atarix4=1000000,atariy4;
	float test;

	//上
	bx1=bx-0.5f;
	by1=by+0.5f;
	bx2=bx+0.5f;
	by2=by+0.5f;


	if((y1>=by1 && y2<by1) || (y2>=by1 && y1<by1))
	{
		test=x1+(x2-x1)*(y1-by1)/(y1-y2); 
		if(test>=bx1 && test<=bx2)
		{
				atarix1=test;
				atariy1=by1+0.001f;
		}
	}



	//下
	bx1=bx-0.5f;
	by1=by-0.5f;
	bx2=bx+0.5f;
	by2=by-0.5f;

	if((y1>=by1 && y2<by1) || (y2>=by1 && y1<by1))
	{
		test=x1+(x2-x1)*(y1-by1)/(y1-y2); 
		if(test>=bx1 && test<=bx2)
		{
				atarix2=test;
				atariy2=by1-0.001f;
		}
	}






	//右
	by1=by-0.5f;
	bx1=bx+0.5f;
	by2=by+0.5f;
	bx2=bx+0.5f;

	if((x1>=bx1 && x2<bx1) || (x2>=bx1 && x1<bx1))
	{
		test=y1+(y2-y1)*(x1-bx1)/(x1-x2); 
		if(test>=by1 && test<=by2)
		{
				atariy3=test;
				atarix3=bx1+0.001f;
		}
	}


//	//左
	by1=by-0.5f;
	bx1=bx-0.5f;
	by2=by+0.5f;
	bx2=bx-0.5f;

	if((x1>=bx1 && x2<bx1) || (x2>=bx1 && x1<bx1))
	{
		test=y1+(y2-y1)*(x1-bx1)/(x1-x2); 
		if(test>=by1 && test<=by2)
		{
				atariy3=test;
				atarix3=bx1-0.001f;
		}
	}



	if(atarix1==1000000 && atarix2==1000000 && atarix3==1000000 && atarix4==1000000)
	{
		return false;
	}
	else
	{
		atarix=atarix1;
		atariy=atariy1;
		if(sqrtf((atarix-x1)*(atarix-x1)+(atariy-y1)*(atariy-y1))>sqrtf((atarix2-x1)*(atarix2-x1)+(atariy2-y1)*(atariy2-y1)))
		{
			atarix=atarix2;
			atariy=atariy2;
		}
		if(sqrtf((atarix-x1)*(atarix-x1)+(atariy-y1)*(atariy-y1))>sqrtf((atarix3-x1)*(atarix3-x1)+(atariy3-y1)*(atariy3-y1)))
		{
			atarix=atarix3;
			atariy=atariy3;
		}
		if(sqrtf((atarix-x1)*(atarix-x1)+(atariy-y1)*(atariy-y1))>sqrtf((atarix4-x1)*(atarix4-x1)+(atariy4-y1)*(atariy4-y1)))
		{
			atarix=atarix4;
			atariy=atariy4;
		}

//		atariy+=0.5;
		*a=angle(atarix,atariy);
		*r=sqrtf(atarix*atarix+atariy*atariy);

		return true;
	}

}


bool ude(float* a,float* r,float x1,float y1,float x2,float y2,float bx,float by)//腕とブロックとの当たり判定
{	
	float ma;

	//どっち回りかの確認
	float a1=angle(x1,y1),a2=angle(x2,y2);
	ma=reangle(a2-a1);
	if (ma>=180)ma-=360;
 
	if(a1==a2)
		return false;

	float ataria,atarir;
	float ataria1,atarir1=0;
	float ataria2,atarir2=0;
	float ataria3,atarir3=0;
	float ataria4,atarir4=0;
	float x,y,ang;
	
	if (ma>0)
	{
		ataria1=1000000;
		ataria2=1000000;
		ataria3=1000000;
		ataria4=1000000;
	}
	else
	{
		ataria1=-1000000;
		ataria2=-1000000;
		ataria3=-1000000;
		ataria4=-1000000;
	}


	//右上
	x=bx+0.5f;
	y=by+0.5f;
	ang=angle(x,y);

	if(sinf(rad*(ang-a1))*ma>0 && sinf(rad*(ang-a2))*ma<=0)
	{
		if ( sin((angle(x-x1,y-y1)-angle(x2-x1,y2-y1))*rad)*ma>=0 && sqrtf(x*x+y*y)>0.0001f)
		{
			ataria1=angle(x,y);
			atarir1=sqrtf(x*x+y*y);
		}
	}

	
	//右下
	x=bx+0.5f;
	y=by-0.5f;
	ang=angle(x,y);

	if(sinf(rad*(ang-a1))*ma>0 && sinf(rad*(ang-a2))*ma<=0)
	{
		if ( sin((angle(x-x1,y-y1)-angle(x2-x1,y2-y1))*rad)*ma>=0 && sqrtf(x*x+y*y)>0.0001f)
		{
			ataria2=angle(x,y);
			atarir2=sqrtf(x*x+y*y);
		}
	}



	//左上
	x=bx-0.5f;
	y=by+0.5f;
	ang=angle(x,y);

	if(sinf(rad*(ang-a1))*ma>0 && sinf(rad*(ang-a2))*ma<=0)
	{
		if ( sin((angle(x-x1,y-y1)-angle(x2-x1,y2-y1))*rad)*ma>=0 && sqrtf(x*x+y*y)>0.0001f)
		{
			ataria3=angle(x,y);
			atarir3=sqrtf(x*x+y*y);
		}
	}

	
	//左下
	x=bx-0.5f;
	y=by-0.5f;
	ang=angle(x,y);

	if(sinf(rad*(ang-a1))*ma>0 && sinf(rad*(ang-a2))*ma<=0)
	{
		if ( sin((angle(x-x1,y-y1)-angle(x2-x1,y2-y1))*rad)*ma>=0 && sqrtf(x*x+y*y)>0.0001f)
		{
			ataria4=angle(x,y);
			atarir4=sqrtf(x*x+y*y);
		}
	}



	if(atarir1==0 && atarir2==0 && atarir3==0 && atarir4==0)
		return false;


	ataria=ataria1;
	atarir=atarir1;

	if ((ataria-ataria2)*ma>0 || ((ataria-ataria2)*ma==0 && atarir<atarir2))
	{
		ataria=ataria2;
		atarir=atarir2;
	}
	if ((ataria-ataria3)*ma>0 || ((ataria-ataria3)*ma==0 && atarir<atarir3))
	{
		ataria=ataria3;
		atarir=atarir3;
	}
	if ((ataria-ataria4)*ma>0 || ((ataria-ataria4)*ma==0 && atarir<atarir4))
	{
		ataria=ataria4;
		atarir=atarir4;
	}

	*a=ataria;
	*r=atarir;

	return true;

}






//-----------------------------------------------------------------------------
class CMyD3DApplication : public CD3DApplication
{
    CD3DFont*     m_pFont;              // Font for drawing text

	LPDIRECT3DVERTEXBUFFER8 senVB;
    LPDIRECT3DVERTEXBUFFER8 itaVB;

	LPDIRECT3DTEXTURE8 HyouzyunTexture;
	LPDIRECT3DTEXTURE8 BackTexture[10];
	LPDIRECT3DTEXTURE8 BlockTexture[30];
	LPDIRECT3DTEXTURE8 BouTexture[2][10];
	LPDIRECT3DTEXTURE8 GenProTexture[5];
	LPDIRECT3DTEXTURE8 UdeTexture;
	LPDIRECT3DTEXTURE8 NameTexture[15];//0は煉獄庭園

	LPDIRECTINPUT8       di; 
	LPDIRECTINPUTDEVICE8 keyboard;   
	LPDIRECTINPUTDEVICE8 joystick;     
	CSoundManager* soundmanager;
	CSound*        runsound;
	CSound*        jumpsound;
	CSound*        tyakusound;


protected:
    HRESULT OneTimeSceneInit();
    HRESULT InitDeviceObjects();
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();
    HRESULT FinalCleanup();
    HRESULT Render();
    HRESULT FrameMove();

public:
    CMyD3DApplication();
};


CMyD3DApplication g_d3dApp;

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	srand((unsigned)time(NULL));

    if( FAILED( g_d3dApp.Create( hInst ) ) )
        return 0;

    return g_d3dApp.Run();
}




//-----------------------------------------------------------------------------
// Name: CMyD3DApplication() 
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CMyD3DApplication::CMyD3DApplication()
{

	m_FrameLate=60;
	m_Fullscreen=true;
	m_FrameLateEnable=true;

    m_strWindowTitle    = _T("伸びる棒");
    m_bUseDepthBuffer   = TRUE;

	m_dwCreationWidth   = 640;
    m_dwCreationHeight  = 480;

	m_pFont        = new CD3DFont( _T("HG創英角ﾎﾟｯﾌﾟ体"), 100);//, D3DFONT_BOLD );

    itaVB      = NULL;
    senVB      = NULL;

	DWORD i;
	for( i=0; i<10; i++ )
        BackTexture[i]=NULL;
	for( i=0; i<10; i++ )
        BlockTexture[i]=NULL;
	for( i=0; i<10; i++ )
        BouTexture[0][i]=NULL;
	for( i=0; i<10; i++ )
        BouTexture[1][i]=NULL;
	for( i=0; i<15; i++ )
        NameTexture[i]=NULL;
}




//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::OneTimeSceneInit()
{

	char name[100]="Data/Stage/Data";
	char str[10];
	FILE *fp;
	if((fp = fopen(name,"r")) == NULL)
		kuria=1;
	else
	{
		fscanf(fp,"%s",str);
		kuria = atoi(str);
		int check;
		fscanf(fp,"%d",&check);

		//データが不正な場合最初から
		if(check!=(int)(sqrtf((float)kuria*83)*899)+8782%10000)
			kuria=0;
	}

	fclose(fp);



//書き込むとき
	//fopen(name,"w");

	//fprintf(fp,"test");

	//fclose(fp);






	Load("0");


    DWORD dwCreationFlags;
    dwCreationFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;


    soundmanager = new CSoundManager();
    soundmanager->Initialize( m_hWnd, DSSCL_PRIORITY, 2, 22050, 16 );

	DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION,IID_IDirectInput8, (VOID**)&di, NULL );

	di->CreateDevice( GUID_SysKeyboard, &keyboard, NULL );
	keyboard->SetDataFormat( &c_dfDIKeyboard);
	keyboard->SetCooperativeLevel( m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    keyboard->Acquire();

	di->CreateDevice( GUID_Joystick, &joystick, NULL );

	if (joystick!=NULL)
	{
		joystick->SetDataFormat( &c_dfDIJoystick2 );
		joystick->SetCooperativeLevel( m_hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		joystick->Acquire();
	}

	soundmanager->Create( &runsound, "Data/Sound/run", DSBCAPS_CTRLFREQUENCY, GUID_NULL );

	soundmanager->Create( &jumpsound, "Data/Sound/jump", 0, GUID_NULL );

	soundmanager->Create( &tyakusound, "Data/Sound/tyaku", 0, GUID_NULL ); 

//	bool Looped = true;
//    DWORD dwFlags = Looped ? DSBPLAY_LOOPING : 0L;
    //music->Play( 0, dwFlags );

//	soundmanager->Create( &music, "Data/Sound/Music.wav", 0, GUID_NULL );
//    music->Play( 0, DSBPLAY_LOOPING );

	ShowSetting();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FrameMove()
{

//最初はフルスクリーン
//static bool FULL_Ikkoma=false;if (FULL_Ikkoma == false){FULL_Ikkoma=true;ToggleFullscreen();DXUtil_Timer( TIMER_RESET );}



	int i;
//	int j; 


    int     keyi;
    BYTE    diks[256];
	bool    key[256];
	for(keyi=0;keyi<256;keyi++)key[keyi]=false;
    ZeroMemory( &diks, sizeof(diks) );
	keyboard->Acquire();
	keyboard->GetDeviceState( sizeof(diks), &diks );
	for( keyi = 0; keyi < 256; keyi++ )if( diks[keyi] & 0x80 )key[keyi]=true;



//ジョイスティックの判定
	if (joystick!=NULL) 
	{
		DIJOYSTATE2 js;

		joystick->Poll(); 
		joystick->Acquire();
		joystick->GetDeviceState( sizeof(DIJOYSTATE2), &js );

		if (js.rgbButtons[0] & 0x80)
			key[44]=1;
		if (js.rgbButtons[1] & 0x80)
			key[45]=1;
		if (js.rgbButtons[2] & 0x80)
			key[46]=1;
		if (js.rgbButtons[3] & 0x80)
			key[1]=1;
//		if (js.rgbButtons[8] & 0x80)
//			key[1]=1;
		if(js.lX >65535/2+10000)
			key[205]=1;
		if(js.lX <65535/2-10000)
			key[203]=1;
		if(js.lY >65535/2+10000)
			key[208]=1;
		if(js.lY <65535/2-10000)
			key[200]=1;

	}
	






	if(gamezyoutai==0)//元気プロジェクト
	{
		if (key[1] != 0)
		PostMessage( m_hWnd, WM_CLOSE, 0, 0 );

		titlezyoutai++;





		static bool keydown=false;

		if(key[44]!=0)// Zキー ジャンプ
		{
			keydown=true;
		}
		else if(keydown==true)
		{
			titlezyoutai=800;
		}






		if (titlezyoutai == 800)
		{


			titlezyoutai=0;
			gamezyoutai=1;
			stage=0;
			stagea=rnd(360.0f);
			cameralx=sinf(stagea*rad)*100;
			cameraly=cosf(stagea*rad)*100;
			lx=0;
			ly=0;			
		}







	}
	else if(gamezyoutai==1)//ゲーム
	{


if(stage==0)
{
	//ステージセレクト


	//cameralx=0;
	//cameraly=0;

	mx=0;
	my=0;

	if (key[1] != 0)
	PostMessage( m_hWnd, WM_CLOSE, 0, 0 );

	static int stagekettei=0;

	if(stagekettei==0)
	{

	axisang+=0.023f;
	axisang2+=0.01f;
	if (axisang>=1000000)
	{
		axisang=0;
		axisang2=0;
	}

	static float keytime=0;
	keytime--;
	if(keytime<0)
		keytime=0;


	if(key[200]!=0 && keytime==0)
	{
		stageselect++;
		if(stageselect>kuria && stageselect<102)
			stageselect=102;
		keytime=8;
	}
	if(key[208]!=0 && keytime==0)
	{
		stageselect--;
		if(stageselect>kuria && stageselect<102)
			stageselect=kuria;
		keytime=8;
	}
	if(key[205]!=0 && keytime==0)
	{
		stageselect+=10;
		if(stageselect>kuria && stageselect<102)
			stageselect=102;
		keytime=8;
	}
	if(key[203]!=0 && keytime==0)
	{
		stageselect-=10;
		if(stageselect>kuria && stageselect<102)
			stageselect=kuria;
		keytime=8;
	}

	if(stageselect<1)
		stageselect=1;
	if(stageselect>1101)
		stageselect=1101;

	static bool kettei=false;
	if(key[44]!=0)// Zキー決定
	{
		kettei=true;
	}
	else if(kettei==true)//ステージ移動
	{
		kettei=false;
		char c[10]; 
		_stprintf(c,"%d",stageselect);
		if(stageselect>101)
			sprintf(c,"Edit%d",stageselect-101);

//		MessageBox(NULL,c,"",MB_OK);
		if (Load(c)==true)
		{
//			stage=stageselect;
			stagea=rnd(360.0f);
			lx=-sinf(stagea*rad)*100;
			ly=-cosf(stagea*rad)*100;
			stagekettei=1;

		}
	}


	}

	if(stagekettei!=0)
		stagekettei++;
	else
	{
		lx=0;
		ly=0;
	}
	if(stagekettei>=50)
	{

		//巻きもどして再生
		LONGLONG llPos = 0;
		pMediaSeeking->SetPositions(	&llPos, AM_SEEKING_AbsolutePositioning,
											&llPos, AM_SEEKING_NoPositioning);
		pMediaControl->Run();

		cameralx=sinf(stagea*rad)*100;
		cameraly=cosf(stagea*rad)*100;
		mx=0;
		my=0;
		lx=0;
		ly=0.5;
		over=0;
		goal=0;
		stagekettei=0;
		stage=stageselect;
	}

	cameralx=cameralx+((lx*20)-cameralx)/10;//+sinf(rad*ropea)*roper;
	cameraly=cameraly+((ly*20)-cameraly)/10;//+cosf(rad*ropea)*roper;

}
else if(stage==51)//スタッフロール
{
	static int sutaffu=0;

	if (sutaffu==0)
	{
		axisang=0;
		axisang2=0;
	}

//	for(i=0;i<=10;i++)
//	{

	sutaffu+=2;
	cameralx=0;
	cameraly=(float)-sutaffu*0.05f;


	axisang+=0.023f;
	axisang2+=0.01f;
	if (axisang>=1000000)
	{
		axisang=0;
		axisang2=0;
	}


//	}




	static bool esc=false;

	if (sutaffu>=8600)
	{
		sutaffu=0;
		key[1]=0;
		esc=true;
	}
	
	if (key[1] != 0)esc=true;
	else if(esc==true)
	{
		pMediaControl->Stop();

	titlezyoutai=0;
	gamezyoutai=1;
	stageselect=stage;
	stage=0;
	stagea=rnd(360.0f);
	cameralx=sinf(stagea*rad)*100;
	cameraly=cosf(stagea*rad)*100;
	lx=0;
	ly=0;	
	esc=false;
	sutaffu=0;
	}


}
else if(goal==0 && over==0)
{


	//音楽ループ再生(終了しちゃったらもう一度再生！)
	long evCode, param1, param2;
	if(SUCCEEDED(pEvent->GetEvent(&evCode, &param1, &param2, 0)))
	{
		pEvent->FreeEventParams(evCode, param1, param2);
		if ((EC_COMPLETE == evCode))
		{
			pMediaControl->Stop();

			//巻き戻し
			LONGLONG llPos = 0;
			pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,
										&llPos, AM_SEEKING_NoPositioning);
			pMediaControl->Run();
		} 
	}




static bool esc=false;
if (key[1] != 0)esc=true;
else if(esc==true)
{

	pMediaControl->Stop();

titlezyoutai=0;
gamezyoutai=1;
stageselect=stage;
stage=0;
stagea=rnd(360.0f);
cameralx=sinf(stagea*rad)*100;
cameraly=cosf(stagea*rad)*100;
lx=0;
ly=0;	
esc=false;
}





if(udedamageanime>0)
	udedamageanime--;




	if(key[205]!=0)
	{
//			mx+=0.02;
		if(jump==false && (ropelevel==0 || ropelevel==3) && key[46]==0)
		{
			anime+=2;
			if (anime>=40)
				anime=0;
			if(mx>0.15)
				mx-=0.01f;
			else
				mx+=0.01f;

			tune=false;
		}
	}
	else if(key[203]!=0)
	{
//			mx-=0.02;
		if(jump==false && (ropelevel==0 || ropelevel==3) && key[46]==0)
		{
			anime+=2;
			if (anime>=40)
				anime=0;
			if(mx<-0.15)
				mx+=0.01f;
			else
				mx-=0.01f;
			tune=true;
		}
	}

	static bool hokou=false;
//	if((anime==20 || anime==0) && hokou==true)
	if((anime==0) && hokou==true)
	{

		runsound->Stop();
		runsound->Reset();
		runsound->GetBuffer( 0 )->SetFrequency( 44100-(int)rnd(12000) );
	    runsound->Play( 0, 0L );

		hokou=false;
	}
	else if(anime!=20 && anime!=0)
	{
		hokou=true;
	}




	if(key[44]!=0)// Zキー ジャンプ
	{
		if(jump==false && (ropelevel==0 || ropelevel==3))
		{
			my=0.23f;
		    jumpsound->Play( 0, 0L );
		}
	}


	if(ropelevel==0)
	{
		maeropex=lx;
		maeropey=ly;
	}



	static bool ON_45=false;

	if(key[45]!=0)// Xキー ロープ発射＆停止＆巻く
	{
		if(ropelevel==0 && mx<=0.0001 && mx>=-0.0001 && my<=0.001 && my>=-0.001 && jump==false && rope==0 && ON_45==false)
		{
			ropema=0;
			ropelevel=1;//発射
			rope=1;
			ropex[0]=lx;
			ropey[0]=ly+0.001f;
			ropex[1]=lx+sinf(rad*ropea)*0.1f;
			ropey[1]=ly+cosf(rad*ropea)*0.1f+0.001f;
			ON_45=true;
		}
		else if(ropelevel>=2 && ON_45==false)
		{
			if (ropelevel==3)
			{

				//入れ替え!!
				float damiropex[1000],damiropey[1000];
				for(i=0;i<=rope;i++)
				{
					damiropex[i]=ropex[i];
					damiropey[i]=ropey[i];
				}
				for(i=0;i<=rope;i++)
				{
					ropex[i]=damiropex[rope-i];
					ropey[i]=damiropey[rope-i];
				}


			}
			ropelevel=0;//巻き戻す
			ON_45=true;
		}
	}
	else
	{
		if(ropelevel==1)
			ropelevel=2;//止まれ

			ON_45=false;

	}



	ropezyoutai=0;

	if(key[200]!=0)
	{
//			my+=0.02;
		if(ropelevel==3)
		{
			//上る
			ropezyoutai=1;
		}
		else if(key[46]==0)
		{
			ropea+=1;
			roper+=0.15f;
		}
	}
	if(key[208]!=0)
	{
//			my-=0.02;
		if(ropelevel==3)
		{
			//下る
			ropezyoutai=2;
		}
		else if(key[46]==0)
		{
			ropea-=1;
			roper+=0.15f;
		}
	}





	if(key[46]!=0)// Cキー ロープ緩める
	{
		if(ropelevel==3)
			ropezyoutai=3;
		else
		{

			static bool keydown[4];			
			if(key[200]!=0)
				keydown[0]=true;
			if(key[205]!=0)
				keydown[1]=true;
			if(key[208]!=0)
				keydown[2]=true;
			if(key[203]!=0)
				keydown[3]=true;

			if(key[200]==0 && key[205]==0 && key[208]==0 && key[203]==0)
			{
				keydown[0]=false;
				keydown[1]=false;
				keydown[2]=false;
				keydown[3]=false;
			}


			if(keydown[0]==true)//上
			{
				if(keydown[1]==true)//右
				{
					ropea=45;
				}
				else if(keydown[3]==true)//左
				{
					ropea=270+45;
				}
				else
				{
					ropea=0;					
				}
			}
			else if(keydown[2]==true)//下
			{
				if(keydown[1]==true)//右
				{
					ropea=90+45;					
				}
				else if(keydown[3]==true)//左
				{
					ropea=180+45;
				}
				else
				{
					ropea=180;
				}
			}
			else
			{
				if(keydown[1]==true)//右
				{
					ropea=90;					
				}
				else if(keydown[3]==true)//左
				{
					ropea=270;
				}
				else
				{
//					ropea=180;
				}
			}
		}
	}

	if((jump==false) && (((key[205]==0) && (key[203]==0)) || (key[46]!=0)))
	{
		mx*=0.7f;
	}

		my-=0.01f;
		mx*=0.999f;
		my*=0.999f;


	if (mx > 1)
		mx=1;
	if(mx<-1)
		mx=-1;

	if (my > 1)
		my=1;

	//落下しすぎるとゲームオーバー
	if(my<=-2.0f)
		over=1.0;





	if(ropelevel==0 && rope!=0)
	{

		float r=sqrtf((ropex[rope]-ropex[rope-1])*(ropex[rope]-ropex[rope-1])+(ropey[rope]-ropey[rope-1])*(ropey[rope]-ropey[rope-1])),x=(ropex[rope]-ropex[rope-1])/r,y=(ropey[rope]-ropey[rope-1])/r;
		if(r<=1)
			rope--;
		else
		{
			ropex[rope]=ropex[rope]-(x);
			ropey[rope]=ropey[rope]-(y);
		}		
		if(rope<0)
			rope=0;	

	}
	else if(ropelevel==1)
	{
		float ang=angle(ropex[1]-ropex[0],ropey[1]-ropey[0]);
		float kyori=sqrtf((ropex[1]-ropex[0])*(ropex[1]-ropex[0])+(ropey[1]-ropey[0])*(ropey[1]-ropey[0]));
		kyori+=0.3f;
		if (kyori>20)
			kyori=20;
		ropex[1]=ropex[0]+sinf(rad*ang)*kyori;
		ropey[1]=ropey[0]+cosf(rad*ang)*kyori;

	}
	else if(ropelevel==2)
	{
		float ang=angle(ropex[rope]-ropex[rope-1],ropey[rope]-ropey[rope-1]);
		float kyori=sqrtf((ropex[rope]-ropex[rope-1])*(ropex[rope]-ropex[rope-1])+(ropey[rope]-ropey[rope-1])*(ropey[rope]-ropey[rope-1]));

		float testsin=sinf(rad*ang)*1/(kyori);
		ropema=ropema+testsin;

		ropema*=0.999f;
		ropex[rope]=ropex[rope-1]+sinf(rad*(ang+ropema))*kyori;
		ropey[rope]=ropey[rope-1]+cosf(rad*(ang+ropema))*kyori;
	}


	if(ropelevel==0)
	{
		//16と18を探す
		for(i=0;i<indexmax;i++)
		{
			if(blockk[i]==19)blockk[i]=16;
			if(blockk[i]==20)blockk[i]=18;
		}
	}




	//腕を直す
	if(rope>=2 && ropelevel!=0)
	{
		if(sinf(rad*(angle(ropex[rope]-ropex[rope-1],ropey[rope]-ropey[rope-1])-angle(ropex[rope-1]-ropex[rope-2],ropey[rope-1]-ropey[rope-2])))*((float)ropemuki[rope]-0.5f)>=0)
		{

			float kyori1=sqrtf((ropex[rope]-ropex[rope-2])*(ropex[rope]-ropex[rope-2])+(ropey[rope]-ropey[rope-2])*(ropey[rope]-ropey[rope-2]));
			float kyori2=sqrtf((ropex[rope]-ropex[rope-1])*(ropex[rope]-ropex[rope-1])+(ropey[rope]-ropey[rope-1])*(ropey[rope]-ropey[rope-1]));

			ropex[rope-1]=ropex[rope];
			ropey[rope-1]=ropey[rope];
			rope--;
			maeropex=ropex[rope];
			maeropey=ropey[rope];
			if(ropelevel==2)
				ropema=ropema*(kyori2/kyori1);
			
		}
	}





	float ma;

	//どっち回りかの確認
	float a1=angle(maeropex-ropex[rope-1],maeropey-ropey[rope-1]),a2=angle(ropex[rope]-ropex[rope-1],ropey[rope]-ropey[rope-1]),r=sqrtf((maeropex-ropex[rope-1])*(maeropex-ropex[rope-1])+(maeropey-ropey[rope-1])*(maeropey-ropey[rope-1]));
	ma=reangle(a2-a1);
	if (ma>=180)ma-=360;

	float atattaa,atattar=0,testa,testr;
	int atattaindex;
	bool hanndo;

	if (ma>=0)
		atattaa=1000000;
	else
		atattaa=-1000000;
	

	//ロープの判定

	if(rope!=0)
	{

		//手の判定
		if(ropelevel==1 || ropelevel==2)
		{

			for(i=0;i<indexmax;i++)
			{
				if(blockk[i]!=0)
				{
					if(te(&testa,&testr,maeropex-ropex[rope-1],maeropey-ropey[rope-1],ropex[rope]-ropex[rope-1],ropey[rope]-ropey[rope-1],blockx[i]-ropex[rope-1],blocky[i]-ropey[rope-1])==true)
					{
						if((atattaa-testa)*ma>0 || ((atattaa-testa)*ma==0 && (testr<atattar || atattar==0)))
						{
							if(blockk[i]==2 || blockk[i]==7 || blockk[i]==8 || blockk[i]==10 || blockk[i]==11 || blockk[i]==12 || blockk[i]==13 || blockk[i]==14 || blockk[i]==15 || blockk[i]==17)
							{
								atattaa=testa;
								atattar=testr;
								atattaindex=i;
								hanndo=true;
							}
						}
					}
				}
			}

		}

		//腕の判定
		if(ropelevel==2 || ropelevel==3)
		{

			for(i=0;i<indexmax;i++)
			{
				if(blockk[i]!=0)
				{
					if(ude(&testa,&testr,maeropex-ropex[rope-1],maeropey-ropey[rope-1],ropex[rope]-ropex[rope-1],ropey[rope]-ropey[rope-1],blockx[i]-ropex[rope-1],blocky[i]-ropey[rope-1])==true)
					{
						if((atattaa-testa)*ma>0 || ((atattaa-testa)*ma==0 && testr>atattar))
						{
							if(blockk[i]==2 || blockk[i]==7 || blockk[i]==8 || blockk[i]==12 || blockk[i]==13 || blockk[i]==14 || blockk[i]==15 || blockk[i]==17)
							{
								atattaa=testa;
								atattar=testr;
								atattaindex=i;
								hanndo=false;
							}
						}
					}
				}
			}

		}


		if(atattar!=0)
		{
			if(blockk[atattaindex]==8 || (blockk[atattaindex]==12 && hanndo==false) || (blockk[atattaindex]==13 && hanndo==true) || blockk[atattaindex]==14)//とげ
			{

				if (ropelevel==3)
				{

					//入れ替え!!
					float damiropex[1000],damiropey[1000];
					for(i=0;i<=rope;i++)
					{
						damiropex[i]=ropex[i];
						damiropey[i]=ropey[i];
					}
					for(i=0;i<=rope;i++)
					{
						ropex[i]=damiropex[rope-i];
						ropey[i]=damiropey[rope-i];
					}


				}
				ropelevel=0;
				udedamageanime=20;

			}
			else
			{

				if(hanndo==true)
				{



					if(blockk[atattaindex]==15)
					{
//						MessageBox(NULL,"","スイッチ1",MB_OK);
						suittiok=false;
						suittikind=1;
						//16と18を探す
						for(i=0;i<indexmax;i++)
						{
							if(blockk[i]==16)blockk[i]=19;
						}						
					}
					if(blockk[atattaindex]==17)
					{
//						MessageBox(NULL,"","スイッチ2",MB_OK);
						suittiok=false;
						suittikind=2;
						//16と18を探す
						for(i=0;i<indexmax;i++)
						{
							if(blockk[i]==18)blockk[i]=20;
						}						
					}


					ropelevel=3;
					ropex[rope]=ropex[rope-1]+sinf(rad*atattaa)*atattar;
					ropey[rope]=ropey[rope-1]+cosf(rad*atattaa)*atattar;
					//入れ替え!!
					float damiropex[1000],damiropey[1000];
					for(i=0;i<=rope;i++)
					{
						damiropex[i]=ropex[i];
						damiropey[i]=ropey[i];
					}
					for(i=0;i<=rope;i++)
					{
						ropex[i]=damiropex[rope-i];
						ropey[i]=damiropey[rope-i];
						if (ropemuki[i]==0)
							ropemuki[i]=1;
						else
							ropemuki[i]=0;
					}

				}
				else
				{
					float kyori=sqrtf((ropex[rope]-ropex[rope-1])*(ropex[rope]-ropex[rope-1])+(ropey[rope]-ropey[rope-1])*(ropey[rope]-ropey[rope-1]));
					ropex[rope+1]=ropex[rope-1]+sinf(rad*atattaa)*kyori;
					ropey[rope+1]=ropey[rope-1]+cosf(rad*atattaa)*kyori;
					ropex[rope]=ropex[rope-1]+sinf(rad*atattaa)*atattar;
					ropey[rope]=ropey[rope-1]+cosf(rad*atattaa)*atattar;
					if(ma>0)
					{
						ropemuki[rope+1]=false;
					}
					else
					{
						ropemuki[rope+1]=true;
					}
					rope++;
					if(ropelevel==2)
					{
						float kyori1=sqrtf((ropex[rope]-ropex[rope-2])*(ropex[rope]-ropex[rope-2])+(ropey[rope]-ropey[rope-2])*(ropey[rope]-ropey[rope-2]));
						float kyori2=sqrtf((ropex[rope]-ropex[rope-1])*(ropex[rope]-ropex[rope-1])+(ropey[rope]-ropey[rope-1])*(ropey[rope]-ropey[rope-1]));
						ropema=ropema*(kyori1/kyori2);
					}
				}
			}

		}


	}




	//前ロープ更新
	maeropex=ropex[rope];
	maeropey=ropey[rope];


	//ロープ状態に応じて、移動先の変更
	if(ropezyoutai==1)//上る
	{
		float r,a;
		float testx,testy;
		r=sqrtf(((lx)-ropex[rope-1])*((lx)-ropex[rope-1])+((ly)-ropey[rope-1])*((ly)-ropey[rope-1]))-0.1f;
		a=angle((lx+mx)-ropex[rope-1],(ly+my)-ropey[rope-1]);
		testx=(ropex[rope-1]+sinf(rad*a)*(r))-lx;
		testy=(ropey[rope-1]+cosf(rad*a)*(r))-ly;
		mx=mx+(testx-mx)*0.4f;
		my=my+(testy-my)*0.4f;
	}
	if(ropezyoutai==2)//下る
	{
		float r,a;
		float testx,testy;
		r=sqrtf(((lx)-ropex[rope-1])*((lx)-ropex[rope-1])+((ly)-ropey[rope-1])*((ly)-ropey[rope-1]))+0.1f;
		a=angle((lx+mx)-ropex[rope-1],(ly+my)-ropey[rope-1]);
		testx=(ropex[rope-1]+sinf(rad*a)*(r))-lx;
		testy=(ropey[rope-1]+cosf(rad*a)*(r))-ly;
		mx=mx+(testx-mx)*0.4f;
		my=my+(testy-my)*0.4f;
	}
	if(ropezyoutai==3)//止まる
	{
		float r,a;
		float testx,testy;
		r=sqrtf(((lx)-ropex[rope-1])*((lx)-ropex[rope-1])+((ly)-ropey[rope-1])*((ly)-ropey[rope-1]));
		a=angle((lx+mx)-ropex[rope-1],(ly+my)-ropey[rope-1]);
		testx=(ropex[rope-1]+sinf(rad*a)*(r))-lx;
		testy=(ropey[rope-1]+cosf(rad*a)*(r))-ly;
		mx=testx;//mx+(testx-mx)*0.2;
		my=testy;//my+(testy-my)*0.2;
	}




	if(ropelevel==3)//ロープ限界
	{
		float zentair=0;
		for(i=1;i<=rope;i++)
		{
			zentair=zentair+(sqrtf((ropex[i]-ropex[i-1])*(ropex[i]-ropex[i-1])+(ropey[i]-ropey[i-1])*(ropey[i]-ropey[i-1])));
		}
		if(zentair>40)
		{
		float r,a;
		float testx,testy;
		i=rope;
		r=sqrtf(((lx)-ropex[rope-1])*((lx)-ropex[rope-1])+((ly)-ropey[rope-1])*((ly)-ropey[rope-1]))-(zentair-40);//(sqrtf((ropex[i]-ropex[i-1])*(ropex[i]-ropex[i-1])+(ropey[i]-ropey[i-1])*(ropey[i]-ropey[i-1])));//sqrtf(((lx)-ropex[rope-1])*((lx)-ropex[rope-1])+((ly)-ropey[rope-1])*((ly)-ropey[rope-1]));
		a=angle((lx+mx)-ropex[rope-1],(ly+my)-ropey[rope-1]);
		testx=(ropex[rope-1]+sinf(rad*a)*(r))-lx;
		testy=(ropey[rope-1]+cosf(rad*a)*(r))-ly;
		mx=mx+(testx-mx)*0.5f;
		my=my+(testy-my)*0.5f;
		}
	}









	if(fmod(lx,1)<0.001 && lx>0)
		lx=(float)((int)lx);
	else if(fmod(lx,1)>1.0-0.001 && lx>0)
		lx=(float)((int)lx)+1;
	else if(fmod(lx,1)>-0.001 && lx<0)
		lx=(float)((int)lx);
	else if(fmod(lx,1)<-1.0+0.001 && lx<0)
		lx=(float)((int)lx)-1;

	//ブロックとの当たり判定

	jump=true;



int loop;
for(loop=0;loop<10;loop++)
{




	for (i=0;i<indexmax;i++)
	{
		if(blockk[i]!=0)
		{
			//移動先が食い込んでる
			if((lx+mx+0.5>blockx[i]-0.5) && (lx+mx-0.5<blockx[i]+0.5) && (ly+my+1>blocky[i]-0.5) && (ly+my-1<blocky[i]+0.5))
			{


				if((lx+0.5>blockx[i]-0.5) && (lx-0.5<blockx[i]+0.5) && (ly+1>blocky[i]-0.5) && (ly-1<blocky[i]+0.5))
				{//食い込んでた

					if(blockk[i]==1)//ゴール
					{
						goal=1;
						if (ropelevel==3)
						{

							//入れ替え!!
							float damiropex[1000],damiropey[1000];
							for(i=0;i<=rope;i++)
							{
								damiropex[i]=ropex[i];
								damiropey[i]=ropey[i];
							}
							for(i=0;i<=rope;i++)
							{
								ropex[i]=damiropex[rope-i];
								ropey[i]=damiropey[rope-i];
							}
						}
						ropelevel=0;

					}
					else if(blockk[i]==3)
					{
						float idou=0.4f;
						my=my+(idou-my)*0.01f;
					}
					else if(blockk[i]==4)
					{
						float idou=0.4f;
						mx=mx+(idou-mx)*0.01f;
					}
					else if(blockk[i]==5)
					{
						float idou=-0.4f;
						my=my+(idou-my)*0.01f;
					}
					else if(blockk[i]==6)
					{
						float idou=-0.4f;
						mx=mx+(idou-mx)*0.01f;
					}
					else if(blockk[i]==8 || blockk[i]==13)//とげ
					{
						over=1;
						if (ropelevel==3)
						{

							//入れ替え!!
							float damiropex[1000],damiropey[1000];
							for(i=0;i<=rope;i++)
							{
								damiropex[i]=ropex[i];
								damiropey[i]=ropey[i];
							}
							for(i=0;i<=rope;i++)
							{
								ropex[i]=damiropex[rope-i];
								ropey[i]=damiropey[rope-i];
							}
						}
						ropelevel=0;
					}
					else if(blockk[i]==16 || blockk[i]==18)//スイッチブロック
					{
						//前の位置が食い込んでいたかを確認
//						if(lx-mx+0.5f>blockx[i]-0.5f && lx-mx-0.5f<blockx[i]+0.5f && ly-my+1.0f>blocky[i]-0.5f && ly-my-1.0f<blocky[i]+0.5f )
						if(suittiok==false && ((blockk[i]==16 && suittikind==1) || (blockk[i]==18 && suittikind==2)))
						{
//							MessageBox(NULL,"スイッチによるゲームオーバー","",MB_OK);
							over=1;
							if (ropelevel==3)
							{

								//入れ替え!!
								float damiropex[1000],damiropey[1000];
								for(i=0;i<=rope;i++)
								{
									damiropex[i]=ropex[i];
									damiropey[i]=ropey[i];
								}
								for(i=0;i<=rope;i++)
								{
									ropex[i]=damiropex[rope-i];
									ropey[i]=damiropey[rope-i];
								}
							}
							ropelevel=0;
						}
						else
						{
							mx=0;
							my=0;
							if(lx <= blockx[i])
								lx=blockx[i]-1;
							else
								lx=blockx[i]+1;

							if(ly <= blocky[i])
								ly=blocky[i]-1.5f;
							else
								ly=blocky[i]+1.5f;

						}
					}
					else if(blockk[i]==10 || blockk[i]==19 || blockk[i]==20)
					{
					}
					else
					{
						mx=0;
						my=0;
						if(lx <= blockx[i])
							lx=blockx[i]-1;
						else
							lx=blockx[i]+1;

						if(ly <= blocky[i])
							ly=blocky[i]-1.5f;
						else
							ly=blocky[i]+1.5f;
					}
				}

			}

		}

	}







	for (i=0;i<indexmax;i++)
	{
		if(blockk[i]!=0)
		{
			//移動先が食い込んでる
			if((lx+mx+0.5>blockx[i]-0.5) && (lx+mx-0.5<blockx[i]+0.5) && (ly+my+1>blocky[i]-0.5) && (ly+my-1<blocky[i]+0.5))
			{

				//上からあたった
				if((ly-1>=blocky[i]+0.5) && (lx+0.5>blockx[i]-0.5) && (lx-0.5<blockx[i]+0.5))
				{
					if(blockk[i]==2 || blockk[i]==7 || blockk[i]==9 || blockk[i]==11 || blockk[i]==12 || blockk[i]==14 || blockk[i]==15 || blockk[i]==16 || blockk[i]==17 || blockk[i]==18)
					{

						if(my<-0.1f)
							tyakusound->Play( 0, 0L );

						if(blockk[i]==2 || blockk[i]==9 || blockk[i]==11 || blockk[i]==12 || blockk[i]==14 || blockk[i]==15 || blockk[i]==16 || blockk[i]==17 || blockk[i]==18)
							my=0;
						else if(blockk[i]==7)
							my=-my*0.9f;

						ly=blocky[i]+1.5f;
						if(blockk[i]!=7)
							jump=false;
						
					}
				}


				//左からあたった
				else if((lx+0.5<=blockx[i]-0.5) && (ly+1>blocky[i]-0.5) && (ly-1<blocky[i]+0.5))
				{
					if(blockk[i]==2 || blockk[i]==7 || blockk[i]==9 || blockk[i]==11 || blockk[i]==12 || blockk[i]==14 || blockk[i]==15 || blockk[i]==16 || blockk[i]==17 || blockk[i]==18)
					{
						if(blockk[i]==2 || blockk[i]==9 || blockk[i]==11 || blockk[i]==12 || blockk[i]==14 || blockk[i]==15 || blockk[i]==16 || blockk[i]==17 || blockk[i]==18)
							mx=0;
						else if(blockk[i]==7)
							mx=-mx*0.9f;

						lx=blockx[i]-1.0f;
					}
				}


				//右からあたった
				else if((lx-0.5>=blockx[i]+0.5) && (ly+1>blocky[i]-0.5) && (ly-1<blocky[i]+0.5))
				{
					if(blockk[i]==2 || blockk[i]==7 || blockk[i]==9 || blockk[i]==11 || blockk[i]==12 || blockk[i]==14 || blockk[i]==15 || blockk[i]==16 || blockk[i]==17 || blockk[i]==18)
					{
						if(blockk[i]==2 || blockk[i]==9 || blockk[i]==11 || blockk[i]==12 || blockk[i]==14 || blockk[i]==15 || blockk[i]==16 || blockk[i]==17 || blockk[i]==18)
							mx=0;
						else if(blockk[i]==7)
							mx=-mx*0.9f;

						lx=blockx[i]+1.0f;
					}
				}


				//下からあたった
				else if((ly+1<=blocky[i]-0.5) && (lx+0.5>blockx[i]-0.5) && (lx-0.5<blockx[i]+0.5))
				{
					if(blockk[i]==2 || blockk[i]==7 || blockk[i]==9 || blockk[i]==11 || blockk[i]==12 || blockk[i]==14 || blockk[i]==15 || blockk[i]==16 || blockk[i]==17 || blockk[i]==18)
					{
						if(blockk[i]==2 || blockk[i]==9 || blockk[i]==11 || blockk[i]==12 || blockk[i]==14 || blockk[i]==15 || blockk[i]==16 || blockk[i]==17 || blockk[i]==18)
							my=0;
						else if(blockk[i]==7)
							my=-my*0.9f;

						ly=blocky[i]-1.5f;
					}
				}

				else//斜め
				{
					//if(blockk[i]==16 || blockk[i]==18)
					//{
					//	if (mx>0)
					//		lx=blockx[i]-1.0f;
					//	else
					//		lx=blockx[i]+1.0f;
					//	if (my>0)
					//		ly=blocky[i]-1.5f;
					//	else
					//		ly=blocky[i]+1.5f;
					//	mx=0;
					//	my=0;
					//}
				}

			}

		}
	}


}




	lx+=mx;
	ly+=my;


	if(ropelevel==0)
	{
	ropex[0]=lx;
	ropey[0]=ly;
	}
	else if(ropelevel==3)
	{
	ropex[rope]=lx;
	ropey[rope]=ly;
	}













	if(ropelevel==0)
		suittiok=true;







	//向きの修正
	if(ropelevel==1 || ropelevel==2)
	{
		if(ropex[1]>=lx)
			tune=false;
		else
			tune=true;
	}
	else if(ropelevel==3)
	{
		if(ropex[rope-1]>=lx)
			tune=false;
		else
			tune=true;
	}

	roper*=0.9f;



	cameralx=cameralx+((lx+mx*20)-cameralx)/10+sinf(rad*ropea)*roper;
	cameraly=cameraly+((ly+my*20)-cameraly)/10+cosf(rad*ropea)*roper;

}
else
{

damageanime++;
if(damageanime>=10)
	damageanime=0;

if (goal!=0)
{

	goal++;
	float x,y;
	for(i=0;i<indexmax;i++)
	{
		if(blockk[i]==1)
		{
			x=blockx[i];
			y=blocky[i];
		}
	}

	if(lx<x)
		mx+=0.1f;
	else if(lx>x)
		mx-=0.1f;

	if(ly<y)
		my+=0.1f;
	else if(ly>y)
		my-=0.1f;

	mx*=0.9f;
	my*=0.9f;

	if (goal<50)
	{
	lx=lx+mx;
	ly=ly+my;
	}

	if(ropelevel==0)
	{
	ropex[0]=lx;
	ropey[0]=ly;
	}
	else if(ropelevel==3)
	{
	ropex[rope]=lx;
	ropey[rope]=ly;
	}

	if (goal==50)//カメラ移動
	{
		stagea=rnd(360.0f);
		lx=-sinf(stagea*rad)*100;
		ly=-cosf(stagea*rad)*100;
	}
	if(goal==100)
	{
		if(stage<=101)
		{

			stage++;

			if(stage==101 && kuria==101)
			{
				//全クリ

				pMediaControl->Stop();

				titlezyoutai=0;
				gamezyoutai=1;
				stageselect=stage-1;
				stage=0;
				stagea=rnd(360.0f);
				cameralx=sinf(stagea*rad)*100;
				cameraly=cosf(stagea*rad)*100;
				lx=0;
				ly=0;	
			}
			else
			{
				char c[10];
				_stprintf(c,"%i",stage);
				Load(c);
				cameralx=sinf(stagea*rad)*100;
				cameraly=cosf(stagea*rad)*100;
				goal=0;
			}



			if (kuria<stage && stage<101)
			{
				kuria=stage;

				char name[100]="Data/Stage/Data";

				FILE *fp;
				fp=fopen(name,"w");
				fprintf(fp,"%d\n",kuria);
				//不正チェック用データ付着
				fprintf(fp,"%d\n",(int)(sqrtf((float)kuria*83)*899)+8782%10000);
				fclose(fp);

			}

		}
		else
		{
			//エディットモードなのでセレクトに戻す
			pMediaControl->Stop();

			goal=0;
			titlezyoutai=0;
			gamezyoutai=1;
			stageselect=stage;
			stage=0;
			stagea=rnd(360.0f);
			cameralx=sinf(stagea*rad)*100;
			cameraly=cosf(stagea*rad)*100;
			lx=0;
			ly=0;	
		}


	}


	//腕を戻す
	if(ropelevel==0 && rope!=0)
	{

		float r=sqrtf((ropex[rope]-ropex[rope-1])*(ropex[rope]-ropex[rope-1])+(ropey[rope]-ropey[rope-1])*(ropey[rope]-ropey[rope-1])),x=(ropex[rope]-ropex[rope-1])/r,y=(ropey[rope]-ropey[rope-1])/r;
		if(r<=1)
			rope--;
		else
		{
			ropex[rope]=ropex[rope]-(x);
			ropey[rope]=ropey[rope]-(y);
		}		
		if(rope<0)
			rope=0;	

	}


	cameralx=cameralx+((lx+mx*20)-cameralx)/10+sinf(rad*ropea)*roper;
	cameraly=cameraly+((ly+my*20)-cameraly)/10+cosf(rad*ropea)*roper;

}
else
{


	over++;

	lx=lx+mx;
	ly=ly+my;

	if(ropelevel==0)
	{
	ropex[0]=lx;
	ropey[0]=ly+((float)over)-(((float)over/7)*((float)over/7));
	}
	else if(ropelevel==3)
	{
	ropex[rope]=lx;
	ropey[rope]=ly+((float)over)-(((float)over/7)*((float)over/7));
	}

	//腕を戻す
	if(ropelevel==0 && rope!=0)
	{

		float r=sqrtf((ropex[rope]-ropex[rope-1])*(ropex[rope]-ropex[rope-1])+(ropey[rope]-ropey[rope-1])*(ropey[rope]-ropey[rope-1])),x=(ropex[rope]-ropex[rope-1])/r,y=(ropey[rope]-ropey[rope-1])/r;
		if(r<=1)
			rope--;
		else
		{
			ropex[rope]=ropex[rope]-(x);
			ropey[rope]=ropey[rope]-(y);
		}		
		if(rope<0)
			rope=0;	

	}


	cameralx=cameralx+((lx+mx*20)-cameralx)/10+sinf(rad*ropea)*roper;
	cameraly=cameraly+((ly+my*20)-cameraly)/10+cosf(rad*ropea)*roper;

	if (over>100)
	{
		pMediaControl->Stop();

		over=0;
		titlezyoutai=0;
		gamezyoutai=1;
		stageselect=stage;
		stage=0;
		stagea=rnd(360.0f);
		cameralx=sinf(stagea*rad)*100;
		cameraly=cosf(stagea*rad)*100;
		lx=0;
		ly=0;	
	}
}


}




	}




//	Frame_FrameLateN++;




    return S_OK;
}



//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::Render()
{
//Frame_FrameLateN++;
//追いついていないので飛ばす
//if(1.0f/(float)Frame_FrameLate*(float)Frame_FrameLateN<DXUtil_Timer( TIMER_GETAPPTIME ))return S_OK;

	int i;
	//int j;
	char str[100];

	// Clear the viewport
    m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, RGB(0,0,0), 1.0f, 0L );

	D3DXMATRIX matView;


	D3DXMATRIX matWorld;
	D3DXMATRIX matWorld2;
	
	// Begin the scene
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
        m_pd3dDevice->SetStreamSource( 0, itaVB, sizeof(MYVERTEX) );
        m_pd3dDevice->SetVertexShader( D3DFVF_MYVERTEX );


		
		
		
if(gamezyoutai==0)//Genkiプロジェクト
{		

	if(titlezyoutai>=100 && titlezyoutai<300)
	{
		D3DXVECTOR3 vFromPt( 0, 0, -15);
		D3DXVECTOR3 vLookatPt( 0, 0, 0 );
		D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
		D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
		m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

		m_pd3dDevice->SetTexture( 0, GenProTexture[2] ); 

		D3DXMatrixScaling(&matWorld,30.0f,5.0f,1.0f);
		D3DXMatrixTranslation(&matWorld2,0,5.0f-(float)(titlezyoutai%100)/10.0f-0.5f,0.0f);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		D3DXMatrixScaling(&matWorld,30.0f,5.0f,1.0f);
		D3DXMatrixTranslation(&matWorld2,0,5.0f-(float)((titlezyoutai+50)%100)/10.0f-0.5f,0.0f);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );


		m_pd3dDevice->SetTexture( 0, GenProTexture[1] );
		D3DXMatrixScaling(&matWorld,60.0f,20.0f,1.0f);
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );


		m_pd3dDevice->SetTexture( 0, GenProTexture[3] );
		D3DXMatrixScaling(&matWorld,50.0f,23.0f,1.0f);
		D3DXMatrixTranslation(&matWorld2,23,5.0f,0.0f);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);

		D3DXMatrixRotationZ(&matWorld2,-rad*(10-(titlezyoutai-100)/4.0f));
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		D3DXMatrixTranslation(&matWorld2,-25+(titlezyoutai-100)/3.0f,0.0f,0.0f);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	}
	else if(titlezyoutai>=300 && titlezyoutai<470)
	{



		D3DXVECTOR3 vFromPt( 0, 0, -15);
		D3DXVECTOR3 vLookatPt( 0, 0, 0 );
		D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
		D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
		m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

		m_pd3dDevice->SetTexture( 0, GenProTexture[2] ); 

		D3DXMatrixScaling(&matWorld,30.0f,5.0f,1.0f);
		D3DXMatrixTranslation(&matWorld2,0,5.0f-(float)(titlezyoutai%100)/10.0f-0.5f,0.0f);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		D3DXMatrixScaling(&matWorld,30.0f,5.0f,1.0f);
		D3DXMatrixTranslation(&matWorld2,0,5.0f-(float)((titlezyoutai+50)%100)/10.0f-0.5f,0.0f);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );


		//m_pd3dDevice->SetRenderState( D3DRS_BLENDOP,  D3DBLENDOP_REVSUBTRACT );
		//m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCCOLOR );
		//m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );

		m_pd3dDevice->SetTexture( 0, GenProTexture[1] );
		D3DXMatrixScaling(&matWorld,60.0f,20.0f,1.0f);
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		//m_pd3dDevice->SetRenderState( D3DRS_BLENDOP,  D3DBLENDOP_ADD );
		//m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		//m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );


		m_pd3dDevice->SetTexture( 0, GenProTexture[3] );
		D3DXMatrixScaling(&matWorld,50.0f,23.0f,1.0f);
		D3DXMatrixTranslation(&matWorld2,23,5.0f,0.0f);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);

		D3DXMatrixRotationZ(&matWorld2,-rad*(-30+(titlezyoutai-300)/4.0f));
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		D3DXMatrixTranslation(&matWorld2,28-(titlezyoutai-300)/3.0f,0.0f,0.0f);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );



	}
	else if(titlezyoutai>=500)
	{


		if (titlezyoutai>=743)
		{

			m_pd3dDevice->SetTexture( 0, BackTexture[0] ); 


			D3DXVECTOR3 vFromPt( 0, 0, -15);
			D3DXVECTOR3 vLookatPt( 0, 0, 0 );
			D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
			D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
			m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

			D3DXMatrixScaling(&matWorld,40.0f,30.0f,1.0f);
			m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
			m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		}



		D3DXVECTOR3 vFromPt( 0, 0, -46);
		D3DXVECTOR3 vLookatPt( 0, 0, 0 );
		D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
		D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
		m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

		m_pd3dDevice->SetTexture( 0, GenProTexture[4] );
		D3DXMatrixScaling(&matWorld,10,40,1.0f);
		D3DXMatrixTranslation(&matWorld2,0.0f,35+cosf(rad*((titlezyoutai-500)*1))*60,sinf(rad*((titlezyoutai-500)*1))*50);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	}


}		

else if(gamezyoutai==1)//ゲーム
{



	m_pd3dDevice->SetTexture( 0, BackTexture[0] ); 


	D3DXVECTOR3 vFromPt( 0, 0, -15);
    D3DXVECTOR3 vLookatPt( 0, 0, 0 );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	D3DXMatrixScaling(&matWorld,40.0f,30.0f,1.0f);
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );


	vFromPt= D3DXVECTOR3( cameralx, cameraly, -15);
	vLookatPt=D3DXVECTOR3( cameralx, cameraly, 0 );
	vUpVec=D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	




if(stage==0)
{




    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, true );
	m_pd3dDevice->SetTexture( 0, NULL); 


	D3DXMatrixScaling(&matWorld,0.3f,0.3f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf(axisang),cosf(axisang),0),sinf(axisang2)*40*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,5,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);

	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
    D3DMATERIAL8 mtrl;

    D3DUtil_InitMaterial( mtrl, 1.0f, 0.0f, 0.0f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );
    m_pFont->Render3DText( _T("Stage Select"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );

//	if(stageselect<=100)//普通ステージ
//	{
		
	D3DXMatrixScaling(&matWorld,0.2f,0.2f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)axisang*3.173f),cosf((float)axisang*3.173f),0),sinf((float)axisang2*3.173f)*40*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-3,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);

	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
//    D3DMATERIAL8 mtrl;




	if(stageselect==50)//ラストステージ
	{
    D3DUtil_InitMaterial( mtrl, 1.0f, 0.5f, 0.0f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );
	m_pFont->Render3DText( _T("Last Stage"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );
	}
	else if(stageselect==51)//スタッフロール
	{
    D3DUtil_InitMaterial( mtrl, 0.3f, 0.3f, 0.3f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );
	m_pFont->Render3DText( _T("Stuff Roll"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );
	}
	else if (stageselect<100)
	{
    D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 0.0f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );	_stprintf(str,"%i",stageselect);
	_tcscat( str, " Stage" );
    m_pFont->Render3DText( str, D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );
	}
	else
	{
    D3DUtil_InitMaterial( mtrl, 1.0f,0.0f, 1.0f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );
	char str2[100];
	_stprintf(str,"Edit ");
	_stprintf(str2,"%i",stageselect-101);
	_tcscat( str, str2);
	_tcscat( str," Stage");
    m_pFont->Render3DText( str, D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );		
	}








    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, false );

}
else if(stage==51)//スタッフロール
{
	D3DMATERIAL8 mtrl;

    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, true );







	D3DXMatrixScaling(&matWorld,0.3f,0.3f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)axisang*3.173f),cosf((float)axisang*3.173f),0),sinf((float)axisang2*2.173f)*30*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-25,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 0.3f, 0.3f, 0.3f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Stuff Roll"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );







	D3DXMatrixScaling(&matWorld,0.2f,0.2f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+10)*3.173f),cosf((float)(axisang+10)*3.173f),0),sinf((float)(axisang2+10)*3.173f)*40*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-40,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 1.0f, 0.5f, 0.0f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Game Designer"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );



	D3DXMatrixScaling(&matWorld,0.4f,0.4f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+130)*4.173f),cosf((float)(axisang+130)*4.173f),0),sinf((float)(axisang2+20)*5.173f)*40*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-50,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 0.0f, 0.6f, 0.2f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Genki"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );





	D3DXMatrixScaling(&matWorld,0.2f,0.2f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+10)*7.173f),cosf((float)(axisang+10)*7.173f),0),sinf((float)(axisang2+10)*1.173f)*40*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-70,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 0.0f, 0.4f, 1.0f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Character Designer"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );



	D3DXMatrixScaling(&matWorld,0.4f,0.4f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+130)*4.173f),cosf((float)(axisang+130)*4.173f),0),sinf((float)(axisang2+20)*5.173f)*40*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixRotationZ(&matWorld2,-rad*(sinf(axisang*rad*9.321f)*343.0f-124));
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-80,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 0.0f, 0.6f, 0.2f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Genki"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );





	D3DXMatrixScaling(&matWorld,0.2f,0.2f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+1320)*2.173f),cosf((float)(axisang+1320)*2.173f),0),rad*70);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-100,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 1.0f, 0.2f, 0.4f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Graphic Editer"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );



	D3DXMatrixScaling(&matWorld,0.4f,0.4f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+170)*4.173f),cosf((float)(axisang+170)*4.173f),0),sinf((float)(axisang2+20)*5.173f)*40*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixRotationX(&matWorld2,-rad*(sinf(axisang*rad*29.321f)*543.0f-30));
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-110,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 0.0f, 0.6f, 0.2f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Genki"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );




	D3DXMatrixScaling(&matWorld,0.2f,0.2f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+10)*3.173f),cosf((float)(axisang+10)*3.173f),0),sinf((float)(axisang2+10)*3.173f)*40*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixRotationY(&matWorld2,-rad*(sinf(axisang*rad*3.321f)*543.0f-30));
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-130,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 0.0f, 0.8f, 0.0f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Music Creater"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );








	D3DXMatrixScaling(&matWorld,0.2f,0.2f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+10)*3.173f),cosf((float)(axisang+10)*3.173f),0),sinf((float)(axisang2+10)*3.173f)*40*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixScaling(&matWorld2,sinf(axisang*rad*70)/2.0f+1.0f,sinf(axisang*rad*120.2145f)/2.0f+1.0f,1.0f);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-160,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 0.0f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Sound Editer"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );



	D3DXMatrixScaling(&matWorld,0.4f,0.4f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+130)*4.173f),cosf((float)(axisang+130)*4.173f),0),sinf((float)(axisang2+20)*5.173f)*40*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixRotationX(&matWorld2,rad*axisang*5.924687f);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixRotationY(&matWorld2,-rad*axisang*7.1415f);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixRotationZ(&matWorld2,-rad*axisang*6.1415f);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-170,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 0.0f, 0.6f, 0.2f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Genki"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );




	D3DXMatrixScaling(&matWorld,0.2f,0.2f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+1320)*2.173f),cosf((float)(axisang+1320)*2.173f),0),rad*70);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-190,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 0.6f, 0.6f, 0.6f,0.7f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Stage Creater"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );









	float prodown=160;

	D3DXMatrixScaling(&matWorld,0.4f,0.4f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+10)*3.173f),cosf((float)(axisang+10)*3.173f),0),sinf((float)(axisang2+10)*3.173f)*30*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-200-prodown,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 1.0f, 0.0f, 0.0f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Programmer"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );



	D3DXMatrixScaling(&matWorld,0.7f,0.7f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(2,1,0),-rad*axisang*10+3);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0-10,-230+10-prodown,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 1.0f, 0.3f, 0.3f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Genki"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );



	D3DXMatrixScaling(&matWorld,0.7f,0.7f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(2,-1,0),-rad*axisang*10+8);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0+10,-230+10-prodown,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 0.8f, 0.8f, 0.2f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Genki"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );



	D3DXMatrixScaling(&matWorld,0.7f,0.7f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(-2,1,0),-rad*axisang*10+1);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0-10,-230-10-prodown,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 0.3f, 0.3f, 1.0f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Genki"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );



	D3DXMatrixScaling(&matWorld,0.7f,0.7f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(-2,-1,0),-rad*axisang*10+1422);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0+10,-230-10-prodown,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 0.6f, 0.7f, 0.4f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Genki"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );




	D3DXMatrixScaling(&matWorld,1.0f,1.0f,1.0f);
	D3DXMatrixRotationZ(&matWorld2,-rad*axisang*12.1415f);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-230-prodown,0);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 0.0f, 0.6f, 0.2f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pFont->Render3DText( _T("Genki"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );













	//画像で表示するところはこっち


    m_pd3dDevice->SetStreamSource( 0, itaVB, sizeof(MYVERTEX) );
    m_pd3dDevice->SetVertexShader( D3DFVF_MYVERTEX );


	D3DXMatrixScaling(&matWorld,28.0f,7.0f,1.0f);
	D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+130)*2.173f),cosf((float)(axisang+130)*2.173f),0),30*rad);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixRotationX(&matWorld2,rad*axisang*6.924687f);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixRotationY(&matWorld2,-rad*axisang*5.1415f);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixRotationZ(&matWorld2,-rad*180);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	D3DXMatrixTranslation(&matWorld2,0,-140,0);//-10
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DUtil_InitMaterial( mtrl, 1.0f, 0.0f, 0.0f,0.6f );
    m_pd3dDevice->SetMaterial( &mtrl );

	m_pd3dDevice->SetTexture( 0, NameTexture[0] );//煉獄庭園
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );


	//ここで製作してくれた人全員の名前を挙げる
	//-190以降10間隔

	//最作者リスト

	//Genki
	//千鳥足ｗ
	//oosaki
	//eme
	//ハンター
	//riki
	//マサユキ
	//干しダイコン
	//supirito
	//ハッシー
	//武藤 洋大
	//†エド†
	//ドラゴン
	//侍

	for(i=1;i<=14;i++)
	{


		D3DXMatrixScaling(&matWorld,28.0f,7.0f,1.0f);
		D3DXMatrixRotationAxis(&matWorld2,&D3DXVECTOR3(sinf((float)(axisang+130)*2.173f*(sinf(sqrtf(i*37.0f))/4+1)),cosf((float)(axisang+130)*2.173f*(sinf(sqrtf(i*37.0f))/4+1)),0),30*rad);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		D3DXMatrixRotationX(&matWorld2,rad*axisang*6.924687f+sinf(sqrtf(i*37.0f))*3);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		D3DXMatrixRotationY(&matWorld2,-rad*axisang*5.1415f+sinf(sqrtf(i*23.0f))*3);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		D3DXMatrixRotationX(&matWorld2,-rad*180);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		D3DXMatrixRotationZ(&matWorld2,-rad*180);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		D3DXMatrixTranslation(&matWorld2,0,-190-i*10.0f,0);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

		if(i==1)//Ｇｅｎｋｉは指定
			D3DUtil_InitMaterial( mtrl, 0.0f, 0.6f, 0.2f,0.6f );
		else
			D3DUtil_InitMaterial( mtrl, fmod(i*0.231f,1.0f),fmod(i*0.743f,1.0f),fmod(i*0.894f,1.0f),0.6f );

		m_pd3dDevice->SetMaterial( &mtrl );

		m_pd3dDevice->SetTexture( 0, NameTexture[i] );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );


	}











	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, false );

}
else
{



	if(0>cameralx-21 && 0<cameralx+21 && 0>cameraly-16 && 0<cameraly+16)
	{
		m_pd3dDevice->SetTexture( 0, BlockTexture[0] ); 	
		D3DXMatrixTranslation(&matWorld,0,0,0.0f);
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	}

	for (i=0;i<indexmax;i++)
	{
		if(blockk[i]!=0)
		{
			if(blockx[i]>cameralx-21 && blockx[i]<cameralx+21 && blocky[i]>cameraly-16 && blocky[i]<cameraly+16)
			{
//				m_pd3dDevice->SetTexture( 0, NameTexture[1] ); 	
				m_pd3dDevice->SetTexture( 0, BlockTexture[blockk[i]] ); 	
				D3DXMatrixTranslation(&matWorld,blockx[i],blocky[i],0.0f);
				m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
				m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
//	m_pFont->Render3DText( _T("ABC"), D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );
			}
		}
	}





 









	if(rope!=0)
	{
		if (rope>=1000)
			rope=999;
		MYVERTEX* v;
	    SAFE_RELEASE( senVB );
		senVB=NULL;

		// Create a square grid m_n*m_m for rendering the wall
		if( FAILED( m_pd3dDevice->CreateVertexBuffer( (rope+1)*sizeof(MYVERTEX),
													D3DUSAGE_WRITEONLY, D3DFVF_MYVERTEX,
													D3DPOOL_MANAGED, &senVB ) ) )
		{
			return E_FAIL;
		}

		// Fill in the grid vertex data
		senVB->Lock( 0, 0, (BYTE**)&v, 0 );

		for(i=0;i<=rope;i++)
		{
			v[i].p  = D3DXVECTOR3(ropex[i],ropey[i],0);
			v[i].u  = 0.0f;
			v[i].v  = 0.0f;
		}

		senVB->Unlock();


		m_pd3dDevice->SetStreamSource( 0, senVB, sizeof(MYVERTEX) );
		m_pd3dDevice->SetVertexShader( D3DFVF_MYVERTEX );
		m_pd3dDevice->SetTexture( 0, UdeTexture ); 

		D3DXMatrixTranslation(&matWorld,0,0,0);
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

		m_pd3dDevice->DrawPrimitive( D3DPT_LINESTRIP, 0, rope );

	}


        m_pd3dDevice->SetStreamSource( 0, itaVB, sizeof(MYVERTEX) );
        m_pd3dDevice->SetVertexShader( D3DFVF_MYVERTEX );



if(over!=0 || goal!=0)
{

	if(damageanime<5)
		m_pd3dDevice->SetTexture( 0, BouTexture[0][8] ); 	
	else
		m_pd3dDevice->SetTexture( 0, BouTexture[1][8] ); 	

}
else if(udedamageanime!=0)
{

	if(tune==false)
		m_pd3dDevice->SetTexture( 0, BouTexture[0][9] ); 	
	else
		m_pd3dDevice->SetTexture( 0, BouTexture[1][9] ); 	
}
else
{

	if(rope==0)
	{

		if(jump==false)
		{
			if (anime < 20)
			{

				if(tune==false)
					m_pd3dDevice->SetTexture( 0, BouTexture[0][1] ); 	
				else
					m_pd3dDevice->SetTexture( 0, BouTexture[1][1] ); 	

			}
			else
			{

				if(tune==false)
					m_pd3dDevice->SetTexture( 0, BouTexture[0][2] ); 	
				else
					m_pd3dDevice->SetTexture( 0, BouTexture[1][2] ); 	

			}
		}
		else
		{

			if(tune==false)
				m_pd3dDevice->SetTexture( 0, BouTexture[0][3] ); 	
			else
				m_pd3dDevice->SetTexture( 0, BouTexture[1][3] ); 	

		}

	}
	else
	{





		if(jump==false)
		{
			if (anime < 20)
			{

				if(tune==false)
					m_pd3dDevice->SetTexture( 0, BouTexture[0][6] ); 	
				else
					m_pd3dDevice->SetTexture( 0, BouTexture[1][6] ); 	

			}
			else
			{

				if(tune==false)
					m_pd3dDevice->SetTexture( 0, BouTexture[0][7] ); 	
				else
					m_pd3dDevice->SetTexture( 0, BouTexture[1][7] ); 	

			}
		}
		else
		{

			if(tune==false)
				m_pd3dDevice->SetTexture( 0, BouTexture[0][5] ); 	
			else
				m_pd3dDevice->SetTexture( 0, BouTexture[1][5] ); 	

		}

	}
	

}


if (goal<50)

{

	D3DXMatrixScaling(&matWorld,1.0f/((float)goal/10+1),2.0f/((float)goal/10+1),1.0f);
	D3DXMatrixRotationZ(&matWorld2,-rad*goal*goal);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);				
	D3DXMatrixTranslation(&matWorld2,lx,ly+((float)over)-(((float)over/7)*((float)over/7)),0.0f);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);				
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );










	m_pd3dDevice->SetTexture( 0, HyouzyunTexture );

	D3DXMatrixScaling(&matWorld,2.0f,2.0f,1.0f);
	D3DXMatrixTranslation(&matWorld2,lx+sinf(rad*ropea)*10,ly+cosf(rad*ropea)*10,0.0f);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);				
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

}

//
//
//
//	//てすと
//
////    D3DUtil_InitMaterial( mtrl, 1.0f,0.0f, 1.0f,0.6f );
////    m_pd3dDevice->SetMaterial( &mtrl );
//	D3DXMatrixScaling(&matWorld,0.4f,0.4f,0.4f);
//	D3DXMatrixTranslation(&matWorld2,lx,ly,0.0f);
//	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorld2);				
//	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
//	char strstr[100];
//	sprintf(strstr,"%f",my);
//    m_pFont->Render3DText( strstr, D3DFONT_CENTERED|D3DFONT_TWOSIDED|D3DFONT_FILTERED );		
//


}





}




        m_pd3dDevice->EndScene();
    }

//        m_pFont->DrawText( 2,  0, D3DCOLOR_ARGB(255,255,255,0), m_strFrameStats );
        //m_pFont->DrawText( 2, 20, D3DCOLOR_ARGB(255,255,255,0), m_strDeviceStats );


//		DIJOYSTATE2 js;

//		joystick->Poll(); 
//		joystick->Acquire();
//		joystick->GetDeviceState( sizeof(DIJOYSTATE2), &js );

//_stprintf(str,"%i",js.lX);
//m_pFont->DrawText( 0,  0, D3DCOLOR_ARGB(255,255,255,0), str );

	
return S_OK;

}




//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: This creates all device-dependent managed objects, such as managed
//       textures and managed vertex buffers.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitDeviceObjects()
{
    // Initialize the font's internal textures
    m_pFont->InitDeviceObjects( m_pd3dDevice );


    MYVERTEX* ver;

    // Create a square grid m_n*m_m for rendering the wall
    if( FAILED( m_pd3dDevice->CreateVertexBuffer( 4*sizeof(MYVERTEX),
                                                  D3DUSAGE_WRITEONLY, D3DFVF_MYVERTEX,
                                                  D3DPOOL_MANAGED, &itaVB ) ) )
    {
        return E_FAIL;
    }

    // Fill in the grid vertex data
    itaVB->Lock( 0, 0, (BYTE**)&ver, 0 );

	ver[0].p  = D3DXVECTOR3(-0.5f, 0.5f, 0 );
    ver[0].u  = 0.0f;
    ver[0].v  = 0.0f;

	ver[1].p  = D3DXVECTOR3(0.5f, 0.5f, 0 );
    ver[1].u  = 1.0f;
    ver[1].v  = 0.0f;

	ver[2].p  = D3DXVECTOR3(-0.5f, -0.5f, 0 );
    ver[2].u  = 0.0f;
    ver[2].v  = 1.0f;

	ver[3].p  = D3DXVECTOR3(0.5f, -0.5f, 0 );
    ver[3].u  = 1.0f;
    ver[3].v  = 1.0f;

	itaVB->Unlock();


    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Hyouzyun",&HyouzyunTexture );


    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Back",&BackTexture[0] );

	D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block0",&BlockTexture[0] );
	D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block1",&BlockTexture[1] );
	D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block2",&BlockTexture[2] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block3",&BlockTexture[3] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block4",&BlockTexture[4] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block5",&BlockTexture[5] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block6",&BlockTexture[6] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block7",&BlockTexture[7] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block8",&BlockTexture[8] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block9",&BlockTexture[9] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block10",&BlockTexture[10] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block11",&BlockTexture[11] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block12",&BlockTexture[12] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block13",&BlockTexture[13] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block14",&BlockTexture[14] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block15",&BlockTexture[15] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block16",&BlockTexture[16] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block17",&BlockTexture[17] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block18",&BlockTexture[18] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block19",&BlockTexture[19] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Block20",&BlockTexture[20] );

    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/R1",&BouTexture[0][1] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/R2",&BouTexture[0][2] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/R3",&BouTexture[0][3] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/R4",&BouTexture[0][4] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/R5",&BouTexture[0][5] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/R6",&BouTexture[0][6] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/R7",&BouTexture[0][7] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/R8",&BouTexture[0][8] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/R9",&BouTexture[0][9] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/L1",&BouTexture[1][1] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/L2",&BouTexture[1][2] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/L3",&BouTexture[1][3] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/L4",&BouTexture[1][4] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/L5",&BouTexture[1][5] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/L6",&BouTexture[1][6] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/L7",&BouTexture[1][7] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/L8",&BouTexture[1][8] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/L9",&BouTexture[1][9] );

    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/GenPro1",&GenProTexture[1] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/GenPro2",&GenProTexture[2] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/GenPro3",&GenProTexture[3] );
    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/GenPro4",&GenProTexture[4] );

    D3DUtil_CreateTexture( m_pd3dDevice, "Data/Graphics/Ude",&UdeTexture );



	char filename[MAX_PATH];
	for(int i=0;i<15;i++)
	{
		sprintf(filename,"Data/Graphics/Creater/%d",i);
	    D3DUtil_CreateTexture( m_pd3dDevice, filename,&NameTexture[i] );
	}


    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Restore device-memory objects and state after a device is created or
//       resized.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RestoreDeviceObjects()
{
    // Restore the device objects for the meshes and fonts

    m_pFont->RestoreDeviceObjects();

    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,   false );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, false );
    m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE,   false );
    m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, false );

	//m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
 //   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
 //   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
 //   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
 //   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
 //   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,  D3DTADDRESS_CLAMP );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,  D3DTADDRESS_CLAMP );

   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
   m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
   m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
   m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
   m_pd3dDevice->SetTextureStageState( 2, D3DTSS_COLOROP,   D3DTOP_DISABLE );
   m_pd3dDevice->SetTextureStageState( 2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );



    m_pd3dDevice->SetRenderState( D3DRS_AMBIENT, RGB(255,255,255));

	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_NONE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   true );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/2, 1.33333333333f, 0.000000001f, 100000.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Called when the device-dependent objects are about to be lost.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InvalidateDeviceObjects()
{

    m_pFont->InvalidateDeviceObjects();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::DeleteDeviceObjects()
{
    m_pFont->DeleteDeviceObjects();

	DWORD i;

	SAFE_RELEASE( HyouzyunTexture );

	for( i=0; i<10; i++ )
        SAFE_RELEASE( BackTexture[i] );
	for( i=0; i<30; i++ )
        SAFE_RELEASE( BlockTexture[i] );
	for( i=0; i<10; i++ )
        SAFE_RELEASE( BouTexture[0][i] );
	for( i=0; i<10; i++ )
        SAFE_RELEASE( BouTexture[1][i] );

	for( i=0; i<5; i++ )
        SAFE_RELEASE( GenProTexture[i] );

	SAFE_RELEASE( UdeTexture );

    SAFE_RELEASE( itaVB )
    SAFE_RELEASE( senVB )

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FinalCleanup()
{
    SAFE_DELETE( m_pFont );

    return S_OK;
}



    //aKeys(1) = "DIK_ESCAPE"
    //aKeys(2) = "DIK_1  On main keyboard"
    //aKeys(3) = "DIK_2  On main keyboard"
    //aKeys(4) = "DIK_3  On main keyboard"
    //aKeys(5) = "DIK_4  On main keyboard"
    //aKeys(6) = "DIK_5  On main keyboard"
    //aKeys(7) = "DIK_6  On main keyboard"
    //aKeys(8) = "DIK_7  On main keyboard"
    //aKeys(9) = "DIK_8  On main keyboard"
    //aKeys(10) = "DIK_9  On main keyboard"
    //aKeys(11) = "DIK_0  On main keyboard"
    //aKeys(12) = "DIK_MINUS  On main keyboard"
    //aKeys(13) = "DIK_EQUALS  On main keyboard"
    //aKeys(14) = "DIK_BACK BACKSPACE"
    //aKeys(15) = "DIK_TAB"
    //aKeys(16) = "DIK_Q"
    //aKeys(17) = "DIK_W"
    //aKeys(18) = "DIK_E"
    //aKeys(19) = "DIK_R"
    //aKeys(20) = "DIK_T"
    //aKeys(21) = "DIK_Y"
    //aKeys(22) = "DIK_U"
    //aKeys(23) = "DIK_I"
    //aKeys(24) = "DIK_O"
    //aKeys(25) = "DIK_P"
    //aKeys(26) = "DIK_LBRACKET  ["
    //aKeys(27) = "DIK_RBRACKET  ]"
    //aKeys(28) = "DIK_RETURN  ENTER on main keyboard"
    //aKeys(29) = "DIK_LCONTROL  Left CTRL Key"
    //aKeys(30) = "DIK_A"
    //aKeys(31) = "DIK_S"
    //aKeys(32) = "DIK_D"
    //aKeys(33) = "DIK_F"
    //aKeys(34) = "DIK_G"
    //aKeys(35) = "DIK_H"
    //aKeys(36) = "DIK_J"
    //aKeys(37) = "DIK_K"
    //aKeys(38) = "DIK_L"
    //aKeys(39) = "DIK_SEMICOLON"
    //aKeys(40) = "DIK_APOSTROPHE"
    //aKeys(41) = "DIK_GRAVE  Grave accent (`)"
    //aKeys(42) = "DIK_LSHIFT  Left SHIFT"
    //aKeys(43) = "DIK_BACKSLASH"
    //aKeys(44) = "DIK_Z"
    //aKeys(45) = "DIK_X"
    //aKeys(46) = "DIK_C"
    //aKeys(47) = "DIK_V"
    //aKeys(48) = "DIK_B"
    //aKeys(49) = "DIK_N"
    //aKeys(50) = "DIK_M"
    //aKeys(51) = "DIK_COMMA"
    //aKeys(52) = "DIK_PERIOD  On main keyboard"
    //aKeys(53) = "DIK_SLASH  Forward slash (/)on main keyboard"
    //aKeys(54) = "DIK_RSHIFT  Right SHIFT"
    //aKeys(55) = "DIK_MULTIPLY  Asterisk on numeric keypad"
    //aKeys(56) = "DIK_LMENU  Left ALT"
    //aKeys(57) = "DIK_SPACE Spacebar"
    //aKeys(58) = "DIK_CAPITAL  CAPS LOCK"
    //aKeys(59) = "DIK_F1"
    //aKeys(60) = "DIK_F2"
    //aKeys(61) = "DIK_F3"
    //aKeys(62) = "DIK_F4"
    //aKeys(63) = "DIK_F5"
    //aKeys(64) = "DIK_F6"
    //aKeys(65) = "DIK_F7"
    //aKeys(66) = "DIK_F8"
    //aKeys(67) = "DIK_F9"
    //aKeys(68) = "DIK_F10"
    //aKeys(69) = "vDIK_NUMLOCK"
    //aKeys(70) = "DIK_SCROLL  SCROLL LOCK"
    //aKeys(71) = "DIK_NUMPAD7"
    //aKeys(72) = "DIK_NUMPAD8"
    //aKeys(73) = "DIK_NUMPAD9"
    //aKeys(74) = "DIK_SUBTRACT  Hyphen (minus sign) on numeric keypad"
    //aKeys(75) = "DIK_NUMPAD4"
    //aKeys(76) = "DIK_NUMPAD5"
    //aKeys(77) = "DIK_NUMPAD6"
    //aKeys(78) = "DIK_ADD  Plus sign on numeric keypad"
    //aKeys(79) = "DIK_NUMPAD1"
    //aKeys(80) = "DIK_NUMPAD2"
    //aKeys(81) = "DIK_NUMPAD3"
    //aKeys(82) = "DIK_NUMPAD0"
    //aKeys(83) = "DIK_DECIMAL  Period (decimal point) on numeric keypad"
    //aKeys(87) = "DIK_F11"
    //aKeys(88) = "DIK_F12"
    //aKeys(86) = "DIK_F13"
    //aKeys(84) = "DIK_F14"
    //aKeys(85) = "DIK_F15"
    //aKeys(156) = "DIK_NUMPADENTER"
    //aKeys(157) = "DIK_RCONTROL  Right CTRL key"
    //aKeys(91) = "DIK_NUMPADCOMMA Comma on NEC PC98 numeric keypad"
    //aKeys(181) = "DIK_DIVIDE  Forward slash (/)on numeric keypad"
    //aKeys(183) = "DIK_SYSRQ"
    //aKeys(184) = "DIK_RMENU  Right ALT"
    //aKeys(199) = "DIK_HOME"
    //aKeys(200) = "DIK_UP  Up arrow"
    //aKeys(201) = "DIK_PRIOR  PAGE UP"
    //aKeys(203) = "DIK_LEFT  Left arrow"
    //aKeys(205) = "DIK_RIGHT  Right arrow"
    //aKeys(207) = "DIK_END"
    //aKeys(208) = "DIK_DOWN  Down arrow"
    //aKeys(209) = "DIK_NEXT  PAGE DOWN"
    //aKeys(210) = "DIK_INSERT"
    //aKeys(211) = "DIK_DELETE"
    //aKeys(219) = "DIK_LWIN  Left Windows key"
    //aKeys(220) = "DIK_RWIN  Right Windows key"
    //aKeys(221) = "DIK_APPS  Application key"
    //aKeys(116) = "DIK_PAUSE"