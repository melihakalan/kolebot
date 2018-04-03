//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "_frmMain.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btngenerateClick(TObject *Sender)
{
	char* pKey = new char[32];

	memset(pKey,0,32);

	Generate(pKey);
	txtKey->Text = pKey;

	delete[] pKey;
}
//---------------------------------------------------------------------------

void Generate(char* KeyBuffer){

	char* pKey = KeyBuffer;

	srand (GetTickCount());
	BYTE bKeyNo = (BYTE)atoi(frmMain->txtkeyno->Text.c_str());
	short sRand1 = rand() % 0xFFFF + 1;
	if(sRand1 < 1000)
		sRand1 += 5000;
	BYTE bKeyLimit = (BYTE)atoi(frmMain->txtlimit->Text.c_str());

	SYSTEMTIME pDate;
	GetSystemTime(&pDate);

	srand (pDate.wMilliseconds);
	short sRand2 = rand() % 0xFFFF + 1;
	if(sRand2 < 1000)
		sRand2 += 5000;

	long lKey =	((sRand1 * bKeyNo) + (sRand2 * bKeyLimit)) * (sRand1 + sRand2 + 8095);

	int iLen;

	ltoa(bKeyNo,pKey,16);
	iLen = strlen(pKey);
	if(iLen < 2){
		memmove(pKey+(2-iLen),pKey,iLen);
		memset(pKey,0x30,2-iLen);
	}

	ltoa(bKeyLimit,pKey+2,16);
	iLen = strlen(pKey+2);
	if(iLen < 2){
		memmove(pKey+2+(2-iLen),pKey+2,iLen);
		memset(pKey+2,0x30,2-iLen);
	}

	ltoa(sRand1,pKey+4,16);
	iLen = strlen(pKey+4);
	if(iLen < 4){
		memmove(pKey+4+(4-iLen),pKey+4,iLen);
		memset(pKey+4,0x30,4-iLen);
	}

	ltoa(sRand2,pKey+8,16);
	iLen = strlen(pKey+8);
	if(iLen < 4){
		memmove(pKey+8+(4-iLen),pKey+8,iLen);
		memset(pKey+8,0x30,4-iLen);
	}

	ltoa(lKey,pKey+12,16);
	iLen = strlen(pKey+12);
	if(iLen < 8){
		memmove(pKey+12+(8-iLen),pKey+12,iLen);
		memset(pKey+12,0x30,8-iLen);
	}


}
void __fastcall TfrmMain::btngenerateallClick(TObject *Sender)
{
	ofstream fLogFile;
	fLogFile.open("keylist.txt",std::ios_base::out | std::ios_base::app);
	if(!fLogFile.is_open())return;

	int iCount = atoi(txtcount->Text.c_str());
	char* pKey = new char[32];

	for (int i = 0; i < iCount; i++) {

		memset(pKey,0,32);

		Generate(pKey);
		fLogFile << pKey << "\n";
		Sleep(50);
	}
	fLogFile.close();
	delete [] pKey;
	MessageBox(this->Handle,"Keyler listeye eklendi.","Ekleme",MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnanalizClick(TObject *Sender)
{
	if(!txtKey2->Text.Length())
		return;

	BYTE bKeyNo = HexToByte(txtKey2->Text.c_str());
	BYTE bKeyLimit = HexToByte(txtKey2->Text.c_str() + 2);
	short sRand1 = HexToShort(txtKey2->Text.c_str() + 4);
	short sRand2 = HexToShort(txtKey2->Text.c_str() + 8);
	long lKey = HexToLong(txtKey2->Text.c_str() + 12);

	char tmp[32];

	ltoa(bKeyNo,tmp,10);
	txtkeyno2->Text = tmp;

	ltoa(bKeyLimit,tmp,10);
	txtdays->Text = tmp;

	ltoa(sRand1,tmp,16);
	txtrand1->Text = tmp;

	ltoa(sRand2,tmp,16);
	txtrand2->Text = tmp;

	ltoa(lKey,tmp,16);
	txtlkey->Text = tmp;

}
//---------------------------------------------------------------------------


BYTE HexToByte(char* strHex){
	long lRet = 0;
	sscanf(strHex,"%2X",&lRet);
	return (BYTE)lRet;
}

short HexToShort(char* strHex){
	long lRet = 0;
	sscanf(strHex,"%4X",&lRet);
	return (short)lRet;
}

long HexToLong(char* strHex){
	long lRet = 0;
	sscanf(strHex,"%8X",&lRet);
	return lRet;
}

void __fastcall TfrmMain::btnsavekeyClick(TObject *Sender)
{
	ofstream fLogFile;
	fLogFile.open("keylist.txt",std::ios_base::out | std::ios_base::app);
	if(!fLogFile.is_open())return;
	fLogFile << txtKey->Text.c_str() << "\n";
	fLogFile.close();
}
//---------------------------------------------------------------------------

