//---------------------------------------------------------------------------

#ifndef ffrmLoginH
#define ffrmLoginH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <OleCtrls.hpp>
#include <SHDocVw.hpp>
#include <ComCtrls.hpp>

#include "ffrmBot.h"
#include <ExtCtrls.hpp>

void BanTrialID();
void MakeCaption();
extern int iRegisterTryCount;
extern long iRegisterTryTick;

extern int iLoginTryCount;
extern long iLoginTryTick;

//---------------------------------------------------------------------------
class TfrmLogin : public TForm
{
__published:	// IDE-managed Components
	TPageControl *pageMain;
	TTabSheet *tabLogin;
	TTabSheet *tabRegister;
	TGroupBox *grpLogin;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *txtloginid;
	TEdit *txtloginpw;
	TButton *btnLogin;
	TGroupBox *grpregister;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *txtKey;
	TEdit *txtregisterid;
	TEdit *txtregisterpw;
	TEdit *txtregisterpw2;
	TButton *btnRegister;
	TTabSheet *tabadd;
	TGroupBox *GroupBox1;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TEdit *txtaddkey;
	TEdit *txtaddid;
	TEdit *txtaddpw;
	TButton *btnAdd;
	TCheckBox *chload;
	TButton *btntrial;
	TTimer *tmloginbot;
	TCheckBox *chinterclient;
	TTabSheet *tabchange;
	TGroupBox *grpchange;
	TLabel *Label10;
	TEdit *txtchangekey;
	TLabel *Label11;
	TEdit *txtchangepw;
	TEdit *txtchangenewpw;
	TLabel *Label12;
	TEdit *txtchangenewpw2;
	TLabel *Label13;
	TButton *btnchange;
	TTimer *tmdebugger;
	void __fastcall btnLoginClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnRegisterClick(TObject *Sender);
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall chloadClick(TObject *Sender);
	void __fastcall btntrialClick(TObject *Sender);
	void __fastcall tmloginbotTimer(TObject *Sender);
	void __fastcall chinterclientClick(TObject *Sender);
	void __fastcall btnchangeClick(TObject *Sender);
	void __fastcall tmdebuggerTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
#endif
