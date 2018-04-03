//----------------------------------------------------------------------------
#ifndef fdlgInterClientH
#define fdlgInterClientH
//----------------------------------------------------------------------------
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
//----------------------------------------------------------------------------
class TdlgInterClient : public TForm
{
__published:
	TGroupBox *grpinterclient;
	TButton *btnmakeparty;
	TButton *btnstartall;
	TButton *btnstopall;
	TButton *btnaddmob;
	TButton *btntown;
	TButton *btnsit;
	TButton *btnstand;
	TButton *btnexit;
	TButton *btnwear;
	TButton *btnexpand;
	TButton *btnfollow;
	TButton *btnnofollow;
	TButton *btnsave;
	TLabel *Label1;
	TEdit *txtexpand;
	TButton *btnsuicide;
	TButton *btnclose;
	TButton *btncall;
	TCheckBox *chstick;
	TButton *btnrota;
	void __fastcall btnstartallClick(TObject *Sender);
	void __fastcall btnstopallClick(TObject *Sender);
	void __fastcall btnmakepartyClick(TObject *Sender);
	void __fastcall btnaddmobClick(TObject *Sender);
	void __fastcall btnwearClick(TObject *Sender);
	void __fastcall btnfollowClick(TObject *Sender);
	void __fastcall btnnofollowClick(TObject *Sender);
	void __fastcall btntownClick(TObject *Sender);
	void __fastcall btnsitClick(TObject *Sender);
	void __fastcall btnstandClick(TObject *Sender);
	void __fastcall btncloseClick(TObject *Sender);
	void __fastcall btnexpandClick(TObject *Sender);
	void __fastcall btnsuicideClick(TObject *Sender);
	void __fastcall btnsaveClick(TObject *Sender);
	void __fastcall btnexitClick(TObject *Sender);
	void __fastcall btncallClick(TObject *Sender);
	void __fastcall chstickClick(TObject *Sender);
	void __fastcall btnrotaClick(TObject *Sender);
private:
public:
	virtual __fastcall TdlgInterClient(TComponent* AOwner);

	bool bLockClick;
};
//----------------------------------------------------------------------------
extern PACKAGE TdlgInterClient *dlgInterClient;
//----------------------------------------------------------------------------
#endif    
