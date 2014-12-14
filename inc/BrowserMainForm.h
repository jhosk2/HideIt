#ifndef _BROWSER_MAIN_FORM_H_
#define _BROWSER_MAIN_FORM_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>
#include "ShowTerminatePopup.h"

class BrowserMainForm: public Tizen::Ui::Controls::Form,
		public Tizen::Ui::IActionEventListener,
		public Tizen::Ui::Controls::IFormMenuEventListener,
		public Tizen::Ui::Controls::IFormBackEventListener,
		public Tizen::Ui::Scenes::ISceneEventListener {
public:
	BrowserMainForm(void);
	virtual ~BrowserMainForm(void);
	bool Initialize(void);

private:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source,
			int actionId);

	virtual void OnFormMenuRequested(Tizen::Ui::Controls::Form& source);

	void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
			const Tizen::Ui::Scenes::SceneId& currentSceneId,
			Tizen::Base::Collection::IList* pArgs);
	void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
			const Tizen::Ui::Scenes::SceneId& nextSceneId);
protected:
	Tizen::Ui::Controls::OptionMenu* __pOption;
	static const int IDA_CONTEXT_ITEM = 103;
	static const int ID_HEADER_ITEM1 = 101;
	static const int ID_HEADER_ITEM2 = 102;
	Tizen::Graphics::Bitmap *__pTizenBitmap;
	ShowTerminatePopup terimPopup;
};

#endif	//_BROWSER_MAIN_FORM_H_
