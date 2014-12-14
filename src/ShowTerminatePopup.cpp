/*
 * ShowTerminatePopup.cpp
 *
 *  Created on: Dec 8, 2014

 */

#include "ShowTerminatePopup.h"
#include <FApp.h>
#include <FMedia.h>

using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui;
using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Media;

bool PopupEventListenerTerminate::OnKeyReleased(Control& source,
		const KeyEventInfo& keyEventInfo) {
	KeyCode key = keyEventInfo.GetKeyCode();
	if (key == KEY_BACK || key == KEY_ESC) {
		Popup* pPopup_Terminate = static_cast<Popup *>(&source);
		pPopup_Terminate->SetShowState(false);
		pPopup_Terminate->Invalidate(true);
	}
	return false;
}

ShowTerminatePopup::ShowTerminatePopup() :
		__pPopup_Terminate(null) {
	result r = E_SUCCESS;
	__pPopupListener_Terminate = new PopupEventListenerTerminate;

	__pPopup_Terminate = new Popup();
	if (__pPopup_Terminate == null)
		AppLog("Popup Error");
	__pPopup_Terminate->Construct(false, Dimension(600, 220));

	Label* pLabel = new Label();
	if (pLabel == null)
		AppLog("Label Error");
	pLabel->Construct(Rectangle(25, 25, 550, 80), L"종료하시겠습니까?");

	// Creates an instance of Button to Exit Program
	Button* pTerminateButton = new Button();
	if (pTerminateButton == null)
		AppLog("Btn1 Error");
	pTerminateButton->Construct(Rectangle(25, 115, 250, 80), L"Exit");
	pTerminateButton->SetActionId(ID_BUTTON_TERMINATE);
	pTerminateButton->AddActionEventListener(*this);

	// Creates an instance of Button to cancel the popup.
	Button* pCancelButton = new Button();
	if (pCancelButton == null)
		AppLog("Btn2 Error");
	pCancelButton->Construct(Rectangle(320, 115, 250, 80), L"Cancel");
	pCancelButton->SetActionId(ID_BUTTON_CANCEL);
	pCancelButton->AddActionEventListener(*this);

	// Adds the button to the popup
	__pPopup_Terminate->AddControl(pCancelButton);
	__pPopup_Terminate->AddControl(pTerminateButton);
	__pPopup_Terminate->AddControl(pLabel);
	__pPopup_Terminate->SetPropagatedKeyEventListener(
			__pPopupListener_Terminate);
}

ShowTerminatePopup::~ShowTerminatePopup() {
	// Deallocates the __pPopup
	__pPopup_Terminate->Destroy();
	delete __pPopupListener_Terminate;
}

void ShowTerminatePopup::ShowPopup(void) {
	AppLog("HIHIHIHIHIHIHI");
	__pPopup_Terminate->SetShowState(true);
	__pPopup_Terminate->Show();
}

void ShowTerminatePopup::HidePopup(void) {
	__pPopup_Terminate->SetShowState(false);
}

void ShowTerminatePopup::OnActionPerformed(const Control& source,
		int actionId) {
	switch (actionId) {
	case ID_BUTTON_TERMINATE: {
		AppLog("Terminate!");
		UiApp* pApp = UiApp::GetInstance();
		AppAssert(pApp);
		pApp->Terminate();
		//            ShowPopup();
	}
		break;

	case ID_BUTTON_CANCEL: {
		AppLog("Cancel!");
		HidePopup();
	}
		break;

	default:
		break;
	}
}
