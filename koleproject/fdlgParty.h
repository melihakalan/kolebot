//----------------------------------------------------------------------------
#ifndef fdlgPartyH
#define fdlgPartyH
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
class TdlgParty : public TForm
{
__published:
	TBevel *pnPartyAdd;
	TButton *btnOk;
	TListBox *lstpartyadd;
	TEdit *txtname;
	TButton *btnadd;
	TButton *btnclear;
	TButton *btnprintparty;
	TButton *btnaddselected;
	TGroupBox *grpautoadd;
	TEdit *txtautoadd;
	TCheckBox *chautochatadd;
	TCheckBox *chaccept;
	TCheckBox *chrequest;
	void __fastcall btnprintpartyClick(TObject *Sender);
	void __fastcall btnaddselectedClick(TObject *Sender);
	void __fastcall btnaddClick(TObject *Sender);
	void __fastcall btnclearClick(TObject *Sender);
	void __fastcall chautochataddClick(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
private:
public:
	bool bAccept;
	bool bRequest;
	virtual __fastcall TdlgParty(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TdlgParty *dlgParty;
//----------------------------------------------------------------------------
#endif    
