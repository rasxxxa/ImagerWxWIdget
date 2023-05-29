#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // !WX_PRECOMP

#include <vector>
#include <memory>
#include "../Imager/RawImageHandler.h"

import Filters;

enum
{
	ID_Hello = 1,
	ID_OpenFile = 2,
	Filter1 = 3,
	Filter2 = 4,
	Filter3 = 5,
	Filter4 = 6
};

class MyFrame : public wxFrame
{
public:
	MyFrame();
private:
	wxSpinCtrl* c1, * c2, *c3;
	wxImage m_mainImage;
	wxBitmap m_mainBitmap;
	wxStaticBitmap* m_staticBitmap;
	wxPanel* m_imagePanel;
	RawImageHandler m_mainImageHandler{};
	Image m_mainImageData{};
	void OnHello(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnClickHandler(wxCommandEvent& event);
	void OpenFile(wxCommandEvent& event);
	void RunFilter(wxCommandEvent& event);
	void SetImage(Image* image = nullptr);
};