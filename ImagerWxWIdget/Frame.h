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
	Filter4 = 6,
	Option1 = 1000,
	Option2,
	Option3,
	Option4,
	Save,
};

class MyFrame : public wxFrame
{
public:
	MyFrame();
private:
	std::vector<std::pair<wxSlider*, int>> m_sliders;
	wxImage m_mainImage;
	wxBitmap m_mainBitmap;
	wxStaticBitmap* m_staticBitmap;
	wxPanel* m_imagePanel;
	RawImageHandler m_mainImageHandler{};
	Image m_mainImageData{};
	void SliderChanged(wxCommandEvent& event);
	void OnHello(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnClickHandler(wxCommandEvent& event);
	void OpenFile(wxCommandEvent& event);
	void SaveFile(wxCommandEvent& event);
	void RunFilter(wxCommandEvent& event);
	void SetImage(Image* image = nullptr);
};