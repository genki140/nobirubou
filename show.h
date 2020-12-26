#ifndef SHOW_HED
#define SHOW_HED



#include <dshow.h>

IGraphBuilder *pGraph = NULL;
IMediaControl *pMediaControl = NULL;
IMediaSeeking *pMediaSeeking = NULL;
IMediaEvent   *pEvent = NULL;






void ShowRelease()
{

	if(pMediaControl!=NULL){pMediaControl->Release();pMediaControl=NULL;}
	if(pMediaSeeking!=NULL){pMediaSeeking->Release();pMediaSeeking=NULL;}	
	if(pEvent!=NULL){pEvent->Release();pEvent=NULL;}
	if(pGraph!=NULL){pGraph->Release();pGraph=NULL;}
	CoUninitialize();

}




bool ShowSetting()
{

	// クリーン アップ。
	ShowRelease();

	CoInitialize(NULL);
	
	// フィルタグラフマネージャを作成し、インターフェイスをクエリする。
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
						IID_IGraphBuilder, (void **)&pGraph);
	pGraph->QueryInterface(IID_IMediaControl, (void **)&pMediaControl);
	pGraph->QueryInterface(IID_IMediaSeeking, (void **)&pMediaSeeking);
	pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);

	pGraph->RenderFile(L"data/sound/music", NULL);



	return true;

}





#endif
