#include "BrowserTab1.h"
#include "AppResourceId.h"
#include "FolderBrowser.h"
#include "FolderBrowser.h"

using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::App;

BrowserTab1::BrowserTab1(void) {
	AppLog("Tap1 Creating");
}

BrowserTab1::~BrowserTab1(void) {

}

bool BrowserTab1::Initialize(void) {
	result r = Construct(IDL_MANAGEMENT_PANEL);
	if (IsFailed(r)) {
		return false;
	}
	return true;
}

result BrowserTab1::OnInitializing(void) {
	result r = E_SUCCESS;

	AppLog("Tap1 Init");
	// Layout setting
	const Form* pForm = dynamic_cast<Form*>(GetParent());
	if (pForm) {
		RelativeLayout* pRelativeLayout =
				dynamic_cast<RelativeLayout*>(pForm->GetLandscapeLayoutN());
		if (pRelativeLayout) {
			pRelativeLayout->SetHorizontalFitPolicy(*this, FIT_POLICY_PARENT);
			pRelativeLayout->SetVerticalFitPolicy(*this, FIT_POLICY_PARENT);
			delete pRelativeLayout;
		}
		pRelativeLayout =
				dynamic_cast<RelativeLayout*>(pForm->GetPortraitLayoutN());
		if (pRelativeLayout) {
			pRelativeLayout->SetHorizontalFitPolicy(*this, FIT_POLICY_PARENT);
			pRelativeLayout->SetVerticalFitPolicy(*this, FIT_POLICY_PARENT);
			delete pRelativeLayout;
		}
	}

	__pLabel_Back = static_cast<Label*>(GetControl(IDC_MANAGEMENT_LABEL_BACK,
			true));
	TryReturn(__pLabel_Back != null, r = E_SYSTEM,
			"Panel::GetControl() failed");
	__pLabel_Back->AddTouchEventListener(*this);

	Label* __pLabel = static_cast<Label*>(GetControl(IDC_MANAGEMENT_LABEL_PATH));
	TryReturn(__pLabel != null, r = E_SYSTEM, "Panel::GetControl() failed");

	IconListView* listview = dynamic_cast<IconListView*>(GetControl(
			IDC_MANAGEMENT_ICONLISTVIEW));

	FILE_STATE_FLAG = FILESTATE_DECRYPTION;
	folderBrowser = new FolderBrowser(listview, __pLabel);

	listview->SetItemProvider(*folderBrowser);
	listview->AddIconListViewItemEventListener(*folderBrowser);

	return r;
}

result BrowserTab1::OnTerminating(void) {
	result r = E_SUCCESS;

	// TODO: Add your termination code here
	delete folderBrowser;
	AppLog("Tap1 Terminating");

	return r;
}

void BrowserTab1::OnSceneActivatedN(
		const Tizen::Ui::Scenes::SceneId& previousSceneId,
		const Tizen::Ui::Scenes::SceneId& currentSceneId,
		Tizen::Base::Collection::IList* pArgs) {
	// TODO: Activate your scene here.
	FILE_STATE_FLAG = FILESTATE_DECRYPTION;
	sensorProxy = new ProxySensor();
	folderBrowser->Refresh();

}

void BrowserTab1::OnSceneDeactivated(
		const Tizen::Ui::Scenes::SceneId& currentSceneId,
		const Tizen::Ui::Scenes::SceneId& nextSceneId) {
	// TODO: Deactivate your scene here.
	AppLog("Tap1 Deactivated");
	sensorProxy->~ProxySensor();
}

void BrowserTab1::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	/*
	AppLog("BackBack");
	UiApp* pApp = UiApp::GetInstance();
	AppAssert(pApp);
	pApp->Terminate();
	 */
	AppLog("BackBack Tab1 ");
	terpopup.ShowPopup();
	//sensorProxy->~ProxySensor();
}

void BrowserTab1::OnTouchMoved(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {
}

void BrowserTab1::OnTouchPressed(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {

	Color* color = new Color(18, 102, 255, 255);
	if (source.GetName() == IDC_MANAGEMENT_LABEL_BACK) {
		__pLabel_Back->SetBackgroundColor(*color);
	}
	Invalidate(true);
}

void BrowserTab1::OnTouchReleased(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {

	Color* color = new Color(112, 112, 112, 255);

	if (source.GetName() == IDC_MANAGEMENT_LABEL_BACK) {
		__pLabel_Back->SetBackgroundColor(*color);
		folderBrowser->GotoParent();
	}
	Invalidate(true);

}

void BrowserTab1::OnTouchFocusIn(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {
}

void BrowserTab1::OnTouchFocusOut(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {
}

