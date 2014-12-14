#ifndef _BROWSER_TAB1_H_
#define _BROWSER_TAB1_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>
#include "ProxySensor.h"

#include "ListViewItemPopup.h"
#include "ShowTerminatePopup.h"

class FolderBrowser;

class BrowserTab1: public Tizen::Ui::Controls::Panel,
		public Tizen::Ui::Scenes::ISceneEventListener,
		public Tizen::Ui::ITouchEventListener,
		public Tizen::Ui::Controls::IFormBackEventListener {
public:
	BrowserTab1(void);
	virtual ~BrowserTab1(void);
	bool Initialize(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnSceneActivatedN(
			const Tizen::Ui::Scenes::SceneId& previousSceneId,
			const Tizen::Ui::Scenes::SceneId& currentSceneId,
			Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(
			const Tizen::Ui::Scenes::SceneId& currentSceneId,
			const Tizen::Ui::Scenes::SceneId& nextSceneId);

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	virtual void OnTouchFocusIn(const Tizen::Ui::Control& source,
			const Tizen::Graphics::Point& currentPosition,
			const Tizen::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchFocusOut(const Tizen::Ui::Control& source,
			const Tizen::Graphics::Point& currentPosition,
			const Tizen::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchMoved(const Tizen::Ui::Control& source,
			const Tizen::Graphics::Point& currentPosition,
			const Tizen::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchPressed(const Tizen::Ui::Control& source,
			const Tizen::Graphics::Point& currentPosition,
			const Tizen::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchReleased(const Tizen::Ui::Control& source,
			const Tizen::Graphics::Point& currentPosition,
			const Tizen::Ui::TouchEventInfo& touchInfo);

private:
	Tizen::Ui::Controls::Label* __pLabel_Back;
	FolderBrowser* folderBrowser;
	ProxySensor* sensorProxy;
	ShowTerminatePopup terpopup;

};

#endif // _BROWSER_TAB1_H_
