#include "BrowserMainForm.h"
#include "AppResourceId.h"
#include "SceneRegister.h"
#include <FMedia.h>
#include <FApp.h>

#include "BrowserTab1.h"
#include "BrowserTab2.h"
#include "ListViewItemPopup.h"
#include "ShowTerminatePopup.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Media;
using namespace Tizen::Graphics;

BrowserMainForm::BrowserMainForm(void) {
}

BrowserMainForm::~BrowserMainForm(void) {
}

bool BrowserMainForm::Initialize(void) {
	result r = Construct(IDF_FILE_BROWSER_FORM);
	TryReturn(r == E_SUCCESS, false, "Failed to construct form");

	return true;
}

result BrowserMainForm::OnInitializing(void) {
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	Header* pHeader = GetHeader();
	if (pHeader) {
		pHeader->AddActionEventListener(*this);
	}

	// Setup back event listener
	//SetFormBackEventListener(reinterpret_cast<BrowserTab1*>(GetControl(IDL_MANAGEMENT_PANEL)));
	//SetFormBackEventListener(reinterpret_cast<BrowserTab2*>(GetControl(IDL_HIDDEN_FILES_PANEL)));

	SetFormBackEventListener(this);

	Image pimage;
	r = pimage.Construct();
	String filepath = App::GetInstance()->GetAppDataPath();
	__pTizenBitmap = pimage.DecodeN(filepath + L"icon_filebrowser.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);

	// Set-up header
	AppAssert(pHeader);
	pHeader->SetTitleIcon(__pTizenBitmap);

	//Menu part
	/**/
	SetFormMenuEventListener(this);
	__pOption = new (std::nothrow) OptionMenu();
	__pOption->Construct();
	__pOption->AddItem(L"Set Key Expresstion", IDA_CONTEXT_ITEM);
	__pOption->AddActionEventListener(*this);
	__pOption->SetShowState(false);
	//*/

	return r;
}

result BrowserMainForm::OnTerminating(void) {
	result r = E_SUCCESS;

	// TODO: Add your termination code here
	return r;
}

void BrowserMainForm::OnActionPerformed(const Tizen::Ui::Control& source,
		int actionId) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	switch (actionId) {
	case ID_HEADER_ITEM1:
		AppLog("ITME1");
		pSceneManager->GoForward(
				ForwardSceneTransition(SCENE_FILEBROWSER_MANAGEMENT));
		break;
	case ID_HEADER_ITEM2:
		AppLog("ITME2");
		pSceneManager->GoForward(
				ForwardSceneTransition(SCENE_FILEBROWSER_HIDDEN));
		break;
	case IDA_CONTEXT_ITEM:
		pSceneManager->GoForward(
				ForwardSceneTransition(SCENE_SETTING,
						SCENE_TRANSITION_ANIMATION_TYPE_FADE_IN_OUT,
						SCENE_HISTORY_OPTION_NO_HISTORY,
						SCENE_DESTROY_OPTION_KEEP));
		break;
	default:
		break;
	}
}

void BrowserMainForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	AppLog("BackBack Form");
//	UiApp* pApp = UiApp::GetInstance();
//	AppAssert(pApp);
//	pApp->Terminate();
	terimPopup.ShowPopup();
}

void BrowserMainForm::OnFormMenuRequested(Tizen::Ui::Controls::Form& source) {
	__pOption->SetShowState(true);
	__pOption->Show();
}

void BrowserMainForm::OnSceneActivatedN(
		const Tizen::Ui::Scenes::SceneId& previousSceneId,
		const Tizen::Ui::Scenes::SceneId& currentSceneId,
		Tizen::Base::Collection::IList* pArgs) {

}

void BrowserMainForm::OnSceneDeactivated(
		const Tizen::Ui::Scenes::SceneId& currentSceneId,
		const Tizen::Ui::Scenes::SceneId& nextSceneId) {
}

