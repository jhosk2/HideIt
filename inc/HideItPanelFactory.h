#ifndef _HIDEIT_PANEL_FACTORY_H_
#define _HIDEIT_PANEL_FACTORY_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

extern const wchar_t* PANEL_MANAGEMENT;
extern const wchar_t* PANEL_HIDDEN;

class HideItPanelFactory: public Tizen::Ui::Scenes::IPanelFactory {
public:
	HideItPanelFactory(void);
	virtual ~HideItPanelFactory(void);

	virtual Tizen::Ui::Controls::Panel* CreatePanelN(
			const Tizen::Base::String& panelId,
			const Tizen::Ui::Scenes::SceneId& sceneId);
};

#endif // _BROWSER_PANEL_FACTORY_H_
