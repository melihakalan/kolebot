//----------------------------------------------------------------------------
#ifndef fdlgMadenH
#define fdlgMadenH
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
#include <CheckLst.hpp>

#include "Memory.h"

#define ACT_STORE	1
#define ACT_SELL	2
#define ACT_REMOVE	3
#define ACT_NOP		4

#define ITEM_NORMAL_ORE	399210000
#define ITEM_GOLD_ORE	399200000

extern BYTE pPacket_MineMain[6];
extern BYTE pPacket_MineOre1[19];
extern BYTE pPacket_MineOre2[19];

void ClickAction(int iType, int iItemIndex);
int iRandomPos(int iVal, int iMaxChange);
void Town();
void SellItem(DWORD dwSundID, int iItemSlot);
void RefreshInv();
void StoreItem(DWORD dwInnID,int iSourceSlot, int iDestSlot);
void OpenInn(DWORD dwInnID);
bool CheckInn();
void RepairLeft(DWORD dwSundID);
void RepairRight(DWORD dwSundID);
void SaveMineSettings();
void LoadMineSettings();
void InitActionList();
void RemoveItem(int iItemSlot);
void RunNPC(DWORD dwNpcID);
bool IsMining(){return (bool)*(BYTE*)( *(DWORD*)KO_DLGA + KO_MINING); }
void InitMineZone();
bool Move(float fToX, float fToY, int iBoosting);
//----------------------------------------------------------------------------
class TdlgMaden : public TForm
{
__published:
	TCheckListBox *lstinn;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TCheckListBox *lstsund;
	TCheckListBox *lstremove;
	TLabel *Label4;
	TCheckListBox *lstnop;
	TButton *btnclose;
	TCheckBox *chmaden;
	TLabel *Label6;
	TTimer *tmmaden;
	TTimer *tmrpr;
	TTimer *tmsundgo;
	TButton *btnabyss;
	TButton *btnblue;
	TButton *btngreen;
	TButton *btnred;
	TCheckBox *chmadensh;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall lstinnClick(TObject *Sender);
	void __fastcall lstsundClick(TObject *Sender);
	void __fastcall lstremoveClick(TObject *Sender);
	void __fastcall lstnopClick(TObject *Sender);
	void __fastcall btncloseClick(TObject *Sender);
	void __fastcall chmadenClick(TObject *Sender);
	void __fastcall tmmadenTimer(TObject *Sender);
	void __fastcall tmrprTimer(TObject *Sender);
	void __fastcall tmsundgoTimer(TObject *Sender);
	void __fastcall btnabyssClick(TObject *Sender);
	void __fastcall btnblueClick(TObject *Sender);
	void __fastcall btngreenClick(TObject *Sender);
	void __fastcall btnredClick(TObject *Sender);
	void __fastcall chmadenshClick(TObject *Sender);
private:
public:

	int	iItemActionList[33];
	long lItemList[33];
	bool bStackable[33];
	char* strItemList[33];
	long lFragmentList[7];
	char* strFragmentList[7];
	bool bEnabled;

	int iCurrentAction;
	bool bTowned;

	bool bSundOK;
	bool bInnOK;
	bool bInnOpened;

	int	iMineX;
	int	iMineY;

	int iPitmanX;
	int	iPitmanY;

	int iTownToInnX[7];
	int iTownToInnY[7];

	int iTownToMineX;
	int	iTownToMineY;

	int iNextX;
	int iNextY;
	int iLastPos;

	bool bTownOK;
	DWORD dwLastInnBase;

	bool	bOnSundWay;
	bool	bOnMineWay;

	long	lLastStartTick;

	bool	bSH;


	virtual __fastcall TdlgMaden(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TdlgMaden *dlgMaden;
//----------------------------------------------------------------------------
#endif
