/*
 * ShowTerminatePopup.h
 *
 *  Created on: Dec 8, 2014
 *
 */

#ifndef TERMINATEPOPUP_H_
#define TERMINATEPOPUP_H_
#include <FUi.h>


class PopupEventListenerTerminate
    : public Tizen::Ui::IPropagatedKeyEventListener
{
    // key events for back-key
    virtual bool OnKeyPressed(Tizen::Ui::Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo) { return false; };
    virtual bool OnKeyReleased(Tizen::Ui::Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
    virtual bool OnPreviewKeyPressed(Tizen::Ui::Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo) { return false; };
    virtual bool OnPreviewKeyReleased(Tizen::Ui::Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo) { return false; };
    virtual bool TranslateKeyEventInfo(Tizen::Ui::Control& source, Tizen::Ui::KeyEventInfo& keyEventInfo) { return false; };

};

class ShowTerminatePopup :
	public Tizen::Ui::IActionEventListener{
public:
	ShowTerminatePopup();
	virtual ~ShowTerminatePopup();

    void ShowPopup(void);
    void HidePopup(void);

    virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

private:
    static const int ID_BUTTON_TERMINATE = 7001;
    static const int ID_BUTTON_CANCEL =7002;

    Tizen::Ui::Controls::Popup* __pPopup_Terminate;
    PopupEventListenerTerminate* __pPopupListener_Terminate;
};


#endif /* TERMINATEPOPUP_H_ */
