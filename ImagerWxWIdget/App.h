#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // !WX_PRECOMP


class App : public wxApp
{
public:
	virtual bool OnInit() override;
	static void Run(int argc, char** argv)
	{
		wxApp::SetInstance(new App());
		auto instance = wxApp::GetInstance();
		wxEntryStart(argc, argv);
		instance->CallOnInit();
		instance->OnRun();
		instance->OnExit();
		wxEntryCleanup();
	}

};