#include <wx/wx.h>
#include "blueprint.h"
#include "main.h"

enum {
  ID_PasteBlueprint = 1,
  ID_GetBlueprint   = 2
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(ID_PasteBlueprint, MainFrame::OnPasteBlueprint)
EVT_MENU(wxID_EXIT, MainFrame::OnExit)
EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP_CONSOLE(WxApp);

bool WxApp::OnInit() {
  MainFrame *frame = new MainFrame("Factorio Blueprint Reader", wxPoint(50, 50), wxSize(450, 340));
  frame->Show(true);

  printf("Factorio Blueprint Reader started.\n");
  return true;
}

//--------------------------------------------------------------------------------------------------------------
// MainFrame
//--------------------------------------------------------------------------------------------------------------

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
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

void MainFrame::OnPasteBlueprint(wxCommandEvent& event) {
  PasteDialog pasteDialog(this, -1, _("Paste a blueprint"),
    wxPoint(320, 240), wxSize(640, 480));
  if (pasteDialog.ShowModal() != wxID_OK) {
    SetStatusText("Pasting a blueprint was canceled.");

    return;
  }

  SetStatusText("A blueprint was pasted.");
  string bluePrintStr = pasteDialog.getBlueprintStr();

  Blueprint bp;
  if (!bp.load(bluePrintStr)) {
    printf("Error on loading blueprint.\n");

    return;
  }

  printf(bp.toJsonStr());
}

void MainFrame::OnExit(wxCommandEvent& event) {
  Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
  wxMessageBox("Made by coon\n\n"
               "E-Mail: coon@mailbox.org\n"
               "IRC: coon@freenode.org\n\n"
               "https://www.reddit.com/user/coon42\nhttps://github.com/coon42\n",
               "About Factorio Blueprint Reader", wxOK | wxICON_INFORMATION);
}

wxBEGIN_EVENT_TABLE(PasteDialog, wxDialog)
EVT_BUTTON(wxID_OK, PasteDialog::onOk)
EVT_BUTTON(wxID_CANCEL, PasteDialog::onCancel)
EVT_IDLE(PasteDialog::onIdle)
wxEND_EVENT_TABLE()

//--------------------------------------------------------------------------------------------------------------
// PasteDialog
//--------------------------------------------------------------------------------------------------------------

PasteDialog::PasteDialog(wxWindow * parent, wxWindowID id, const wxString & title,
    const wxPoint & position, const wxSize & size, long style) : wxDialog(parent, id, title, position, size, style) {

  wxSize  sz;
  sz.SetWidth(size.GetWidth() - 20);
  sz.SetHeight(size.GetHeight() - 80);

  wxString text = "0eJydlt2KgzAQRl9F5jqCSdW2vsqyLP5Ml4CNkqTLivjua2qhRezGzJWYmJMvkxzMCFV7w15LZaEYQdadMlB8jGDkt"
    "ypb12aHHqEAafEKDFR5dW+X0tjY6lKZvtM2rrC1MDGQqsFfKPj0yQCVlVbigvtnGIO+M/OXnXKzzaNjwWBwjxnYSI310pc/kMOXul0r1G4"
    "aFkjmO8kilJzsBB9CwXsTp6HgvUXOgov8IHMfOadun5d8JGZOfOATMbIXfCaeC28peEI8yn5ysH7PWqxRRN+8ZeXBvr0c3jWLqNjWesOlW"
    "mrn3xSqVGIjJVUjf8pgj57LX6OI5mysVxBV2Uglgt1I3qYKdkO8TUW0YStV6n7x9ztB8XKFYNCWM2luqzutUEcam8h2UYsX1/mD2tzhXBz"
    "SLMuPp3MyTX8brtvl";

  wxPoint p;
  p.x = 6;
  p.y = 2;

  dialogText = new wxTextCtrl(this, -1, text, p, sz, wxTE_MULTILINE);

  p.y += sz.GetHeight() + 10;
  wxButton * b = new wxButton(this, wxID_OK, _("OK"), p, wxDefaultSize);

  p.x += 110;
  wxButton * c = new wxButton(this, wxID_CANCEL, _("Cancel"), p, wxDefaultSize);
}

void PasteDialog::onOk(wxCommandEvent & event) {
  EndModal(wxID_OK);
}

void PasteDialog::onCancel(wxCommandEvent & event) {
  EndModal(wxID_CANCEL);
}

void PasteDialog::onIdle(wxIdleEvent & event) {
  dialogText->SetSelection(-1, -1);
}
