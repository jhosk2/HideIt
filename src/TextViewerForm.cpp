#include "TextViewerForm.h"
#include "AppResourceId.h"
#include "SceneRegister.h"

#include<FApp.h>
#include<FBase.h>
#include<FSystem.h>
#include<FUi.h>
#include<FIo.h>
#include <FMedia.h>
#include "ProxySensor.h"

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Io;
using namespace Tizen::Media;
using namespace Tizen::Graphics;

TextViewerForm::TextViewerForm(void) {
}

TextViewerForm::~TextViewerForm(void) {
}

bool TextViewerForm::Initialize(void) {
	result r = Construct(IDF_TEXTVIEWER_FORM);
	TryReturn(r == E_SUCCESS, false, "Failed to construct form");

	return true;
}

result TextViewerForm::OnInitializing(void) {
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	Header* pHeader = GetHeader();
	if (pHeader) {
		Image pimage;
		r = pimage.Construct();
		String filepath = App::GetInstance()->GetAppDataPath();
		__pTizenBitmap = pimage.DecodeN(filepath + L"icon_textviewer.png",
				BITMAP_PIXEL_FORMAT_ARGB8888);

		// Set-up header
		AppAssert(pHeader);
		pHeader->SetTitleIcon(__pTizenBitmap);
		pHeader->SetTitleText("TEXT VIEWER");
	}

	// Setup back event listener
	SetFormBackEventListener(this);

	// Get a button via resource ID

	pTextBox = static_cast<TextBox*>(GetControl(IDC_TEXTBOX));

	return r;
}

result TextViewerForm::OnTerminating(void) {
	result r = E_SUCCESS;

	// TODO: Add your termination code here
	return r;
}

void TextViewerForm::OnActionPerformed(const Tizen::Ui::Control& source,
		int actionId) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

}

void TextViewerForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {

	/*
	 UiApp* pApp = UiApp::GetInstance();
	 AppAssert(pApp);
	 pApp->Terminate();
	 */


	 SceneManager* pSceneManager = SceneManager::GetInstance();
	 AppAssert(pSceneManager);
	 pSceneManager->GoBackward(
	 BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));

	//popup.ShowPopup();
}

void TextViewerForm::OnSceneActivatedN(
		const Tizen::Ui::Scenes::SceneId& previousSceneId,
		const Tizen::Ui::Scenes::SceneId& currentSceneId,
		Tizen::Base::Collection::IList* pArgs) {
	// TODO: Activate your scene here.
	if (pArgs != null) {
		if (pArgs->GetCount()) {
			String* pStr = dynamic_cast<String*>(pArgs->GetAt(0));
			if (pStr != null) {

				this->filePath = *pStr;
				AppLog("in the textviewer : %S", this->filePath.GetPointer());
			}
		}
		pArgs->RemoveAll(true);
		delete pArgs;
	}

	LoadText();
	sensorProxy = new ProxySensor();
}

void TextViewerForm::OnSceneDeactivated(
		const Tizen::Ui::Scenes::SceneId& currentSceneId,
		const Tizen::Ui::Scenes::SceneId& nextSceneId) {
	// TODO: Deactivate your scene here.
	sensorProxy->~ProxySensor();
}

bool TextViewerForm::ReadFile(const String& strFilePath,
		String& strFileContents) {
	strFileContents = L"";
	File file;

	result r = file.Construct(strFilePath, L"r");

	if (IsFailed(r)) {
		AppLog("File::Consturct()is failed by %s", GetErrorMessage(r));
		return false;
	}

	FileAttributes fileAttrs;
	file.GetAttributes(strFilePath, fileAttrs);
	long long size = fileAttrs.GetFileSize();

	ByteBuffer readBuffer;
	readBuffer.Construct((int) size + 1);
	r = file.Read(readBuffer);

	if (IsFailed(r)) {
		AppLog("File::Read()is failed by %s", GetErrorMessage(r));
		return false;
	}

	readBuffer.Flip();
	strFileContents = ((char*) readBuffer.GetPointer());
	return true;
}

void TextViewerForm::LoadText() {

	String strFileContents, itemText;

	int endPos = 0;

	if (!ReadFile(this->filePath, strFileContents))
		return;

	pTextBox->SetText(strFileContents);
	pTextBox->RequestRedraw();

	while (strFileContents.GetLength() > 0) {
		strFileContents.IndexOf(L"\n", 0, endPos);

		if (endPos < 1) {
			itemText = strFileContents;
			itemText.Trim();
			strFileContents = L"";
		} else {
			strFileContents.SubString(0, endPos, itemText);
			strFileContents.Remove(0, endPos + 1);
		}

		if (itemText.GetLength() < 1)
			continue;
		AppLog("%ls", itemText.GetPointer());
	}
}

