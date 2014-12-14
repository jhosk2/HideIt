#ifndef _CALCULATOR_FORM_H_
#define _CALCULATOR_FORM_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>
#include <string.h>
#include <FUix.h>
#include <FIo.h>
#include <SvmWrapper.h>
#include "BrowserMainForm.h"

class CalculatorForm: public Tizen::Ui::Controls::Form,
		public Tizen::Ui::IActionEventListener,
		public Tizen::Ui::Controls::IFormBackEventListener,
		public Tizen::Ui::ITouchLongPressGestureEventListener,
		public Tizen::Ui::ITouchEventListener,
		public Tizen::Ui::Scenes::ISceneEventListener,
		public Tizen::Ui::IOrientationEventListener,
		public Tizen::Uix::Sensor::ISensorEventListener {

public:
	CalculatorForm(void);
	virtual ~CalculatorForm(void);
	bool Initialize();
	void OnOrientationChanged(const Tizen::Ui::Control& source,
			Tizen::Ui::OrientationStatus orientationStatus);

	virtual void OnLongPressGestureDetected(
			Tizen::Ui::TouchLongPressGestureDetector& gestureDetector);
	virtual void OnLongPressGestureCanceled(
			Tizen::Ui::TouchLongPressGestureDetector& gestureDetector);

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

	virtual void OnDataReceived(Tizen::Uix::Sensor::SensorType sensorType,
			Tizen::Uix::Sensor::SensorData& sensorData, result r);

	void StartSensor(void);
	void StopSensor(void);
	void FeatureProcessing(void);

	virtual void OnSceneActivatedN(
			const Tizen::Ui::Scenes::SceneId& previousSceneId,
			const Tizen::Ui::Scenes::SceneId& currentSceneId,
			Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(
			const Tizen::Ui::Scenes::SceneId& currentSceneId,
			const Tizen::Ui::Scenes::SceneId& nextSceneId);
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source,
			int actionId);
private:

	void lowPass(const Tizen::Base::Collection::ArrayListT<float>& param,
			Tizen::Base::Collection::ArrayListT<float>& output);
	float getMean(Tizen::Base::Collection::ArrayListT<float>& param);
	void clearBuffer(void);

	void AddOperand(Tizen::Base::String exp);
	void AddOperator(Tizen::Base::String exp);
	void AddEngineer(Tizen::Base::String exp);
	void SpecialExpression(Tizen::Base::String exp);
	int Before(int index);

	int* CheckBeforeOperand(Tizen::Base::String expression);
	std::string NarrowString(const std::wstring& str, const char* localeName =
			"C");
//	String convertStringToNormal(wchar_t* pString);

	void ShowResult(double* cal_result);
	void SetLabelTextSize(void);

	result AddCalculatorPanel(void);
	result AddErrorPopup(void);
	result AddList_Init(void);

	void ShowErrorPopup(Tizen::Base::String pErrMsg);

	enum ButtonActionId {
		ID_NOR_BTN_0,
		ID_NOR_BTN_1,
		ID_NOR_BTN_2,
		ID_NOR_BTN_3,
		ID_NOR_BTN_4,
		ID_NOR_BTN_5,
		ID_NOR_BTN_6,
		ID_NOR_BTN_7,
		ID_NOR_BTN_8,
		ID_NOR_BTN_9,
		ID_NOR_BTN_DOT,
		ID_NOR_BTN_SIGN,
		ID_NOR_BTN_MINUS,
		ID_NOR_BTN_PLUS,
		ID_NOR_BTN_BACK,
		ID_NOR_BTN_MULTIPLY,
		ID_NOR_BTN_DIVIDE,
		ID_NOR_BTN_PAREN,
		ID_NOR_BTN_CLEAR,
		ID_ENG_BTN_0,
		ID_ENG_BTN_1,
		ID_ENG_BTN_2,
		ID_ENG_BTN_3,
		ID_ENG_BTN_4,
		ID_ENG_BTN_5,
		ID_ENG_BTN_6,
		ID_ENG_BTN_7,
		ID_ENG_BTN_8,
		ID_ENG_BTN_9,
		ID_ENG_BTN_DOT,
		ID_ENG_BTN_SIGN,
		ID_ENG_BTN_MINUS,
		ID_ENG_BTN_PLUS,
		ID_ENG_BTN_BACK,
		ID_ENG_BTN_MULTIPLY,
		ID_ENG_BTN_DIVIDE,
		ID_ENG_BTN_PAREN,
		ID_ENG_BTN_CLEAR,
		ID_ENG_BTN_FACTO,
		ID_ENG_BTN_ROOT,
		ID_ENG_BTN_PERCENT,
		ID_ENG_BTN_SIN,
		ID_ENG_BTN_COS,
		ID_ENG_BTN_TAN,
		ID_ENG_BTN_LN,
		ID_ENG_BTN_LOG,
		ID_ENG_BTN_1OVERX,
		ID_ENG_BTN_EPOWERX,
		ID_ENG_BTN_XPOWER2,
		ID_ENG_BTN_YPOWERX,
		ID_ENG_BTN_ABS,
		ID_ENG_BTN_PI,
		ID_ENG_BTN_E
	};

protected:
	Tizen::Ui::TouchLongPressGestureDetector* __pLongPressGesture;
	Tizen::Ui::Controls::Label* __pLabel_Normal_Equal;
	Tizen::Ui::Controls::Label* __pLabel_Eng_Equal;
	Tizen::Ui::Controls::Label* __pLabelExpression;
	Tizen::Ui::Controls::Popup* __pPopup;
	Tizen::Ui::Controls::Label* __pPopup_Label;
	Tizen::Ui::Controls::Panel* __pPanel[2];
	int __panelId;

	Tizen::Uix::Sensor::SensorManager __sensorMgr;

	Tizen::Base::Collection::ArrayListT<float> Rawtiltpitch;
	Tizen::Base::Collection::ArrayListT<float> Rawtiltroll;
	Tizen::Base::Collection::ArrayListT<float> Filteredtiltpitch;
	Tizen::Base::Collection::ArrayListT<float> Filteredtiltroll;

	SvmWrapper svm;
	int SensorCnt;

	BrowserMainForm* __pBrowser;

	Tizen::Graphics::Bitmap *__pTizenBitmap;

};

extern "C" {
std::string string_format(const std::string fmt, ...);
}

#endif	//_HIDE_IT_FORM_H_
