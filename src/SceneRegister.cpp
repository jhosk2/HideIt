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

#include "SceneRegister.h"
#include "HideItFormFactory.h"
#include "HideItPanelFactory.h"

using namespace Tizen::Ui::Scenes;

// Definitions of extern.
const wchar_t* SCENE_CALCULATOR = L"CalculatorScene";
const wchar_t* SCENE_SETTING = L"SettingPasswordScene";

const wchar_t* SCENE_FILEBROWSER_MANAGEMENT = L"FileBrowserManagementScene";
const wchar_t* SCENE_FILEBROWSER_HIDDEN = L"FileBrowserHiddenScene";

const wchar_t* SCENE_FILEBROWSER = L"FileBrowserScene";
const wchar_t* SCENE_TEXTVIEWER = L"TextViewerScene";

SceneRegister::SceneRegister(void) {
}

SceneRegister::~SceneRegister(void) {
}

void SceneRegister::RegisterAllScenes(void) {
	static const wchar_t* PANEL_BLANK = L"";
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	static HideItFormFactory formFactory;
	static HideItPanelFactory panelFactory;

	pSceneManager->RegisterPanelFactory(panelFactory);
	pSceneManager->RegisterFormFactory(formFactory);

	pSceneManager->RegisterScene(SCENE_CALCULATOR, FORM_CALCULATOR,
			PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_SETTING, FORM_SETTING, PANEL_BLANK);

	pSceneManager->RegisterScene(SCENE_FILEBROWSER, FORM_FILEBROWSER, PANEL_BLANK);

	pSceneManager->RegisterScene(SCENE_FILEBROWSER_MANAGEMENT, FORM_FILEBROWSER, PANEL_MANAGEMENT);
	pSceneManager->RegisterScene(SCENE_FILEBROWSER_HIDDEN, FORM_FILEBROWSER, PANEL_HIDDEN);
	pSceneManager->RegisterScene(SCENE_TEXTVIEWER, FORM_TEXTVIEWER, PANEL_BLANK);


}
