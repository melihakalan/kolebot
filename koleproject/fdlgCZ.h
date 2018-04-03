//----------------------------------------------------------------------------
#ifndef fdlgCZH
#define fdlgCZH
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
class TdlgCZ : public TForm
{
__published:
	TGroupBox *grpsuicide;
	TCheckBox *chsuicide1;
	TCheckBox *chsuicide2;
	TEdit *txtsuicidelimit;
	TGroupBox *grpflee;
	TCheckBox *chflee;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TListBox *lstflee;
	TButton *btnfleeadd;
	TButton *btnfleeclear;
	TLabel *Label4;
	TEdit *txtfleewait;
	TCheckBox *chfleetown;
	TCheckBox *chfleering;
	TButton *btnclose;
	TTimer *tmsuicide;
	TGroupBox *grpother;
	TCheckBox *chstoponnopt;
	TCheckBox *chleaveondie;
	TCheckBox *chstartinpt;
	TTimer *tmptcheck;
	TTimer *tmflee;
	TCheckBox *chmeat;
	void __fastcall btncloseClick(TObject *Sender);
	void __fastcall chfleeClick(TObject *Sender);
	void __fastcall chsuicide2Click(TObject *Sender);
	void __fastcall chsuicide1Click(TObject *Sender);
	void __fastcall txtsuicidelimitChange(TObject *Sender);
	void __fastcall tmsuicideTimer(TObject *Sender);
	void __fastcall chstoponnoptClick(TObject *Sender);
	void __fastcall chleaveondieClick(TObject *Sender);
	void __fastcall chstartinptClick(TObject *Sender);
	void __fastcall tmptcheckTimer(TObject *Sender);
	void __fastcall txtfleewaitChange(TObject *Sender);
	void __fastcall chfleetownClick(TObject *Sender);
	void __fastcall chfleeringClick(TObject *Sender);
	void __fastcall btnfleeaddClick(TObject *Sender);
	void __fastcall btnfleeclearClick(TObject *Sender);
	void __fastcall tmfleeTimer(TObject *Sender);
	void __fastcall chmeatClick(TObject *Sender);
private:
public:

	bool bSuicideOnEnemy;
	bool bSuicideOnLimit;
	int	iSuicideLimit;
	bool bFlee;
	int iFleeWait;
	bool bFleeTown;
	bool bFleeRing;
	int iFleeX[20];
	int iFleeY[20];
	int iFleeCount;
	int iFleeAction;
	int	iFleeRoute;
	int iFleeLastPos;
	long	lLeftTick;
	bool	bStopBotOnNoPT;
	bool	bLeavePTOnDie;
	bool	bStartInPT;

	virtual __fastcall TdlgCZ(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TdlgCZ *dlgCZ;
//----------------------------------------------------------------------------
#endif    
