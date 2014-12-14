#include "CalculatorForm.h"
#include "AppResourceId.h"
#include "Calculate.h"
#include "SceneRegister.h"
#include <FMedia.h>
#include <FApp.h>

using namespace Tizen::Base;
using namespace Tizen::Graphics;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Uix::Sensor;
using namespace Tizen::Io;
using namespace Tizen::Base::Collection;
using namespace Tizen::Media;
using namespace Tizen::Ui::Scenes;

static const int OPERAND_NUMBER_0 = 600;
static const int OPERAND_NUMBER_NOT_0 = 601;
static const int OPERAND_NOT_NUMBER = 602;
static const int DOT = 603;
static const int OPERATOR = 604;
static const int LEFT_PAREN = 605;
static const int ETC = 606;
static const int NONE = 607;
static const int DELETE_SIGN = 701;
static const int ADD_SIGN_EMPTY = 702;
static const int ADD_SIGN_ETC = 703;
static const int ADD_SIGN_MINUS = 704;
static const int ADD_SIGN_START = 705;

static const int RESULT_OK = 801;
static const int RESULT_ERR = 802;
static const int NO_RESULT = 803;

static const double RESULT_NORMAL = 901;
static const double RESULT_INF = 902;
const static double RESULT_OVERFLOW = 903;
const static double RESULT_INVALID = 904;

static const int NUM_NORMAL_BUTTONS = 19;
static const int NUM_ENG_BUTTONS = 34;

static const int MAX_OPERANDS = 15;
static const int MAX_UNDER_DOT = 9;
static const int MAX_SHOWING_NUM = 150;

static const float LOWPASSWEIGHT = 0.1f;
static const int SAMPLE_WINDOWSIZE = 5;
static const long SENSOR_INTERVAL = 50;
static const int TILTSTOP = 0;
static const int TILTLEFT = 1;
static const int TILTRIGHT =2;

static const int TEXT_SIZE_BIG = 80;
static const int TEXT_SIZE_MIDDLE = 56;
static const int TEXT_SIZE_SMALL = 32;
//에러 팝업창 위치
static const Dimension POPUP_DIMENSION(500, 200);
static const Rectangle POPUP_LABEL_RECT(0, 0, 500, 70);
static const Rectangle POPUP_BTN_RECT(195, 70, 120, 50);
static const int ACTION_ID_POPUP_BTN_OK = 100;

Tizen::Base::String pExpression = "0";
int pNum_Paren = 0;
// pNum_Operand[0] -> 전체 숫자 길이
// pNum_Opearnd[1] -> 소숫점 이하 길이
int pNum_Operand[2] = { 0, 0 };

int pNum_Showing = 1;

bool pDotFlag = false;

int pResultFlag = NO_RESULT;

CalculatorForm::CalculatorForm(void) :
		__panelId(0) {
	__pPanel[0] = null;
	__pPanel[1] = null;
	SensorCnt = 0;

}

CalculatorForm::~CalculatorForm(void) {
}

bool CalculatorForm::Initialize() {
	result r = Construct(IDF_CALCULATOR_FORM);
	TryReturn(r == E_SUCCESS, false, "Failed to construct form");

	OrientationStatus status = GetOrientationStatus();
	if (status == ORIENTATION_STATUS_PORTRAIT
			|| status == ORIENTATION_STATUS_PORTRAIT_REVERSE) {
		__panelId = 0;
	} else {
		__panelId = 1;
	}
	return true;
}
result CalculatorForm::OnInitializing(void) {
	result r = E_SUCCESS;

	// Setup back event listener
	SetFormBackEventListener(this);

	// Setup orientation event listener
	AddOrientationEventListener(*this);

	r = AddCalculatorPanel();
	TryReturn(!IsFailed(r), r, "AddCalculatorPanel() failed with [%s]",
			GetErrorMessage(r));

	r = AddErrorPopup();
	TryReturn(!IsFailed(r), r, "AddErrorPopup() failed with [%s]",
			GetErrorMessage(r));

	r = AddList_Init();
	TryReturn(!IsFailed(r), r, "AddList_Init() failed with [%s]",
			GetErrorMessage(r));

	Image pimage;
	result r1 = pimage.Construct();
	String filepath = App::GetInstance()->GetAppDataPath();
	__pTizenBitmap = pimage.DecodeN(filepath + L"icon_calculator.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);

	// Set-up header
	Header* pHeader = GetHeader();
	AppAssert(pHeader);
	pHeader->SetTitleIcon(__pTizenBitmap);

	return r;
}

result CalculatorForm::AddList_Init(void) {

	__sensorMgr.Construct();

	Rawtiltpitch.Construct();
	Rawtiltroll.Construct();

	Filteredtiltroll.Construct();
	Filteredtiltpitch.Construct();

	result r = E_SUCCESS;
	return r;
}
result CalculatorForm::OnTerminating(void) {
	result r = E_SUCCESS;

	return r;
}

result CalculatorForm::AddCalculatorPanel(void) {
	result r = E_SUCCESS;

	// It's used to define resource ID
	int index = 0;

	__pLongPressGesture = new (std::nothrow) TouchLongPressGestureDetector;
	if (__pLongPressGesture != null) {
		__pLongPressGesture->Construct();
		__pLongPressGesture->AddLongPressGestureEventListener(*this);
	}

	// Get two panels and buttons via resource ID
	__pPanel[0] = static_cast<Panel *>(GetControl(IDC_NORMAL_PANEL));
	if (__pPanel[0] != null) {
		const String NORMAL_BUTTON_NAME[] = { IDC_NORMAL_BTN_0,
				IDC_NORMAL_BTN_1, IDC_NORMAL_BTN_2, IDC_NORMAL_BTN_3,
				IDC_NORMAL_BTN_4, IDC_NORMAL_BTN_5, IDC_NORMAL_BTN_6,
				IDC_NORMAL_BTN_7, IDC_NORMAL_BTN_8, IDC_NORMAL_BTN_9,
				IDC_NORMAL_BTN_DOT, IDC_NORMAL_BTN_SIGN, IDC_NORMAL_BTN_MINUS,
				IDC_NORMAL_BTN_PLUS, IDC_NORMAL_BTN_BACK,
				IDC_NORMAL_BTN_MULTIPLY, IDC_NORMAL_BTN_DIVIDE,
				IDC_NORMAL_BTN_PAREN, IDC_NORMAL_BTN_CLEAR };

		Button* pButton = null;

		for (index = 0; index < NUM_NORMAL_BUTTONS; index++) {
			AppLog("%d", index);
			pButton = static_cast<Button*>(GetControl(NORMAL_BUTTON_NAME[index],
					true));
			TryReturn(pButton != null, r = E_SYSTEM,
			"Panel::GetControl() failed");

			pButton->SetActionId(index);
			pButton->AddActionEventListener(*this);
		}

		__pLabel_Normal_Equal = static_cast<Label*>(GetControl(
				IDC_NORMAL_LABEL_EQUAL, true));
		TryReturn(__pLabel_Normal_Equal != null, r = E_SYSTEM,
		"Panel::GetControl() failed");

		__pLabel_Normal_Equal->AddTouchEventListener(*this);
		__pLabel_Normal_Equal->AddGestureDetector(__pLongPressGesture);

		__pPanel[0]->SetShowState(false);
	}

	__pPanel[1] = static_cast<Panel *>(GetControl(IDC_ENG_PANEL));
	if (__pPanel[1] != null) {
		const String ENG_BUTTON_NAME[] = { IDC_ENG_BTN_0, IDC_ENG_BTN_11,
				IDC_ENG_BTN_2, IDC_ENG_BTN_3, IDC_ENG_BTN_4, IDC_ENG_BTN_5,
				IDC_ENG_BTN_6, IDC_ENG_BTN_7, IDC_ENG_BTN_8, IDC_ENG_BTN_9,
				IDC_ENG_BTN_DOT, IDC_ENG_BTN_SIGN, IDC_ENG_BTN_MINUS,
				IDC_ENG_BTN_PLUS, IDC_ENG_BTN_BACK, IDC_ENG_BTN_MULTIPLY,
				IDC_ENG_BTN_DIVIDE, IDC_ENG_BTN_PAREN, IDC_ENG_BTN_CLEAR,
				IDC_ENG_BTN_FACTO, IDC_ENG_BTN_ROOT, IDC_ENG_BTN_PERCENT,
				IDC_ENG_BTN_SIN, IDC_ENG_BTN_COS, IDC_ENG_BTN_TAN,
				IDC_ENG_BTN_LN, IDC_ENG_BTN_LOG, IDC_ENG_BTN_1OVERX,
				IDC_ENG_BTN_EPOWERX, IDC_ENG_BTN_XPOWER2, IDC_ENG_BTN_YPOWERX,
				IDC_ENG_BTN_ABS, IDC_ENG_BTN_PI, IDC_ENG_BTN_EE };

		Button* pButton = null;

		for (; (index - NUM_NORMAL_BUTTONS) < NUM_ENG_BUTTONS; index++) {
			AppLog("%d", index - NUM_NORMAL_BUTTONS);
			pButton = static_cast<Button*>(GetControl(
					ENG_BUTTON_NAME[index - NUM_NORMAL_BUTTONS], true));
			TryReturn(pButton != null, r = E_SYSTEM,
			"Panel::GetControl() failed");

			pButton->SetActionId(index);
			pButton->AddActionEventListener(*this);
		}

		__pLabel_Eng_Equal = static_cast<Label*>(GetControl(IDC_ENG_LABEL_EQUAL,
				true));
		TryReturn(__pLabel_Eng_Equal != null, r = E_SYSTEM,
		"Panel::GetControl() failed");

		__pLabel_Eng_Equal->AddTouchEventListener(*this);
		__pLabel_Eng_Equal->AddGestureDetector(__pLongPressGesture);

		__pPanel[1]->SetShowState(false);
	}

	__pPanel[__panelId]->SetShowState(true);

	__pLabelExpression = static_cast<Label*>(GetControl(IDC_EXPRESSION));
	TryReturn(__pLabelExpression != null, r = E_SYSTEM,
	"Panel::GetControl(IDC_LBL_DISPLAY) failed");

	return r;
}

result CalculatorForm::AddErrorPopup(void) {
	result r = E_SUCCESS;
	Button* pBtnOK = null;

	__pPopup = new (std::nothrow) Popup();
	TryReturn(__pPopup != null, E_OUT_OF_MEMORY,
			"Unable to allocate memory for error pop-up");

	r = __pPopup->Construct(true, POPUP_DIMENSION);
	TryCatch(!IsFailed(r), /* NOP */,
			"Pop-up::Construct() with [%s]", GetErrorMessage(r));

	__pPopup_Label = new (std::nothrow) Label();
	TryCatch(__pPopup_Label != null, r = E_OUT_OF_MEMORY,
	"Unable to allocate memory for error label");

	r = __pPopup_Label->Construct(POPUP_LABEL_RECT, L"");
	TryCatch(!IsFailed(r), delete __pPopup_Label,
			"Label::Construct() with [%s]", GetErrorMessage(r));

	__pPopup_Label->SetText(L"");

	r = __pPopup->AddControl(__pPopup_Label);
	TryCatch(!IsFailed(r), delete __pPopup_Label,
			"Pop-up::AddControl() with [%s]", GetErrorMessage(r));

	pBtnOK = new (std::nothrow) Button();
	TryCatch(pBtnOK != null, r = E_OUT_OF_MEMORY,
	"Unable to allocate memory for ok button");

	r = pBtnOK->Construct(POPUP_BTN_RECT, L"OK");
	TryCatch(!IsFailed(r), delete pBtnOK, "Button::Construct() with [%s]",
			GetErrorMessage(r));

	pBtnOK->SetActionId(ACTION_ID_POPUP_BTN_OK);
	pBtnOK->AddActionEventListener(*this);

	__pPopup->AddControl(pBtnOK);
	TryCatch(!IsFailed(r), delete pBtnOK, "Pop-up::AddControl() with [%s]",
			GetErrorMessage(r));

	return E_SUCCESS;

	CATCH: delete __pPopup;
	__pPopup = null;
	return r;
}

void CalculatorForm::ShowErrorPopup(Tizen::Base::String pErrMsg) {

	__pPopup_Label->SetText(pErrMsg);
	__pPopup->SetShowState(true);
	__pPopup->Show();
}

//Invalid Input Popup

void CalculatorForm::OnActionPerformed(const Tizen::Ui::Control& source,
		int actionId) {

	switch (actionId) {
	case ID_NOR_BTN_0:
	case ID_ENG_BTN_0:
		AddOperand("0");
		break;
	case ID_NOR_BTN_1:
	case ID_ENG_BTN_1:
		AddOperand("1");
		break;
	case ID_NOR_BTN_2:
	case ID_ENG_BTN_2:
		AddOperand("2");
		break;
	case ID_NOR_BTN_3:
	case ID_ENG_BTN_3:
		AddOperand("3");
		break;
	case ID_NOR_BTN_4:
	case ID_ENG_BTN_4:
		AddOperand("4");
		break;
	case ID_NOR_BTN_5:
	case ID_ENG_BTN_5:
		AddOperand("5");
		break;
	case ID_NOR_BTN_6:
	case ID_ENG_BTN_6:
		AddOperand("6");
		break;
	case ID_NOR_BTN_7:
	case ID_ENG_BTN_7:
		AddOperand("7");
		break;
	case ID_NOR_BTN_8:
	case ID_ENG_BTN_8:
		AddOperand("8");
		break;
	case ID_NOR_BTN_9:
	case ID_ENG_BTN_9:
		AddOperand("9");
		break;
	case ID_NOR_BTN_MINUS:
	case ID_ENG_BTN_MINUS:
		AddOperator("-");
		break;
	case ID_NOR_BTN_PLUS:
	case ID_ENG_BTN_PLUS:
		AddOperator("+");
		break;
	case ID_NOR_BTN_MULTIPLY:
	case ID_ENG_BTN_MULTIPLY:
		AddOperator("*");
		break;
	case ID_NOR_BTN_DIVIDE:
	case ID_ENG_BTN_DIVIDE:
		AddOperator("/");
		break;
	case ID_ENG_BTN_FACTO:
		AddEngineer("!");
		break;
	case ID_ENG_BTN_PERCENT:
		AddOperator("%");
		break;
	case ID_ENG_BTN_SIN:
		AddEngineer("sin(");
		break;
	case ID_ENG_BTN_COS:
		AddEngineer("cos(");
		break;
	case ID_ENG_BTN_TAN:
		AddEngineer("tan(");
		break;
	case ID_ENG_BTN_LN:
		AddEngineer("ln(");
		break;
	case ID_ENG_BTN_LOG:
		AddEngineer("log(");
		break;
	case ID_ENG_BTN_1OVERX:
		AddEngineer("1/(");
		break;
	case ID_ENG_BTN_EPOWERX:
		AddEngineer("e^(");
		break;
	case ID_ENG_BTN_ABS:
		AddEngineer("abs(");
		break;
	case ID_ENG_BTN_PI:
		AddEngineer("Pi");
		break;
	case ID_ENG_BTN_E:
		AddEngineer("e");
		break;
	case ID_ENG_BTN_ROOT:
		AddEngineer("ROOT");
		break;
	case ID_ENG_BTN_XPOWER2:
		AddEngineer("x^2");
		break;
	case ID_ENG_BTN_YPOWERX:
		AddEngineer("Y^x");
		break;
	case ID_NOR_BTN_DOT:
	case ID_ENG_BTN_DOT:
		SpecialExpression(".");
		break;
	case ID_NOR_BTN_SIGN:
	case ID_ENG_BTN_SIGN:
		SpecialExpression("SIGN");
		break;
	case ID_NOR_BTN_BACK:
	case ID_ENG_BTN_BACK:
		SpecialExpression("BACK");
		break;
	case ID_NOR_BTN_PAREN:
	case ID_ENG_BTN_PAREN:
		SpecialExpression("PAREN");
		break;
	case ID_NOR_BTN_CLEAR:
	case ID_ENG_BTN_CLEAR:
		SpecialExpression("CLEAR");
		break;

		//팝업창에서 OK버튼 눌렀을 때
	case ACTION_ID_POPUP_BTN_OK:
		__pPopup->SetShowState(false);
		break;

	default:
		break;
	}
}

void CalculatorForm::AddOperand(Tizen::Base::String exp) {
	if (pNum_Showing > MAX_SHOWING_NUM) {
		ShowErrorPopup("수식은 150자를 넘길 수 없습니다.");
	} else {
		if (pNum_Operand[0] > MAX_OPERANDS) {
			ShowErrorPopup("숫자는 16자를 넘길 수 없습니다.");
		} else if (pNum_Operand[1] > MAX_UNDER_DOT) {
			ShowErrorPopup("소숫점 이하 10자리 수를 넘길 수 없습니다.");
		} else {

			if (pResultFlag == RESULT_OK || pResultFlag == RESULT_ERR) {
				pExpression = "";
				__pLabelExpression->SetText("");
				pResultFlag = NO_RESULT;
			}
			int pBefore = Before(1);
			// Handle 0 to 9
			switch (pBefore) {
			case OPERAND_NUMBER_0: {
				int pDoubleBefore = Before(2);
				if (pDoubleBefore > OPERAND_NUMBER_NOT_0) {
					pExpression.Remove(pExpression.GetLength() - 1, 1);
					pExpression = pExpression + exp;

					String pBackExp = __pLabelExpression->GetText();
					pBackExp.Remove(pBackExp.GetLength() - 1, 1);
					__pLabelExpression->SetText(pBackExp + exp);
					pNum_Showing--;
					pNum_Operand[0]--;
				} else {
					__pLabelExpression->SetText(
							__pLabelExpression->GetText() + exp);
					pExpression = pExpression + exp;
				}
				break;
			}
			case OPERAND_NOT_NUMBER: {
				__pLabelExpression->SetText(
						__pLabelExpression->GetText() + "*" + exp);
				pExpression = pExpression + "*" + exp;
				pNum_Showing++;
				break;
			}
			default: {
				__pLabelExpression->SetText(
						__pLabelExpression->GetText() + exp);
				pExpression = pExpression + exp;
				break;
			}
			}

			pNum_Operand[0]++;
			if (pDotFlag)
				pNum_Operand[1]++;

			pNum_Showing++;
			SetLabelTextSize();
		}
	}
}

void CalculatorForm::AddOperator(Tizen::Base::String exp) {
	if (pNum_Showing > MAX_SHOWING_NUM) {
		ShowErrorPopup("수식은 150자를 넘길 수 없습니다.");
	} else {
		if (!(pResultFlag == RESULT_ERR)) {
			pResultFlag = NO_RESULT;
			int pBefore = Before(1);

			switch (pBefore) {
			case OPERATOR: {

				pExpression.Remove(pExpression.GetLength() - 1, 1);
				pExpression = pExpression + exp;

				String pBackExp = __pLabelExpression->GetText();
				pBackExp.Remove(pBackExp.GetLength() - 1, 1);
				__pLabelExpression->SetText(pBackExp + exp);
				pNum_Showing--;
				break;
			}
			case DOT: {
				__pLabelExpression->SetText(
						__pLabelExpression->GetText() + "0" + exp);
				pExpression = pExpression + "0" + exp;
				pNum_Showing++;
				break;
			}
			case LEFT_PAREN:
			case NONE:
				break;
			default: {
				__pLabelExpression->SetText(
						__pLabelExpression->GetText() + exp);
				pExpression = pExpression + exp;
				break;
			}
			}

			pNum_Operand[0] = 0;
			pNum_Operand[1] = 0;
			pDotFlag = false;
			SetLabelTextSize();
			AppLog("%d : %d", pNum_Operand[0], pNum_Operand[1]);
			pNum_Showing++;
		}
	}
}

void CalculatorForm::AddEngineer(Tizen::Base::String exp) {

	if (pNum_Showing > MAX_SHOWING_NUM) {
		ShowErrorPopup("수식은 150자를 넘길 수 없습니다.");
	} else {
		String temp = __pLabelExpression->GetText();
		if (temp == "0") {
			__pLabelExpression->SetText("");
			pExpression = "";
			pNum_Showing = 0;
		}

		if (!(pResultFlag == RESULT_ERR)) {
			pResultFlag = NO_RESULT;
			String pExp;
			int pBefore = Before(1);
			//Change exp for pExpression
			if (exp == "sin(") {
				pExp = "s(";
				pNum_Paren++;
				pNum_Showing += 4;
			} else if (exp == "cos(") {
				pExp = "c(";
				pNum_Paren++;
				pNum_Showing += 4;
			} else if (exp == "tan(") {
				pExp = "t(";
				pNum_Paren++;
				pNum_Showing += 4;
			} else if (exp == "ln(") {
				pExp = "l(";
				pNum_Paren++;
				pNum_Showing += 4;
			} else if (exp == "log(") {
				pExp = "g(";
				pNum_Paren++;
				pNum_Showing += 4;
			} else if (exp == "abs(") {
				pExp = "a(";
				pNum_Paren++;
				pNum_Showing += 4;
			} else if (exp == "ROOT") {
				exp = "√(";
				pNum_Paren++;
				pExp = "r(";
				pNum_Showing += 2;
			} else if (exp == "Y^x") {
				exp = "^(";
				pExp = "^(";
				pNum_Paren++;
				pNum_Showing += 2;
			} else if (exp == "x^2") {
				exp = "^(2)";
				pExp = "^(2)";
				pNum_Showing += 4;
			} else if (exp == "Pi") {
				pExp = "p";
				pNum_Showing += 2;
			} else {
				pExp = exp;
			}

			if (exp == "^(" || exp == "^(2)" || exp == "!") {

				//Means pBefore is an operand
				if (pBefore <= OPERAND_NOT_NUMBER) {
					__pLabelExpression->SetText(
							__pLabelExpression->GetText() + exp);
					pExpression = pExpression + pExp;
				} else if (pBefore == DOT) {
					__pLabelExpression->SetText(
							__pLabelExpression->GetText() + "0" + exp);
					pExpression = pExpression + "0" + pExp;
					pDotFlag = false;
					pNum_Showing++;
				} else {
					ShowErrorPopup("입력값 오류!");
				}
			} else {
				if (exp == "e^(" || exp == "1/(") {
					pNum_Paren++;
					pNum_Showing += 3;
				} else {
					pNum_Showing++;
				}
				//Means pBefore is an operand
				if (pBefore <= OPERAND_NOT_NUMBER) {
					__pLabelExpression->SetText(
							__pLabelExpression->GetText() + "*" + exp);
					pExpression = pExpression + "*" + pExp;
					pNum_Showing++;
				} else if (pBefore == DOT) {
					__pLabelExpression->SetText(
							__pLabelExpression->GetText() + "0*" + exp);
					pExpression = pExpression + "0*" + pExp;
					pNum_Showing += 2;
					pDotFlag = false;
				} else {
					__pLabelExpression->SetText(
							__pLabelExpression->GetText() + exp);
					pExpression = pExpression + pExp;
				}

			}
			pNum_Operand[0] = 0;
			pNum_Operand[1] = 0;
			SetLabelTextSize();
			AppLog("%d : %d", pNum_Operand[0], pNum_Operand[1]);
		}
	}
}

void CalculatorForm::SpecialExpression(Tizen::Base::String exp) {
//DOT, SIGN, EQUAL, BACK, PAREN, CLEAR
	if (exp == "CLEAR") {
		pNum_Paren = 0;
		pNum_Operand[0] = 0;
		pNum_Operand[1] = 0;
		pDotFlag = false;
		__pLabelExpression->SetText("0");
		pExpression = "0";
		pResultFlag = NO_RESULT;
		pNum_Showing = 1;
		SetLabelTextSize();
	} else if (!(pResultFlag == RESULT_ERR)) {
		if (exp == "BACK") {
			int pBefore = Before(1);
			if (pBefore == OPERAND_NUMBER_0
					|| pBefore == OPERAND_NUMBER_NOT_0) {
				pNum_Operand[0]--;
				if (pDotFlag)
					pNum_Operand[1]--;
			}

			int pRemove_index = 1;
			wchar_t temp;
			pExpression.GetCharAt(pExpression.GetLength() - 1, temp);

			if (temp == '(') {
				pExpression.GetCharAt(pExpression.GetLength() - 2, temp);
				if (temp == 'r')
					pRemove_index = 2;
				else if (temp == 'l')
					pRemove_index = 3;
				else if (temp == 's' || temp == 'c' || temp == 't'
						|| temp == 'g' || temp == 'a')
					pRemove_index = 4;

				pNum_Paren--;
			} else if (temp == ')')
				pNum_Paren++;
			else if (temp == '.')
				pDotFlag = false;

			if (temp == 'p') {
				String pBackExp = __pLabelExpression->GetText();
				pBackExp.Remove(pBackExp.GetLength() - 2, 2);
				__pLabelExpression->SetText(pBackExp);
				pNum_Showing -= 2;
			} else {
				String pBackExp = __pLabelExpression->GetText();
				pBackExp.Remove(pBackExp.GetLength() - pRemove_index,
						pRemove_index);
				__pLabelExpression->SetText(pBackExp);
				pNum_Showing -= pRemove_index;

			}

			if (pRemove_index > 1)
				pRemove_index = 2;

			pExpression.Remove(pExpression.GetLength() - pRemove_index,
					pRemove_index);

		} else if (exp == ".") {
			if (pNum_Showing > MAX_SHOWING_NUM) {
				ShowErrorPopup("수식은 150자를 넘길 수 없습니다.");
			} else {
				if (!pDotFlag) {
					int pBefore = Before(1);

					switch (pBefore) {

					case OPERAND_NUMBER_0:
					case OPERAND_NUMBER_NOT_0: {
						__pLabelExpression->SetText(
								__pLabelExpression->GetText() + exp);
						pExpression = pExpression + exp;
						pNum_Showing++;
						break;
					}

					case OPERAND_NOT_NUMBER: {
						__pLabelExpression->SetText(
								__pLabelExpression->GetText() + "*0" + exp);
						pExpression = pExpression + "*0" + exp;
						pNum_Showing += 2;
						break;
					}

					case DOT:
						break;
					default: {
						__pLabelExpression->SetText(
								__pLabelExpression->GetText() + "0" + exp);
						pExpression = pExpression + "0" + exp;
						pNum_Showing += 2;
						break;
					}
					}
					pDotFlag = true;

				}
			}
		} else if (exp == "SIGN") {
			if (pNum_Showing > MAX_SHOWING_NUM) {
				ShowErrorPopup("수식은 150자를 넘길 수 없습니다.");
			} else {
				String tempString = __pLabelExpression->GetText();

				int* pSignFlag_pExpression = CheckBeforeOperand(pExpression);
				int* pSignFlag_pLabel = CheckBeforeOperand(tempString);
				AppLog("%d : %d", pSignFlag_pExpression[0],
						pSignFlag_pExpression[1]);
				AppLog("%d : %d", pSignFlag_pLabel[0], pSignFlag_pLabel[1]);

				int index_pExp = pExpression.GetLength()
						- pSignFlag_pExpression[1] + 1;
				int index_pLabel = tempString.GetLength() - pSignFlag_pLabel[1]
						+ 1;

				AppLog("%d : %d", index_pExp, index_pLabel);
				switch (pSignFlag_pLabel[0]) {
				case ADD_SIGN_EMPTY: {
					pExpression.Insert("(0-", 0);
					tempString.Insert("(-", 0);
					__pLabelExpression->SetText(tempString);
					pNum_Paren++;
					pNum_Showing += 2;
					break;
				}
				case ADD_SIGN_ETC:
				case ADD_SIGN_MINUS: {
					pExpression.Insert("(0-", index_pExp);
					tempString.Insert("(-", index_pLabel);
					__pLabelExpression->SetText(tempString);
					pNum_Paren++;
					pNum_Showing += 2;
					break;
				}
				case ADD_SIGN_START: {
					pExpression = "(0-" + pExpression;
					__pLabelExpression->SetText(
							"(-" + __pLabelExpression->GetText());
					pNum_Paren++;
					pNum_Showing += 2;
					break;
				}
				case DELETE_SIGN: {
					pExpression.Replace("(0-", "", index_pExp - 3);
					tempString.Replace("(-", "", index_pLabel - 2);
					__pLabelExpression->SetText(tempString);
					pNum_Paren--;
					pNum_Showing -= 2;
					break;
				}
				}
			}
		} else if (exp == "PAREN") {
			if (pNum_Showing > MAX_SHOWING_NUM) {
				ShowErrorPopup("수식은 150자를 넘길 수 없습니다.");
			} else {
				int pBefore = Before(1);
				pNum_Operand[0] = 0;
				pNum_Operand[1] = 0;

				if (pNum_Paren == 0) {
					switch (pBefore) {
					case OPERATOR:
					case NONE:
					case LEFT_PAREN: {
						// '('
						__pLabelExpression->SetText(
								__pLabelExpression->GetText() + "(");
						pExpression = pExpression + "(";
						pNum_Showing++;
						break;
					}
					case DOT: {
						// '0x('
						__pLabelExpression->SetText(
								__pLabelExpression->GetText() + "0*(");
						pExpression = pExpression + "0*(";
						pNum_Showing += 3;
						break;
					}
					default: {
						// 'x('
						__pLabelExpression->SetText(
								__pLabelExpression->GetText() + "*(");
						pExpression = pExpression + "*(";
						pNum_Showing += 2;
						break;
					}
					}
					pNum_Paren++;
				} else {
					switch (pBefore) {
					case OPERATOR:
					case LEFT_PAREN: {
						//'('
						__pLabelExpression->SetText(
								__pLabelExpression->GetText() + "(");
						pExpression = pExpression + "(";
						pNum_Paren++;
						pNum_Showing++;
						break;
					}
					case DOT: {
						// '0x('
						__pLabelExpression->SetText(
								__pLabelExpression->GetText() + "0*(");
						pExpression = pExpression + "0*(";
						pNum_Paren++;
						pNum_Showing += 3;
						break;
					}
					case OPERAND_NUMBER_0:
					case OPERAND_NUMBER_NOT_0:
					case OPERAND_NOT_NUMBER: { // ')'
						__pLabelExpression->SetText(
								__pLabelExpression->GetText() + ")");
						pExpression = pExpression + ")";
						pNum_Paren--;
						pNum_Showing++;
						break;
					}
					}
				}
			}
		}
		AppLog("%d : %d", pNum_Operand[0], pNum_Operand[1]);
		SetLabelTextSize();
	}
}

void CalculatorForm::ShowResult(double* cal_result) {
	AppLog("%f : %f", cal_result[0], cal_result[1]);

	if (cal_result[1] == RESULT_NORMAL) {
		String pResult = Tizen::Base::Double::ToString(cal_result[0]);

		for (int index = pResult.GetLength() - 1; index > 0; index--) {
			wchar_t temp;
			pResult.GetCharAt(index, temp);
			if (temp == '0')
				pResult.Replace("0", "", index);
			else if (temp == '.') {
				pResult.Replace(".", "", index);
				break;
			} else {
				break;
			}
		}

		__pLabelExpression->SetText(pResult);
		pExpression = pResult;
		pNum_Operand[0] = 0;
		pNum_Operand[1] = 0;
		pResultFlag = RESULT_OK;
	} else if (cal_result[1] == RESULT_INF) {
		pExpression = 0;
		__pLabelExpression->SetText("Expression Error!!!");
		pResultFlag = RESULT_ERR;
	} else if (cal_result[1] == RESULT_OVERFLOW) {
		pExpression = 0;
		__pLabelExpression->SetText("Expression Overflow!!!");
		pResultFlag = RESULT_ERR;
	} else if (cal_result[1] == RESULT_INVALID) {
		pExpression = 0;
		__pLabelExpression->SetText("Expression Invalid!!!");
		pResultFlag = RESULT_ERR;
	}
}

void CalculatorForm::SetLabelTextSize(void) {
//  32, 56, 80

	int pFontSize = 0;
	Tizen::Ui::OrientationStatus orientationStatus = GetOrientationStatus();
	if (orientationStatus == ORIENTATION_STATUS_PORTRAIT
			|| orientationStatus == ORIENTATION_STATUS_PORTRAIT_REVERSE) {
		if (pNum_Showing > 57)
			pFontSize = TEXT_SIZE_SMALL;
		else if (pNum_Showing > 26)
			pFontSize = TEXT_SIZE_MIDDLE;
		else
			pFontSize = TEXT_SIZE_BIG;
	} else {
		if (pNum_Showing > 76)
			pFontSize = TEXT_SIZE_SMALL;
		else if (pNum_Showing > 27)
			pFontSize = TEXT_SIZE_MIDDLE;
		else
			pFontSize = TEXT_SIZE_BIG;
	}

	if (__pLabelExpression->GetTextSize() != pFontSize)
		__pLabelExpression->SetTextConfig(pFontSize,
				Tizen::Ui::Controls::LABEL_TEXT_STYLE_NORMAL);

	__pLabelExpression->Invalidate(true);
}
//
int CalculatorForm::Before(int index) {
	int result = 0;

	if (pExpression.GetLength() < index) {
		result = NONE;
	} else {
		wchar_t temp;
		pExpression.GetCharAt(pExpression.GetLength() - index, temp);

		if (temp > '0' && temp <= '9') {
			result = OPERAND_NUMBER_NOT_0;
		} else if (temp == '0') {
			result = OPERAND_NUMBER_0;
		} else if (temp == '!' || temp == ')' || temp == 'e' || temp == 'p') {
			result = OPERAND_NOT_NUMBER;
		} else if (temp == '.') {
			result = DOT;
		} else if (temp == '+' || temp == '-' || temp == '/' || temp == '*'
				|| temp == '%') {
			result = OPERATOR;
		} else if (temp == '(') {
			result = LEFT_PAREN;
		} else {
			result = ETC;
		}
	}

	return result;
}

int* CalculatorForm::CheckBeforeOperand(Tizen::Base::String expression) {
	int* pResult = new int[2];
	int current = 0;
	wchar_t temp;

	if (expression.GetLength() == 1) {
		pResult[0] = ADD_SIGN_EMPTY;
		pResult[1] = 0;
		AppLog("ADD_SIGN_EMPTY");
		return pResult;
	}

	for (int i = 1; i < expression.GetLength() + 2; i++) {
		current = Before(i);

		if (current == OPERATOR) {
			expression.GetCharAt(expression.GetLength() - i, temp);
			if (temp == '+' || temp == '/' || temp == '*' || temp == '('
					|| temp == '%') {
				pResult[0] = ADD_SIGN_ETC;
				pResult[1] = i;
				break;

			} else if (temp == '-') {
				expression.GetCharAt(expression.GetLength() - (i + 1), temp);
				if (temp == '(') {
					pResult[0] = DELETE_SIGN;
					pResult[1] = i;
					break;
				} else {
					pResult[0] = ADD_SIGN_MINUS;
					pResult[1] = i;
					break;
				}
			}
		} else if (current == LEFT_PAREN) {
			pResult[0] = ADD_SIGN_ETC;
			pResult[1] = i;
			break;
		} else if (current == NONE) {
			pResult[0] = ADD_SIGN_START;
			pResult[1] = i;
			break;
		}
	}
	return pResult;
}

void CalculatorForm::OnOrientationChanged(const Tizen::Ui::Control& source,
		Tizen::Ui::OrientationStatus orientationStatus) {
	if (orientationStatus == ORIENTATION_STATUS_PORTRAIT
			|| orientationStatus == ORIENTATION_STATUS_PORTRAIT_REVERSE) {

		AppLog("Portrait");
		__pPanel[0]->SetShowState(true);
		__pPanel[1]->SetShowState(false);
		SetLabelTextSize();
	} else {
		AppLog("LandScape");
		__pPanel[0]->SetShowState(false);
		__pPanel[1]->SetShowState(true);
		SetLabelTextSize();
	}
}

void CalculatorForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source) {
	UiApp* pApp = UiApp::GetInstance();
	AppAssert(pApp);
	pApp->Terminate();
}

void CalculatorForm::OnTouchMoved(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {
	AppLog("On Touch Moved");
}

void CalculatorForm::OnTouchPressed(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {
	AppLog("On Touch Pressed");

	Color* color = new Color(153, 0, 0, 255);

	if (source.GetName() == IDC_NORMAL_LABEL_EQUAL) {
		__pLabel_Normal_Equal->SetBackgroundColor(*color);
	} else {
		__pLabel_Eng_Equal->SetBackgroundColor(*color);
	}
	Invalidate(true);
}

void CalculatorForm::OnTouchReleased(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {

	Color* color = new Color(255, 255, 160, 255);
	if (source.GetName() == IDC_NORMAL_LABEL_EQUAL) {
		__pLabel_Normal_Equal->SetBackgroundColor(*color);
	} else {
		__pLabel_Eng_Equal->SetBackgroundColor(*color);
	}
	Invalidate(true);

// 식을 cal.Run의 parameter로 넣어주면 double 형으로 return을 해줌
	int pBefore = Before(1);
	switch (pBefore) {
	case DOT:
		pExpression = pExpression + "0";
	case OPERAND_NOT_NUMBER:
	case OPERAND_NUMBER_0:
	case OPERAND_NUMBER_NOT_0: {
		while (pNum_Paren > 0) {
			pExpression = pExpression + ")";
			pNum_Paren--;
		}
	}
		break;
	case OPERATOR: {
		AppLog("ERROR");
		break;
	}
	}

	Calculate pCalculate;
	ShowResult(pCalculate.Run(pExpression));

}

void CalculatorForm::OnTouchFocusIn(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {
	AppLog("On Touch Focus In");
}

void CalculatorForm::OnTouchFocusOut(const Tizen::Ui::Control& source,
		const Tizen::Graphics::Point& currentPosition,
		const Tizen::Ui::TouchEventInfo& touchInfo) {
	AppLog("On Touch Focus Out");
}

void CalculatorForm::OnLongPressGestureDetected(
		TouchLongPressGestureDetector& gestureDetector) {
	AppLog("Long Press Detected");
//여기에 추가
	String PREVIOUS_KEY_EXPRESSTION;
	AppRegistry *appRegistry = Application::GetInstance()->GetAppRegistry();
	if(appRegistry->Get("KEY_EXPRESSTION",
			PREVIOUS_KEY_EXPRESSTION) != E_SUCCESS){
		appRegistry->Add("KEY_EXPRESSTION", "1234");
		appRegistry->Save();
	}

	appRegistry->Get("KEY_EXPRESSTION", PREVIOUS_KEY_EXPRESSTION);

	if(pExpression.Equals(PREVIOUS_KEY_EXPRESSTION))
		StartSensor();

}

void CalculatorForm::OnLongPressGestureCanceled(
		TouchLongPressGestureDetector& gestureDetector) {
}

void CalculatorForm::OnDataReceived(Tizen::Uix::Sensor::SensorType sensorType,
		Tizen::Uix::Sensor::SensorData& sensorData, result r) {
	switch (sensorType) {
	case SENSOR_TYPE_TILT: {
		TiltSensorData& tilt_data = static_cast<TiltSensorData&>(sensorData);

		if (SensorCnt >= 5) {
			Rawtiltpitch.Add(tilt_data.pitch);
			Rawtiltroll.Add(tilt_data.roll);
		}

		SensorCnt++;
		if (SensorCnt == 20) {
			StopSensor();
			SensorCnt = 0;
		}
		break;
	}
	default:
		break;
	} //End Switch
}

void CalculatorForm::StartSensor() {
	long interval = SENSOR_INTERVAL;
	__sensorMgr.AddSensorListener(*this, SENSOR_TYPE_TILT, interval, true);
}

void CalculatorForm::StopSensor() {
	AppLog("Deactivating SensorsAppAccelForm...");
	__sensorMgr.RemoveSensorListener(*this);
	FeatureProcessing();
}

void CalculatorForm::lowPass(
		const Tizen::Base::Collection::ArrayListT<float>& param,
		Tizen::Base::Collection::ArrayListT<float>& output) {
	float raw = 1;
	float last = 0;
	float filter = 0;

//앞에 구린 데이터를 처리하지 않도록
	param.GetAt(0, raw);
	output.Add(raw);

	for (int i = 1; i < param.GetCount(); i++) {
		//currrent data
		param.GetAt(i, raw);
		output.GetAt(i - 1, last);
		filter = last * (1.0f - LOWPASSWEIGHT) + raw * LOWPASSWEIGHT;
		output.Add(filter);
	}
	AppLog("OK lowPass");
}

float CalculatorForm::getMean(
		Tizen::Base::Collection::ArrayListT<float>& param) {
	float total = 0;
	float temp = 0;

	for (int i = 0; i < SAMPLE_WINDOWSIZE; i++) {
		param.GetAt(0, temp);
		param.RemoveAt(0);
		//하나씩 지우면서 처음꺼만 계속 뺌
		total += temp;
	}

	AppLog("OK getMean");
	return total / (float) SAMPLE_WINDOWSIZE;
}

void nullfunc(const char* in, ...) {

}

void CalculatorForm::clearBuffer() {
	AppLog("Clear Buffer...");

	Rawtiltroll.RemoveAll();
	Rawtiltpitch.RemoveAll();

	Filteredtiltpitch.RemoveAll();
	Filteredtiltroll.RemoveAll();

}

void CalculatorForm::FeatureProcessing() {

	std::vector<std::string> input;

	double *result_label = new double[3];
	int *retVal = new int[3];

	retVal[0]=0;
	retVal[1]=0;
	retVal[2]=0;

	int cast_label = 0;
	float mean_pitch, mean_roll;

	int port = 0;
	int land = 1;
	int reverse = 2;
	int orientation = 0;

	String label_history;

	//Orientation 을 Feature 1로 추가하는 부분
	OrientationStatus status = GetOrientationStatus();

	//portrait = 0, land = 1, reverse = 2
	if (status == ORIENTATION_STATUS_PORTRAIT
			|| status == ORIENTATION_STATUS_PORTRAIT_REVERSE) {
		orientation = port;
	} else if (status == ORIENTATION_STATUS_LANDSCAPE) {
		orientation = land;
	} else {
		orientation = reverse;
	}

	lowPass(Rawtiltpitch, Filteredtiltpitch);
	lowPass(Rawtiltroll, Filteredtiltroll);

	for (int i = 0; i < 3; i++) {

		mean_pitch = getMean(Filteredtiltpitch);
		mean_roll = getMean(Filteredtiltroll);

		std::string inputstr = string_format("1 1:%d 2:%f 3:%f", orientation,
				mean_pitch, mean_roll);
		input.push_back(inputstr);

		result_label[i] = svm.predict(input, nullfunc);
		cast_label = static_cast<int>(result_label[i]);
		retVal[cast_label]++;
		AppLog("retVal Count %d, %d, %d", retVal[0], retVal[1], retVal[2]);
		input.clear();
		//일단 history에 맞게 뜨는지 확인 후 최다빈출로 대표값 선정하는거 추가
	}
	clearBuffer();

	//가장 많이 관측된 값을 구하는 과정
	int min=-1, maxIndex=0;
	for(int i=0;i<3;i++)
	{
		if(retVal[i]>min)
		{
			min = retVal[i];
			maxIndex = i;
		}
	}

	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	switch(maxIndex)
	{
	case TILTSTOP:
		break;
	case TILTLEFT:
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_FILEBROWSER_MANAGEMENT));
		break;
	case TILTRIGHT:
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_FILEBROWSER_HIDDEN));
		break;
	}

	delete(result_label);
	delete(retVal);

}

#include <stdarg.h>  // for va_start, etc
std::string string_format(const std::string fmt, ...) {
	int size = ((int) fmt.size()) * 2 + 50; // use a rubric appropriate for your code
	std::string str;
	va_list ap;
	while (1) { // maximum 2 passes on a POSIX system...
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *) str.data(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) { // everything worked
			str.resize(n);
			return str;
		}
		if (n > -1) // needed size returned
			size = n + 1; // for null char
		else
			size *= 2; // guess at a larger size (o/s specific)
	}
	return str;
}

void CalculatorForm::OnSceneActivatedN(
		const Tizen::Ui::Scenes::SceneId& previousSceneId,
		const Tizen::Ui::Scenes::SceneId& currentSceneId,
		Tizen::Base::Collection::IList* pArgs) {

}
void CalculatorForm::OnSceneDeactivated(
		const Tizen::Ui::Scenes::SceneId& currentSceneId,
		const Tizen::Ui::Scenes::SceneId& nextSceneId) {
	__pLabelExpression->SetText("0");
}
