#include "App.h"
#include "Frame.h"

bool App::OnInit()
{
	MyFrame* frame = new MyFrame();
	frame->Show(true);
	return true;
}