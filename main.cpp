#include <wx/wx.h>
#include "blueprint.h"

const char* pBluePrint =
  "0eJydlt2KgzAQRl9F5jqCSdW2vsqyLP5Ml4CNkqTLivjua2qhRezGzJWYmJMvkxzMCFV7w15LZaEYQdadMlB8jGDktypb12aHHqEAafEKDFR"
  "5dW+X0tjY6lKZvtM2rrC1MDGQqsFfKPj0yQCVlVbigvtnGIO+M/OXnXKzzaNjwWBwjxnYSI310pc/kMOXul0r1G4aFkjmO8kilJzsBB9CwXs"
  "Tp6HgvUXOgov8IHMfOadun5d8JGZOfOATMbIXfCaeC28peEI8yn5ysH7PWqxRRN+8ZeXBvr0c3jWLqNjWesOlWmrn3xSqVGIjJVUjf8pgj57"
  "LX6OI5mysVxBV2Uglgt1I3qYKdkO8TUW0YStV6n7x9ztB8XKFYNCWM2luqzutUEcam8h2UYsX1/mD2tzhXBzSLMuPp3MyTX8brtvl";

class MyApp : public wxApp {
public:
  virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
  MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
  void OnPasteBlueprint(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);

  wxDECLARE_EVENT_TABLE();
};

enum {
  ID_PasteBlueprint = 1,
  ID_GetBlueprint   = 2
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_PasteBlueprint, MyFrame::OnPasteBlueprint)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP_CONSOLE(MyApp);

bool MyApp::OnInit() {
  MyFrame *frame = new MyFrame("Factorio Blueprint Reader", wxPoint(50, 50), wxSize(450, 340));
  frame->Show(true);

  printf("Factorio Blueprint Reader started.\n");
  return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size) {
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(ID_PasteBlueprint, "&Paste Blueprint\tCtrl-P", "Paste a blueprint");

  wxMenuItem* pGetBpItem = new wxMenuItem(menuFile, ID_GetBlueprint, "&Get Blueprint\tCtrl-C", "Get blueprint");
  menuFile->Append(pGetBpItem);
  pGetBpItem->Enable(false);

  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");

  SetMenuBar(menuBar);
  CreateStatusBar();
  SetStatusText("Ready. Paste a blueprint to start.");
}

void MyFrame::OnExit(wxCommandEvent& event) {
  Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event) {
  wxMessageBox("Made by coon\n\n"
               "E-Mail: coon@mailbox.org\n"
               "IRC: coon@freenode.org\n\n"
               "https://www.reddit.com/user/coon42\nhttps://github.com/coon42\n",
               "About Factorio Blueprint Reader", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnPasteBlueprint(wxCommandEvent& event) {
  Blueprint bp;

  if (!bp.load(pBluePrint))
    printf("Error on loading blueprint.\n");

  printf(bp.toJsonStr());

  wxLogMessage("Coming soon!");
}
