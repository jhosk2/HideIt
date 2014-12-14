#include "BrowserTab2.h"
#include "AppResourceId.h"
#include "FolderBrowser.h"

using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

BrowserTab2::BrowserTab2(void) {
	AppLog("Tap2 Creating");
}

BrowserTab2::~BrowserTab2(void) {

}

bool BrowserTab2::Initialize(void) {
	result r = Construct(IDL_HIDDEN_FILES_PANEL);
	if (IsFailed(r)) {
		return false;
	}

	return true;
}

result BrowserTab2::OnInitializing(void) {
	result r = E_SUCCESS;

	// Layout setting
	const Form* pForm = dynamic_cast<Form*>(GetParent());
//	if (pForm) {
//		RelativeLayout* pRelativeLayout =
//				dynamic_cast<RelativeLayout*>(pForm->GetLandscapeLayoutN());
//		if (pRelativeLayout) {
//			pRelativeLayout->SetHorizontalFitPolicy(*this, FIT_POLICY_PARENT);
//			pRelativeLayout->SetVerticalFitPolicy(*this, FIT_POLICY_PARENT);
//			delete pRelativeLayout;
//		}
//		pRelativeLayout =
//				dynamic_cast<RelativeLayout*>(pForm->GetPortraitLayoutN());
//		if (pRelativeLayout) {
//			pRelativeLayout->SetHorizontalFitPolicy(*this, FIT_POLICY_PARENT);
//			pRelativeLayout->SetVerticalFitPolicy(*this, FIT_POLICY_PARENT);
//			delete pRelativeLayout;
//		}
//	}
	pForm->GetHeader()->SetItemSelected(1);
	IconListView* listview = dynamic_cast<IconListView*>(GetControl(
			IDC_ICONLISTVIEW_HIDDENFILES));
	FILE_STATE_FLAG = FILESTATE_ENCRYPTION;
	folderBrowser = new FolderBrowser(listview);
	folderBrowser->ToHideItFolder();


	listview->SetItemProvider(*folderBrowser);
	listview->AddIconListViewItemEventListener(*folderBrowser);

	r = AddLabel();
	TryReturn(!IsFailed(r), r,
			"AddLabel() failed with [%s]", GetErrorMessage(r));

	return r;
}

result BrowserTab2::AddLabel(void) {
	result r = E_SUCCESS;

	__pLabel_All = static_cast<Label*>(GetControl(IDC_HIDDEN_LABEL_ALL, true));
	TryReturn(__pLabel_All != null, r = E_SYSTEM, "Panel::GetControl() failed");
	__pLabel_All->AddTouchEventListener(*this);

	__pLabel_Image = static_cast<Label*>(GetControl(IDC_HIDDEN_LABEL_IMAGE,
			true));
	TryReturn(__pLabel_Image != null, r = E_SYSTEM,
			"Panel::GetControl() failed");
	__pLabel_Image->AddTouchEventListener(*this);

	__pLabel_Video = static_cast<Label*>(GetControl(IDC_HIDDEN_LABEL_VIDEO,
			true));
	TryReturn(__pLabel_Video != null, r = E_SYSTEM,
			"Panel::GetControl() failed");
	__pLabel_Video->AddTouchEventListener(*this);

	__pLabel_Etc = static_cast<Label*>(GetControl(IDC_HIDDEN_LABEL_ETC, true));
	TryReturn(__pLabel_Etc != null, r = E_SYSTEM, "Panel::GetControl() failed");
	__pLabel_Etc->AddTouchEventListener(*this);

	Color* color = new Color(18, 102, 255, 255);
	__pLabel_All->SetBackgroundColor(*color);

	return r;
}

result BrowserTab2::OnTerminating(void) {
	result r = E_SUCCESS;

	// TODO: Add your termination code here

	delete folderBrowser;
	AppLog("Tap2 Terminating");
	return r;
}

void BrowserTab2::OnSceneActivatedN(
		const Tizen::Ui::Scenes::SceneId& previousSceneId,
		const Tizen::Ui::Scenes::SceneId& currentSceneId,
		Tizen::Base::Collection::IList* pArgs) {
	// TODO: Activate your scene here.
	FILE_STATE_FLAG = FILESTATE_ENCRYPTION;
	sensorProxy = new ProxySensor();
	folderBrowser->Refresh();

}

void BrowserTab2::OnSceneDeactivated(
		const Tizen::Ui::Scenes::SceneId& currentSceneId,
		const Tizen::Ui::Scenes::SceneId& nextSceneId) {
	// TODO: Deactivate your scene here.
	AppLog("Tap2 Deactivated");
	sensorProxy->~ProxySensor();
}

void BrowserTab2::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	/*
	 if ( !folderBrowser->GotoParent() )
	 {
	 //UiApp* pApp = UiApp::GetInstance();
	 //AppAssert(pApp);
	 //pApp->Terminate();
	 }*/

	AppLog("BackBack Tab2 ");

	//sensorProxy->~ProxySensor();
	terpopup.ShowPopup();
}

void BrowserTab2::OnTouchMoved(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {
}

void BrowserTab2::OnTouchPressed(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {
	Color* color = new Color(18, 102, 255, 255);
	Color* colorOriginal = new Color(112, 112, 112, 255);
	__pLabel_All->SetBackgroundColor(*colorOriginal);
	__pLabel_Image->SetBackgroundColor(*colorOriginal);
	__pLabel_Video->SetBackgroundColor(*colorOriginal);
	__pLabel_Etc->SetBackgroundColor(*colorOriginal);
	if (source.GetName() == IDC_HIDDEN_LABEL_ALL) {
		__pLabel_All->SetBackgroundColor(*color);
	} else if (source.GetName() == IDC_HIDDEN_LABEL_IMAGE) {
		__pLabel_Image->SetBackgroundColor(*color);
	} else if (source.GetName() == IDC_HIDDEN_LABEL_VIDEO) {
		__pLabel_Video->SetBackgroundColor(*color);
	} else if (source.GetName() == IDC_HIDDEN_LABEL_ETC) {
		__pLabel_Etc->SetBackgroundColor(*color);
	}
	Invalidate(true);
}

void BrowserTab2::OnTouchReleased(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {
	Color* color = new Color(112, 112, 112, 255);
	if (source.GetName() == IDC_HIDDEN_LABEL_ALL) {
		folderBrowser->SetFilter(H_ALL);
		//__pLabel_All->SetBackgroundColor(*color);
		//
		//folderBrowser->ToHideItFolder();
	} else if (source.GetName() == IDC_HIDDEN_LABEL_IMAGE) {
		folderBrowser->SetFilter(H_IMAGE);
		//__pLabel_Image->SetBackgroundColor(*color);
		//folderBrowser->OtherFolderInHideItFoler(L"/Image/");
	} else if (source.GetName() == IDC_HIDDEN_LABEL_VIDEO) {
		folderBrowser->SetFilter(H_VIDEO);
		//__pLabel_Video->SetBackgroundColor(*color);
		//folderBrowser->OtherFolderInHideItFoler(L"/Video/");
	} else if (source.GetName() == IDC_HIDDEN_LABEL_ETC){
		folderBrowser->SetFilter(H_ETC);
		//__pLabel_Etc->SetBackgroundColor(*color);
		//folderBrowser->OtherFolderInHideItFoler(L"/Etc/");
	}

	Invalidate(true);
}

void BrowserTab2::OnTouchFocusIn(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {
}

void BrowserTab2::OnTouchFocusOut(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {
}

