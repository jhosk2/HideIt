/*
 * SettingPasswordForm.h
 *
 *  Created on: Dec 2, 2014
 *      Author: ajou
 */

#ifndef SETTINGPASSWORDFORM_H_
#define SETTINGPASSWORDFORM_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

class SettingPasswordForm: public Tizen::Ui::Controls::Form,
		public Tizen::Ui::IActionEventListener,
		public Tizen::Ui::Controls::IFormBackEventListener,
		public Tizen::Ui::Scenes::ISceneEventListener,
		public Tizen::Ui::ITextEventListener {
public:
	SettingPasswordForm(void);
	virtual ~SettingPasswordForm(void);
	bool Initialize(void);

private:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source,
			int actionId);
	void SettingPassword(void);

	virtual void OnTextValueChanged(const Tizen::Ui::Control& source);
	virtual void OnTextValueChangeCanceled(const Tizen::Ui::Control& source);

	virtual void OnSceneActivatedN(
			const Tizen::Ui::Scenes::SceneId& previousSceneId,
			const Tizen::Ui::Scenes::SceneId& currentSceneId,
			Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(
			const Tizen::Ui::Scenes::SceneId& currentSceneId,
			const Tizen::Ui::Scenes::SceneId& nextSceneId);

	Tizen::Graphics::Bitmap *__pTizenBitmap;

protected:
	Tizen::Ui::Controls::EditField* pEditField_Input;
	Tizen::Ui::Controls::EditField* pEditField_Re_Input;
	Tizen::Ui::Controls::Label* pLabel_Err;
	static const int ID_BUTTON_CONFIRM = 100;

};

#endif	//SETTINGPASSWORDFORM_H_
