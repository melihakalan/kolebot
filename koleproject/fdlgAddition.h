//----------------------------------------------------------------------------
#ifndef fdlgAdditionH
#define fdlgAdditionH
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
class TdlgAddition : public TForm
{
__published:
	TGroupBox *grpbdw;
	TButton *btngetmonuid;
	TLabel *lbmonu;
	TButton *btngetmonu;
	TCheckBox *chautomonu;
	TButton *btnclose;
	TTimer *tmmonu;
	TGroupBox *grpchaos;
	TCheckBox *chdisablesaw;
	TCheckBox *chlootcubes;
	TTimer *tmlootcubes;
	TTimer *tmchaoscheck;
	void __fastcall btncloseClick(TObject *Sender);
	void __fastcall btngetmonuidClick(TObject *Sender);
	void __fastcall btngetmonuClick(TObject *Sender);
	void __fastcall chautomonuClick(TObject *Sender);
	void __fastcall tmmonuTimer(TObject *Sender);
	void __fastcall chdisablesawClick(TObject *Sender);
	void __fastcall chlootcubesClick(TObject *Sender);
	void __fastcall tmchaoscheckTimer(TObject *Sender);
	void __fastcall tmlootcubesTimer(TObject *Sender);
private:
public:
	DWORD dwMonuID;
	bool  bDisableSaw;
	virtual __fastcall TdlgAddition(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TdlgAddition *dlgAddition;
//----------------------------------------------------------------------------
#endif    
