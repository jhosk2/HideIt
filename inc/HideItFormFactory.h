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

#ifndef _HIDEIT_FORM_FACTORY_H_
#define _HIDEIT_FORM_FACTORY_H_

#include <FUi.h>

// Use 'extern' to eliminate duplicate data allocation.
extern const wchar_t* FORM_CALCULATOR;
extern const wchar_t* FORM_SETTING;
extern const wchar_t* FORM_FILEBROWSER;
extern const wchar_t* FORM_TEXTVIEWER;

class HideItFormFactory: public Tizen::Ui::Scenes::IFormFactory {
public:
	HideItFormFactory(void);
	virtual ~HideItFormFactory(void);

	virtual Tizen::Ui::Controls::Form* CreateFormN(
			const Tizen::Base::String& formId,
			const Tizen::Ui::Scenes::SceneId& sceneId);
};

#endif // _TESTMENU_FORM_FACTORY_H_
