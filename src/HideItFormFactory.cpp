//
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
//
// Licensed under the Flora License, Version 1.1 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://floralicense.org/license/
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <new>

#include "HideItFormFactory.h"
#include "CalculatorForm.h"
#include "SettingPasswordForm.h"
#include "BrowserMainForm.h"
#include "TextViewerForm.h"

using namespace Tizen::Ui::Scenes;

// Definitions of extern.
const wchar_t* FORM_CALCULATOR = L"CalculatorForm";
const wchar_t* FORM_SETTING = L"SettingPasswordForm";
const wchar_t* FORM_FILEBROWSER = L"FileBrowserForm";
const wchar_t* FORM_TEXTVIEWER = L"TextViewerForm";

HideItFormFactory::HideItFormFactory(void) {
}

HideItFormFactory::~HideItFormFactory(void) {
}

Tizen::Ui::Controls::Form*
HideItFormFactory::CreateFormN(const Tizen::Base::String& formId,
		const Tizen::Ui::Scenes::SceneId& sceneId) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	Tizen::Ui::Controls::Form* pNewForm = null;

	if (formId == FORM_CALCULATOR) {
		CalculatorForm* pForm = new (std::nothrow) CalculatorForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	} else if (formId == FORM_SETTING) {
		SettingPasswordForm* pForm = new (std::nothrow) SettingPasswordForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	} else if (formId == FORM_FILEBROWSER) {
		BrowserMainForm* pForm = new (std::nothrow) BrowserMainForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	} else if (formId == FORM_TEXTVIEWER) {
		TextViewerForm* pForm = new (std::nothrow) TextViewerForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}

	return pNewForm;
}
