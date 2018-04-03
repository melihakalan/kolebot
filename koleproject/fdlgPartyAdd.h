//----------------------------------------------------------------------------
#ifndef fdlgPartyAddH
#define fdlgPartyAddH
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
class TdlgPartyAdd : public TForm
{
__published:
	TButton *btnOk;
	TBevel *pnPartyAdd;
	TListBox *lstpartyadd;
	TEdit *txtname;
	TButton *btnadd;
	TButton *btnclear;
	TButton *btnprintparty;
	TButton *btnaddselected;
	TGroupBox *grpautoadd;
	TEdit *txtautoadd;
	TCheckBox *chautochatadd;
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall chautochataddClick(TObject *Sender);
	void __fastcall btnaddClick(TObject *Sender);
	void __fastcall btnclearClick(TObject *Sender);
	void __fastcall btnaddselectedClick(TObject *Sender);
	void __fastcall btnprintpartyClick(TObject *Sender);
	void __fastcall txtautoaddChange(TObject *Sender);
private:
public:
	virtual __fastcall TdlgPartyAdd(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TdlgPartyAdd *dlgPartyAdd;
//----------------------------------------------------------------------------
#endif    
