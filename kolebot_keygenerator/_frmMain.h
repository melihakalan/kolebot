//---------------------------------------------------------------------------

#ifndef _frmMainH
#define _frmMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------

BYTE HexToByte(char* strHex);
short HexToShort(char* strHex);
long HexToLong(char* strHex);

void Generate(char* Buffer);

class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TButton *btngenerate;
	TEdit *txtKey;
	TButton *btngenerateall;
	TLabel *Label1;
	TEdit *txtlimit;
	TLabel *Label2;
	TEdit *txtcount;
	TGroupBox *GroupBox1;
	TEdit *txtKey2;
	TButton *btnanaliz;
	TLabel *Label3;
	TEdit *txtrand1;
	TLabel *Label4;
	TEdit *txtdays;
	TEdit *txtrand2;
	TLabel *Label9;
	TLabel *Label12;
	TEdit *txtkeyno;
	TEdit *txtkeyno2;
	TLabel *Label13;
	TLabel *Label5;
	TEdit *txtlkey;
	TButton *btnsavekey;
	void __fastcall btngenerateClick(TObject *Sender);
	void __fastcall btngenerateallClick(TObject *Sender);
	void __fastcall btnanalizClick(TObject *Sender);
	void __fastcall btnsavekeyClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
