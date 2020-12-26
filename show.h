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

	// �N���[�� �A�b�v�B
	ShowRelease();

	CoInitialize(NULL);
	
	// �t�B���^�O���t�}�l�[�W�����쐬���A�C���^�[�t�F�C�X���N�G������B
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
						IID_IGraphBuilder, (void **)&pGraph);
	pGraph->QueryInterface(IID_IMediaControl, (void **)&pMediaControl);
	pGraph->QueryInterface(IID_IMediaSeeking, (void **)&pMediaSeeking);
	pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);

	pGraph->RenderFile(L"data/sound/music", NULL);



	return true;

}





#endif
