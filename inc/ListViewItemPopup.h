/*
 * FileBrowserPopup.h
 *
 *  Created on: Dec 1, 2014
 *      Author: 원빈
 */

#ifndef FILEBROWSERPOPUP_H_
#define FILEBROWSERPOPUP_H_
#include <FUi.h>

class FolderBrowser;

extern int FILE_STATE_FLAG;
extern const int FILESTATE_ENCRYPTION;
extern const int FILESTATE_DECRYPTION;

class PopupEventListener
    : public Tizen::Ui::IPropagatedKeyEventListener
{
    // key events for back-key
    virtual bool OnKeyPressed(Tizen::Ui::Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo) { return false; };
    virtual bool OnKeyReleased(Tizen::Ui::Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
    virtual bool OnPreviewKeyPressed(Tizen::Ui::Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo) { return false; };
    virtual bool OnPreviewKeyReleased(Tizen::Ui::Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo) { return false; };
    virtual bool TranslateKeyEventInfo(Tizen::Ui::Control& source, Tizen::Ui::KeyEventInfo& keyEventInfo) { return false; };

};

class ListViewItemPopup :
	public Tizen::Ui::IActionEventListener{
public:
	ListViewItemPopup(FolderBrowser* folderBrowser);
	virtual ~ListViewItemPopup();

    void ShowPopup(const Tizen::Base::String& filename);
    void HidePopup(void);

    virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

private:
    void ListViewItemPopup::ExecuteTextFile(Tizen::Base::String filePath);

    FolderBrowser* pFolderBrowser;

    static const int ID_BUTTON_OPEN_POPUP = 501;
    static const int ID_BUTTON_CLOSE_POPUP = 502;
    static const int ID_BUTTON_ENCRYPTION_FILE = 503;
    static const int ID_BUTTON_OPEN_FILE = 504;

    static const int FILESTATE_ENCRYPTION = 601;
    static const int FILESTATE_DECRYPTION = 602;

    Tizen::Ui::Controls::Popup* __pPopup;
    PopupEventListener* __pPopupListener;
    Tizen::Base::String currentFilePath;
};


#endif /* FILEBROWSERPOPUP_H_ */
