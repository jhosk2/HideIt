/*
 * FolderBrowser.h
 *
 *  Created on: Nov 27, 2014
 *      Author: jowonbin
 */

#ifndef FOLDERBROWSER_H_
#define FOLDERBROWSER_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FIo.h>
#include <FUi.h>
#include <FSecurity.h>

#include "ListViewItemPopup.h"

using namespace Tizen::Security;
using namespace Tizen::Security::Crypto;
using namespace Tizen::Io;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Utility;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

#define HIDEITPATH Tizen::App::App::GetInstance()->GetAppDataPath() + L"HideIt/"
#define TEMPPATH "/opt/media/.temp."

typedef enum filetype
{
	H_TEXT,
	H_IMAGE,
	H_VIDEO,
	H_ETC,
	H_ALL,
} FileType;

class FolderBrowser :
 	public Tizen::Ui::Controls::IIconListViewItemProvider,
 	public Tizen::Ui::Controls::IIconListViewItemEventListener
 {
public:
	FolderBrowser(IconListView*);
	FolderBrowser(IconListView*, Label*);
	virtual ~FolderBrowser();

	void GotoDirectoryItem( const DirEntry& entry );
	bool GotoParent();
	bool GotoHideParent();

	bool IsDirectory( String location );

	void Refresh();

	void ToHideItFolder();
	void OtherFolderInHideItFoler(const String& location );

	void SetFilter(FileType type);

private:
	void GotoDirectory( const String& location );
	bool IsFiltered( const DirEntry& entry );

	Tizen::Ui::Controls::IconListViewItem * 	CreateItem(int index);
	bool 	DeleteItem(int index, Tizen::Ui::Controls::IconListViewItem *pItem);
	int 	GetItemCount(void);

	void  OnIconListViewItemStateChanged (Tizen::Ui::Controls::IconListView &iconListView, int index, Tizen::Ui::Controls::IconListViewItemStatus status);

	Tizen::Graphics::Bitmap* pBitmapFolder;
	Tizen::Graphics::Bitmap* pBitmapUp;
	Tizen::Graphics::Bitmap* pBitmapImage;
	Tizen::Graphics::Bitmap* pBitmapText;
	Tizen::Graphics::Bitmap* pBitmapSound;
	Tizen::Graphics::Bitmap* pBitmapVideo;
	Tizen::Graphics::Bitmap* pBitmapMisc;

	IconListView* listView;
	Label* pLabel;
	String currentLocation;
	String HidecurrentLocation;
	String HideDefaultLocation;
	ArrayList items;

	FileType fileType;
	ListViewItemPopup popup;
};


#endif /* FOLDERBROWSER_H_ */
