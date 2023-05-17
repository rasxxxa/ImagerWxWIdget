#include "Frame.h"
#include "../Imager/RawImageHandler.h"

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "HELLO WORLD", wxPoint(0, 0), wxSize(800,800))
{
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);

	wxMenu* menuOpenFile = new wxMenu;
	menuOpenFile->Append(wxID_OPEN);

	wxMenu* menuFilters = new wxMenu;
	//menuFilters->Append(wxID_FILE1);
	menuFilters->Append(Filter1, "&Filter 1");
	menuFilters->Append(Filter2, "&Filter 2");
	menuFilters->Append(Filter3, "&Filter 3");
	menuFilters->Append(Filter4, "&Filter 4");


	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	menuBar->Append(menuOpenFile, "&Open File");
	menuBar->Append(menuFilters, "&Filters");

	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Imager");

	Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
	Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_MENU, &MyFrame::OpenFile, this, wxID_OPEN);

	m_imagePanel = new wxPanel(this);
}


void MyFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello wxWidget");
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("This is a wxWidget Hello world example", "About hello world", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void MyFrame::OnClickHandler(wxCommandEvent& event)
{
	
}

void MyFrame::OpenFile(wxCommandEvent& event)
{
	wxString wildcard = "Image Files (*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp";
	wxFileDialog openFileDialog(this, "Open File", "", "", wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	// Show the file dialog
	if (openFileDialog.ShowModal() == wxID_OK)
	{
		// Get the selected file path
		wxString filePath = openFileDialog.GetPath();
		m_mainImageData = m_mainImageHandler.LoadImageFile(filePath.c_str().operator const char *());
		
		m_mainImage = wxImage((int)m_mainImageData.m_width, (int)m_mainImageData.m_height, (unsigned char*)m_mainImageData.m_imageData.data(), true);
		m_mainImage.Rescale(600, 600, wxIMAGE_QUALITY_HIGH);
		m_mainBitmap = wxBitmap(m_mainImage);
		m_staticBitmap = new wxStaticBitmap(this, wxID_ANY, m_mainBitmap);
		m_staticBitmap->SetSize(600, 600);
		m_staticBitmap->SetPosition(wxPoint(0, 0));

		// Resize the panel to fit the static bitmap
		m_imagePanel->SetSize(m_staticBitmap->GetSize());

		// Set the panel as the main window's sizer
		SetSizer(new wxBoxSizer(wxVERTICAL));
		GetSizer()->Add(m_imagePanel, 1, wxEXPAND);
	}
}

