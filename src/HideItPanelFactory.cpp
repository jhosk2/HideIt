#include <new>
#include "HideItPanelFactory.h"
#include "BrowserTab1.h"
#include "BrowserTab2.h"

using namespace Tizen::Ui::Scenes;

const wchar_t* PANEL_MANAGEMENT = L"ManagementPanel";
const wchar_t* PANEL_HIDDEN = L"HiddenPanel";

HideItPanelFactory::HideItPanelFactory(void) {
}

HideItPanelFactory::~HideItPanelFactory(void) {
}

Tizen::Ui::Controls::Panel*
HideItPanelFactory::CreatePanelN(const Tizen::Base::String& panelId,
		const Tizen::Ui::Scenes::SceneId& sceneId) {
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	Tizen::Ui::Controls::Panel* pNewPanel = null;

	if (panelId == PANEL_MANAGEMENT) {
		BrowserTab1* pPanel = new (std::nothrow) BrowserTab1();
		pPanel->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pPanel);
		pNewPanel = pPanel;
	} else if (panelId == PANEL_HIDDEN) {
		BrowserTab2* pPanel = new (std::nothrow) BrowserTab2();
		pPanel->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pPanel);
		pNewPanel = pPanel;
	}
	// TODO: Add your panel creation code here
	return pNewPanel;
}
