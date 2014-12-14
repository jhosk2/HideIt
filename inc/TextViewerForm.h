#ifndef _TEXT_VIEWER_FORM_H_
#define _TEXT_VIEWER_FORM_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

#include "ShowTerminatePopup.h"
#include "ProxySensor.h"

class TextViewerForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
 	, public Tizen::Ui::Scenes::ISceneEventListener
{
public:
		TextViewerForm(void);
	virtual ~TextViewerForm(void);
	bool Initialize(void);

private:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId);

	bool ReadFile(const Tizen::Base::String& strFilePath, Tizen::Base::String& strFileContents);
	void LoadText();

protected:
	Tizen::Base::String filePath;
	Tizen::Ui::Controls::TextBox* pTextBox;
	Tizen::Graphics::Bitmap *__pTizenBitmap;
	ShowTerminatePopup popup;
	ProxySensor* sensorProxy;
};

#endif	//_TEXT_MAIN_FORM_H_
