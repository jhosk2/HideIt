#include "SettingPasswordForm.h"
#include "AppResourceId.h"
#include "SceneRegister.h"
#include "ListViewItemPopup.h"

#include <FMedia.h>
#include <FApp.h>

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Media;
using namespace Tizen::Graphics;
using namespace Tizen::Io;

static const String ERR_INPUT = "Valid Input : 0 to 9 with . + - * / ( )";
static const String ERR_DIFFERENT_PASSWORD = "Passwords are NOT the same";

bool pErr_Flag = true;

SettingPasswordForm::SettingPasswordForm(void) {
}

SettingPasswordForm::~SettingPasswordForm(void) {
}

bool SettingPasswordForm::Initialize(void) {
	result r = Construct(IDF_SETTING_FORM);
	TryReturn(r == E_SUCCESS, false, "Failed to construct form");

	return true;
}
result SettingPasswordForm::OnInitializing(void) {
	result r = E_SUCCESS;

	// TODO: Add your initialization code here

	Image image;
	r = image.Construct();
	String filepath = App::GetInstance()->GetAppDataPath();
	__pTizenBitmap = image.DecodeN(filepath + L"icon_setting.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);

	// Set-up header
	Header* pHeader = GetHeader();
	AppAssert(pHeader);
	pHeader->SetTitleIcon(__pTizenBitmap);

	// Setup back event listener
	SetFormBackEventListener(this);

	Button* pButton = static_cast<Button*>(GetControl(IDC_BUTTON_CONFIRM, true));
	pButton->SetActionId(ID_BUTTON_CONFIRM);
	pButton->AddActionEventListener(*this);

	pEditField_Input = static_cast<EditField*>(GetControl(IDC_EDITFIELD_INPUT,
			true));
	pEditField_Input->AddTextEventListener(*this);

	pEditField_Re_Input = static_cast<EditField*>(GetControl(
			IDC_EDITFIELD_RE_INPUT, true));
	pEditField_Re_Input->AddTextEventListener(*this);

	pLabel_Err = static_cast<Label*>(GetControl(IDC_LABEL_ERR, true));
	pLabel_Err->SetShowState(false);

	return r;
}

result SettingPasswordForm::OnTerminating(void) {
	result r = E_SUCCESS;

	// TODO: Add your termination code here

	return r;
}

void SettingPasswordForm::OnActionPerformed(const Tizen::Ui::Control& source,
		int actionId) {
	switch (actionId) {
	case ID_BUTTON_CONFIRM:
		SettingPassword();
		break;
	default:
		break;
	}
}

void SettingPasswordForm::OnFormBackRequested(
		Tizen::Ui::Controls::Form& source) {

	if (FILE_STATE_FLAG == FILESTATE_DECRYPTION) {
		SceneManager* pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(
				ForwardSceneTransition(SCENE_FILEBROWSER_MANAGEMENT));
	}

	else {
		SceneManager* pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(
				ForwardSceneTransition(SCENE_FILEBROWSER_HIDDEN));
	}

}

void SettingPasswordForm::SettingPassword(void) {

	String pPassword = pEditField_Input->GetText();

	if (pPassword == pEditField_Re_Input->GetText()) {
		AppRegistry *registry = Application::GetInstance()->GetAppRegistry();
		registry->Set("KEY_EXPRESSTION", pPassword);
		registry->Save();

		SceneManager* pSceneManager = SceneManager::GetInstance();
		AppAssert(pSceneManager);
		pSceneManager->GoForward(
				ForwardSceneTransition(SCENE_FILEBROWSER_MANAGEMENT));
	} else {
		pEditField_Input->SetText("");
		pEditField_Re_Input->SetText("");
		pLabel_Err->SetText(ERR_DIFFERENT_PASSWORD);
		pLabel_Err->SetShowState(true);
		Invalidate(true);
		pErr_Flag = false;
	}
}

void SettingPasswordForm::OnTextValueChanged(const Tizen::Ui::Control& source) {

	String pCheck;
	wchar_t temp;

	if (!pErr_Flag) {
		pLabel_Err->SetText(ERR_INPUT);
		pErr_Flag = true;
	}

	if (source.GetName() == "IDC_EDITFIELD_INPUT") {
		pCheck = pEditField_Input->GetText();

		if (pCheck == "")
			return;

		pCheck.GetCharAt(pCheck.GetLength() - 1, temp);
		if (!((temp >= '0' && temp <= '9') || temp == '.' || temp == '+'
				|| temp == '*' || temp == '-' || temp == '/' || temp == '('
				|| temp == ')')) {
			pCheck.Remove(pCheck.GetLength() - 1, 1);
			pEditField_Input->SetText(pCheck);
			pLabel_Err->SetShowState(true);
			Invalidate(true);
		} else {
			pLabel_Err->SetShowState(false);
		}
	} else {
		pCheck = pEditField_Re_Input->GetText();

		if (pCheck == "")
			return;

		pCheck.GetCharAt(pCheck.GetLength() - 1, temp);

		if (!((temp >= '0' && temp <= '9') || temp == '.' || temp == '+'
				|| temp == '*' || temp == '-' || temp == '/' || temp == '('
				|| temp == ')')) {
			pCheck.Remove(pCheck.GetLength() - 1, 1);
			pEditField_Re_Input->SetText(pCheck);
			pLabel_Err->SetShowState(true);
			Invalidate(true);
		} else {
			pLabel_Err->SetShowState(false);
		}
	}
}
void SettingPasswordForm::OnTextValueChangeCanceled(
		const Tizen::Ui::Control& source) {
}

void SettingPasswordForm::OnSceneActivatedN(
		const Tizen::Ui::Scenes::SceneId& previousSceneId,
		const Tizen::Ui::Scenes::SceneId& currentSceneId,
		Tizen::Base::Collection::IList* pArgs) {
}

void SettingPasswordForm::OnSceneDeactivated(
		const Tizen::Ui::Scenes::SceneId& currentSceneId,
		const Tizen::Ui::Scenes::SceneId& nextSceneId) {
	pEditField_Input->SetText("");
	pEditField_Re_Input->SetText("");
	Invalidate(true);
}

