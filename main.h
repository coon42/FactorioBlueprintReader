#ifndef MAIN_H__
#define MAIN_H__


class WxApp : public wxApp {
public:
  virtual bool OnInit();
};

class MainFrame : public wxFrame {
public:
  MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
  void OnPasteBlueprint(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);

  wxDECLARE_EVENT_TABLE();
};

class PasteDialog : public wxDialog {
public:
  PasteDialog(wxWindow * parent, wxWindowID id, const wxString & title, const wxPoint & pos = wxDefaultPosition,
      const wxSize & size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

  wxTextCtrl* dialogText;
  wxString getBlueprintStr()                                      { return dialogText->GetValue(); }

private:
  void onOk(wxCommandEvent & event);
  void onCancel(wxCommandEvent & event);
  void onIdle(wxIdleEvent & event);

  DECLARE_EVENT_TABLE();
};


#endif // MAIN_H__
