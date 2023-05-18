#include "Frame.h"
#include "../Imager/RawImageHandler.h"

#include "wx/mstream.h"

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "HELLO WORLD", wxPoint(0, 0), wxSize(800,800))
{
	wxInitAllImageHandlers();
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
	menuFilters->Append(Filter1, "&Gray Scale filter");
	menuFilters->Append(Filter2, "&Black and white");
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
	Bind(wxEVT_MENU, &MyFrame::RunFilter, this, Filter1);
	Bind(wxEVT_MENU, &MyFrame::RunFilter, this, Filter2);
	Bind(wxEVT_MENU, &MyFrame::RunFilter, this, Filter3);
	Bind(wxEVT_MENU, &MyFrame::RunFilter, this, Filter4);

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
		SetImage();
	}
}

void MyFrame::RunFilter(wxCommandEvent& event)
{
	if (m_mainImageData.m_imageData.empty())
	{
		std::cout << "Empty image " << std::endl;
		assert(false);
		return;
	}
	switch (event.GetId())
	{
	case Filter1: 
	{
		std::cout << &m_mainImageData.m_imageData<< std::endl;
		ThreadDivisionOfFunction(std::ref(m_mainImageData), false, 40, nullptr, GrayFilter);
	}break;
	case Filter2: {
	
		ThreadDivisionOfFunction(std::ref(m_mainImageData), false, 1, nullptr, BlackAndWhite);
	
	}break;
	case Filter3: {
	
		ThreadDivisionOfFunction(std::ref(m_mainImageData), true, 1, Pixelate, nullptr);
	
	}break;
	case Filter4: {}break;
	default:
		break;
	}
	m_mainImageHandler.WriteImage(m_mainImageData);
	SetImage();
}

#include <filesystem>

void MyFrame::SetImage()
{
	auto x = m_mainImageHandler.SeparateAlpha(m_mainImageData);
	m_mainImage = wxImage(m_mainImageData.m_width, m_mainImageData.m_height, x.first.data(), x.second.data(), true); // ANY => can load many image formats
	//auto res = m_mainImage.LoadFile(m_mainImageData.path, wxBITMAP_TYPE_PNG_RESOURCE);

	m_mainImage.Rescale(600, 600, wxIMAGE_QUALITY_HIGH);
	if (m_staticBitmap)
		delete m_staticBitmap;

	m_mainBitmap = wxBitmap(m_mainImage);
	m_staticBitmap = new wxStaticBitmap(this, wxID_ANY, m_mainBitmap);
	m_staticBitmap->SetSize(600, 600);
	m_staticBitmap->SetPosition(wxPoint(0, 0));
	m_staticBitmap->SetTransparent(wxByte(255));
	// Resize the panel to fit the static bitmap
	m_imagePanel->SetSize(m_staticBitmap->GetSize());

	// Set the panel as the main window's sizer
	SetSizer(new wxBoxSizer(wxVERTICAL));
	GetSizer()->Add(m_imagePanel, 1, wxEXPAND);
	Refresh(true);
}

