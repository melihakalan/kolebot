//---------------------------------------------------------------------------

#ifndef _frmLoaderH
#define _frmLoaderH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <OleCtrls.hpp>
#include <SHDocVw.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "Tlhelp32.h"
#define _WIN32_WINNT  0x0501
#include "Windows.h"
#include "Winternl.h"
#include "Psapi.h"

extern char* DLLNAME;
extern char* strWebAddress;

extern char* strKODIR;

extern char strKOWND[25];
extern long lLastTick;

extern DWORD dwDebugAPI;

void HandleProcess();
void LoadProcess();
void ClearLogFiles();

const DWORD KO_WNDA = 	0xA69180;
const DWORD KO_APRA =	0xC3AEA8;//
const DWORD KO_APRB =	0xC3AEB8;//
const DWORD KO_APRC =	0xC347B4;//

void StrEncrypt(char* pStr, BYTE bUseKey);
void StrDecrypt(char* pStr);

void GamePatch(HANDLE hProcess);

void CheckDLL();
void CheckDebugger();

bool CheckCommandLine();
void EditDbgBreakPoint();

void MakeRandomCaption();

extern bool bCheckDebugEntry;

//---------------------------------------------------------------------------
class TfrmLoader : public TForm
{
__published:	// IDE-managed Components
	TWebBrowser *webMain;
	TPageControl *pageMain;
	TTabSheet *tabauto;
	TOpenDialog *dlgPath;
	TTabSheet *tabmanual;
	TLabel *Label1;
	TEdit *txtPath;
	TLabel *Label2;
	TRadioButton *oprandom;
	TRadioButton *opmanual;
	TEdit *txtmanualwnd;
	TButton *btnLoad2;
	TEdit *txtWindow;
	TButton *btnLoad;
	TTabSheet *tabotologin;
	TCheckBox *chotologin;
	TLabel *Label3;
	TEdit *txtgameid;
	TLabel *Label4;
	TEdit *txtgamepw;
	TEdit *txtbotid;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *txtbotpw;
	TLabel *Label7;
	TRadioButton *opleft;
	TRadioButton *opcenter;
	TRadioButton *opright;
	TLabel *Label8;
	TLabel *Label9;
	TComboBox *comserverlist;
	TComboBox *comserverno;
	TTimer *tmautologin;
	TCheckBox *chtrial;
	TTabSheet *tabsave;
	TButton *btnsave;
	TTrayIcon *tray;
	TTimer *tmdebugger;
	void __fastcall btnLoadClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnLoad2Click(TObject *Sender);
	void __fastcall tmautologinTimer(TObject *Sender);
	void __fastcall chtrialClick(TObject *Sender);
	void __fastcall btnsaveClick(TObject *Sender);
	void __fastcall trayClick(TObject *Sender);
	void __fastcall tmdebuggerTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmLoader(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLoader *frmLoader;
//---------------------------------------------------------------------------
#endif
