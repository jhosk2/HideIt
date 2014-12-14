/*
 * FolderBrowser.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: jowonbin
 */

#include "FolderBrowser.h"
#include <FMedia.h>
#include "Cryptor.h"

FolderBrowser::FolderBrowser(IconListView* listView) :
		listView(listView), pLabel(null), currentLocation(L"/opt/usr/media/"), HidecurrentLocation(HIDEITPATH),
		HideDefaultLocation(Tizen::App::App::GetInstance()->GetAppDataPath() + L"HideIt/"),
		popup(this), fileType(H_ALL){
	AppLog("FolderBrowser Tab2 Construct");
	GotoDirectory(currentLocation);

	String filepath = Tizen::App::App::GetInstance()->GetAppDataPath();

	Tizen::Media::Image img;
	img.Construct();

	pBitmapFolder = img.DecodeN(filepath + L"folder.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapImage = img.DecodeN(filepath + L"image.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapText = img.DecodeN(filepath + L"text.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapSound = img.DecodeN(filepath + L"sound.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapSound = img.DecodeN(filepath + L"video.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapMisc = img.DecodeN(filepath + L"misc.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);

	AppLog("FolderBrowser Tab2 Construct OK");
}

FolderBrowser::FolderBrowser(IconListView* listView, Label* pLabel) :
		listView(listView), pLabel(pLabel), currentLocation(L"/opt/usr/media/"), HidecurrentLocation(Tizen::App::App::GetInstance()->GetAppDataPath() + L"HideIt/"),
		HideDefaultLocation(Tizen::App::App::GetInstance()->GetAppDataPath() + L"HideIt/"),
		popup(this), fileType(H_ALL){

	AppLog("FolderBrowser Tab1 Construct");
	GotoDirectory(currentLocation);

	String filepath = Tizen::App::App::GetInstance()->GetAppDataPath();

	Tizen::Media::Image img;
	img.Construct();

	pBitmapFolder = img.DecodeN(filepath + L"folder.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapImage = img.DecodeN(filepath + L"image.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapText = img.DecodeN(filepath + L"text.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapSound = img.DecodeN(filepath + L"sound.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapSound = img.DecodeN(filepath + L"video.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	pBitmapMisc = img.DecodeN(filepath + L"misc.png",
			BITMAP_PIXEL_FORMAT_ARGB8888);
	AppLog("FolderBrowser Tab1 Construct OK");
}

FolderBrowser::~FolderBrowser() {
}

bool FolderBrowser::IsDirectory(String location) {
	Directory dir;
	result r = dir.Construct(location);
	if (IsFailed(r)) {
		return false;
	}
	return true;
}

bool FolderBrowser::GotoParent() {
	String parent;

	int lastIndexOfL;
	result r = currentLocation.LastIndexOf('/', currentLocation.GetLength() - 2,
			lastIndexOfL);
	if (r != E_SUCCESS) {
		return false;
	}

	if (lastIndexOfL == 8) {
		return false;
	}

	if (r != currentLocation.SubString(0, lastIndexOfL + 1, parent)) {
		return false;
	}

	GotoDirectory(parent);

	return true;
}

bool FolderBrowser::GotoHideParent() {
	String parent;

	int lastIndexOfL;
	AppLog("CurrentLocation = %S",HidecurrentLocation.GetPointer());

	result r = HidecurrentLocation.LastIndexOf('/', HidecurrentLocation.GetLength() - 2,
			lastIndexOfL);
	if (r != E_SUCCESS) {
		return false;
	}

	if (lastIndexOfL == 8) {
		return false;
	}

	if (r != HidecurrentLocation.SubString(0, lastIndexOfL + 1, parent)) {
		return false;
	}

	if(HidecurrentLocation.Equals(HideDefaultLocation))
	{
		AppLog("Do nothing");
	}
	else
	{
		AppLog("go next to parent");
		GotoDirectory(parent);
	}
	return true;
}

void FolderBrowser::GotoDirectoryItem(const DirEntry& targetFolder) {
	GotoDirectory(currentLocation + targetFolder.GetName() + L"/");

}

void FolderBrowser::OtherFolderInHideItFoler(const String& location ){
	GotoDirectory(Tizen::App::App::GetInstance()->GetAppDataPath() + "HideIt/" + location);
}

void FolderBrowser::ToHideItFolder() {
	GotoDirectory(Tizen::App::App::GetInstance()->GetAppDataPath() + "HideIt/");
}

void FolderBrowser::GotoDirectory(const String& targetFolder) {

	currentLocation = targetFolder;
	HidecurrentLocation = targetFolder;

	Directory* pDir = new Directory; // Allocates the %Directory instance

	// Open the directory
	result r = pDir->Construct(currentLocation);
	if (IsFailed(r)) {
		switch (r) {
		case E_SUCCESS:
			break;
		case E_INVALID_ARG:
			break;
		case E_ILLEGAL_ACCESS:
			break;
		case E_FILE_NOT_FOUND:
			break;
		case E_MAX_EXCEEDED:
			break;
		case E_IO:
			break;
		}
		return;
	}

	// Reads all the directory entries
	DirEnumerator* pDirEnum = pDir->ReadN();
	if (!pDirEnum) {
		return;
	}
	items.RemoveAll(true);
	// Loops through all the directory entries
	while (pDirEnum->MoveNext() == E_SUCCESS) {
		DirEntry entry = pDirEnum->GetCurrentDirEntry();
		if (entry.GetName().StartsWith(L".", 0))
			continue;
		if ( IsFiltered(entry))
			items.Add(*(new DirEntry(entry)));
	}

	// Releases the %Directory instance.
	// The opened directory is closed automatically when the destructor of the %Directory class is invoked.

	delete pDirEnum;

	delete pDir;
	pDir = null;

	if (pLabel != null){
		pLabel->SetText(currentLocation);
		pLabel->Invalidate(true);
	}
	listView->UpdateList();
}

void FolderBrowser::SetFilter(FileType type)
{
	fileType = type;
	GotoDirectory(currentLocation);
}

Tizen::Ui::Controls::IconListViewItem *
FolderBrowser::CreateItem(int index) {

	AppLog("CreateItem Start");
	// Creates an instance of IconListViewItem
	Tizen::Ui::Controls::IconListViewItem* pIconListviewItem =
			new Tizen::Ui::Controls::IconListViewItem();

	DirEntry* entry = reinterpret_cast<DirEntry*>(items.GetAt(index));
	FileAttributes attr;
	String fullPath = currentLocation + entry->GetName();
	result r = File::GetAttributes(fullPath, attr);
	if (IsFailed(r)) {
		return NULL;
	}

	String fileName = entry->GetName();

	Tizen::Graphics::Bitmap* bitmap;

	if (attr.IsDirectory()) {
		bitmap = pBitmapFolder;
	} else {

		String ext = File::GetFileExtension(fullPath);
		if (ext.Equals(L"png", false) || ext.Equals(L"bmp", false)
				|| ext.Equals(L"jpg", false) || ext.Equals(L"jpeg", false)
				|| ext.Equals(L"gif", false)) {
			bitmap = pBitmapImage;
		} else if (ext.Equals(L"txt", false) || ext.Equals(L"doc", false)
				|| ext.Equals(L"docx", false) || ext.Equals(L"hwp", false)) {
			bitmap = pBitmapText;
		} else if (ext.Equals(L"mp3", false) || ext.Equals(L"wav", false)
				|| ext.Equals(L"ogg", false)) {
			bitmap = pBitmapSound;
		} else if (ext.Equals(L"mp4", false) || ext.Equals(L"mkv", false)
				|| ext.Equals(L"avi", false)) {
			bitmap = pBitmapVideo;
		} else {
			bitmap = pBitmapMisc;
		}
	}

	pIconListviewItem->Construct(*bitmap, &fileName);
	AppLog("CreateeItem Start OK");
	return pIconListviewItem;
}

bool FolderBrowser::DeleteItem(int index,
		Tizen::Ui::Controls::IconListViewItem *pItem) {
	items.Remove(*pItem);
	delete pItem;
	return true;
}

int FolderBrowser::GetItemCount(void) {
	return items.GetCount();
}

void FolderBrowser::Refresh(){
	GotoDirectory(currentLocation);
}

void FolderBrowser::OnIconListViewItemStateChanged(
		Tizen::Ui::Controls::IconListView &iconListView, int index,
		Tizen::Ui::Controls::IconListViewItemStatus status) {
	switch (status) {
	case ICON_LIST_VIEW_ITEM_CHECKED:
		AppLog("checked");
		break;

	case ICON_LIST_VIEW_ITEM_UNCHECKED:
		AppLog("unchecked");
		break;

	case ICON_LIST_VIEW_ITEM_SELECTED: {
		DirEntry entry = *reinterpret_cast<DirEntry*>(items.GetAt(index));
		if (entry.IsDirectory()) {
			GotoDirectoryItem(entry);
		} else {
			popup.ShowPopup(currentLocation + entry.GetName());
			AppLog("!!!");
		}
		AppLog("selected");
	}
		break;

	case ICON_LIST_VIEW_ITEM_HIGHLIGHTED:
		AppLog("highlighted");
		break;
	}
}

bool FolderBrowser::IsFiltered( const DirEntry& entry ){

	String ext = File::GetFileExtension(entry.GetName());

	switch(fileType)
	{
	case H_IMAGE:
		if (ext.Equals(L"png", false) || ext.Equals(L"bmp", false)
					|| ext.Equals(L"jpg", false) || ext.Equals(L"jpeg", false)
					|| ext.Equals(L"gif", false))
			return true;
		break;
	case H_VIDEO:
		if (ext.Equals(L"mp4", false) || ext.Equals(L"mkv", false)
			|| ext.Equals(L"avi", false))
			return true;
		break;
	case H_ETC:
		if (ext.Equals(L"png", false) || ext.Equals(L"bmp", false)
							|| ext.Equals(L"jpg", false) || ext.Equals(L"jpeg", false)
							|| ext.Equals(L"gif", false))
			return false;

		if (ext.Equals(L"mp4", false) || ext.Equals(L"mkv", false)
			|| ext.Equals(L"avi", false))
			return false;
		return true;
		break;
	case H_ALL:
		return true;
		break;
	}
	return false;
}

