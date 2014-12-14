/*
 * FileBrowserPopup.cpp
 *
 *  Created on: Dec 1, 2014
 *      Author: 원빈
 *
 *  2014-12-3 Add pop-up Button, Jong_won
 *
 */

#include "ListViewItemPopup.h"
#include "SceneRegister.h"
#include <FGraphics.h>
#include <FIo.h>
#include <FApp.h>
#include "Cryptor.h"
#include "FolderBrowser.h"

using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Io;
using namespace Tizen::Base;

//int fileFlag = FILESTATE_ENCRYPTION;

int FILE_STATE_FLAG = FILESTATE_DECRYPTION;
const int FILESTATE_ENCRYPTION = 601;
const int FILESTATE_DECRYPTION = 602;

bool PopupEventListener::OnKeyReleased(Control& source,
		const KeyEventInfo& keyEventInfo) {
	KeyCode key = keyEventInfo.GetKeyCode();
	if (key == KEY_BACK || key == KEY_ESC) {
		Popup* pPopup = static_cast<Popup *>(&source);
		pPopup->SetShowState(false);
		pPopup->Invalidate(true);
	}

	return false;
}

ListViewItemPopup::ListViewItemPopup(FolderBrowser* _pFolderBrowser) :
		__pPopup(null), pFolderBrowser(_pFolderBrowser) {
	result r = E_SUCCESS;

	__pPopupListener = new PopupEventListener;

	// Creates an instance of Popup
	__pPopup = new Popup;
	__pPopup->Construct(true, Dimension(300, 360));
	__pPopup->SetTitleText(L"Select Task");
	// Creates an instance of Button to Open File
	Button* pOpenButton = new Button();
	pOpenButton->Construct(Rectangle(25, 10, 250, 80), L"Open File");
	pOpenButton->SetActionId(ID_BUTTON_OPEN_POPUP);
	pOpenButton->AddActionEventListener(*this);

	if (FILE_STATE_FLAG == FILESTATE_DECRYPTION) {
		// Creates an instance of Button to Encryption file
		Button* pEncryptionButton = new Button();
		pEncryptionButton->Construct(Rectangle(25, 100, 250, 80),
				L"Encryption");
		pEncryptionButton->SetActionId(ID_BUTTON_ENCRYPTION_FILE);
		pEncryptionButton->AddActionEventListener(*this);
		__pPopup->AddControl(pEncryptionButton);
	} else {
		Button* pDecryptionButton = new Button();
		pDecryptionButton->Construct(Rectangle(25, 100, 250, 80),
				L"Decryption");
		pDecryptionButton->SetActionId(ID_BUTTON_ENCRYPTION_FILE);
		pDecryptionButton->AddActionEventListener(*this);
		__pPopup->AddControl(pDecryptionButton);
	}
	// Creates an instance of Button to close the popup.
	Button* pCloseButton = new Button();
	pCloseButton->Construct(Rectangle(25, 190, 250, 80), L"Close");
	pCloseButton->SetActionId(ID_BUTTON_CLOSE_POPUP);
	pCloseButton->AddActionEventListener(*this);

	// Adds the button to the popup
	__pPopup->AddControl(pCloseButton);
	__pPopup->AddControl(pOpenButton);
	__pPopup->SetPropagatedKeyEventListener(__pPopupListener);

}
;

ListViewItemPopup::~ListViewItemPopup() {
	// Deallocates the __pPopup
	__pPopup->Destroy();
	delete __pPopupListener;
}

void ListViewItemPopup::ShowPopup(const Tizen::Base::String& filename) {
	AppLog("Name is : %S", filename.GetPointer());
	currentFilePath = filename;
	__pPopup->SetShowState(true);
	__pPopup->Show();
}

void ListViewItemPopup::HidePopup(void) {
	__pPopup->SetShowState(false);
}

void ListViewItemPopup::OnActionPerformed(const Control& source, int actionId) {

	switch (actionId) {
	case ID_BUTTON_OPEN_POPUP: {

		String targetFile;
    	if (FILE_STATE_FLAG != FILESTATE_DECRYPTION) {
    		Cryptor cryptor;
    		cryptor.Decrypt(currentFilePath, true);

    		targetFile.Append(TEMPPATH);
    		targetFile.Append(File::GetFileExtension(currentFilePath));
    	}
    	else
    	{
    		targetFile = currentFilePath;
    	}

		String ext = File::GetFileExtension(targetFile);
		if (ext.Equals(L"txt", false) || ext.Equals(L"doc", false)
					|| ext.Equals(L"docx", false) || ext.Equals(L"hwp", false)) {
				ExecuteTextFile(targetFile);
		}
		else
		{
			result r;
			try{
			r = Tizen::App::AppControl::FindAndStart(L"http://tizen.org/appcontrol/operation/view", &targetFile, null, null, null, null);
			}
			catch(result r2)
			{

			}
			switch(r)
			{
			case E_SUCCESS:
					break;
			case E_MAX_EXCEEDED:
					break;
			case E_OBJ_NOT_FOUND:
					break;
			case E_PRIVILEGE_DENIED:
					break;
			case E_SYSTEM:
					break;
			case E_ILLEGAL_ACCESS:
					break;
			}
		}
		HidePopup();

    	if (FILE_STATE_FLAG != FILESTATE_DECRYPTION) {
    		//File::Remove(targetFile);
    	}
	}

		break;

	case ID_BUTTON_CLOSE_POPUP: {
		HidePopup();
	}
		break;

    case ID_BUTTON_ENCRYPTION_FILE :
    {
    	Cryptor cryptor;

    	if (FILE_STATE_FLAG == FILESTATE_DECRYPTION) {
    		cryptor.Encrypt(currentFilePath);
    		File::Remove(currentFilePath);
    	}
    	else
    	{
    		cryptor.Decrypt(currentFilePath);
    		File::Remove(currentFilePath);
    	}
		HidePopup();
    	//TODO file encryption, 플래그 변경
    }
        break;
	default:
		break;
	}
	pFolderBrowser->Refresh();
}

void ListViewItemPopup::ExecuteTextFile(Tizen::Base::String filePath) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	ArrayList* pList = new (std::nothrow) ArrayList;
	AppAssert(pList);
	pList->Construct();
	pList->Add(*(new (std::nothrow) Tizen::Base::String(filePath)));

	pSceneManager->GoForward(ForwardSceneTransition(SCENE_TEXTVIEWER), pList);

}
