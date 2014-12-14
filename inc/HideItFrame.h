#ifndef _HIDE_IT_FRAME_H_
#define _HIDE_IT_FRAME_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

class HideItFrame
	: public Tizen::Ui::Controls::Frame
{
public:
	HideItFrame(void);
	virtual ~HideItFrame(void);

private:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
};

#endif	//_HIDE_ITFRAME_H_
