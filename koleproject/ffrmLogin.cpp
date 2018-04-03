//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ffrmLogin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLogin *frmLogin;

int iRegisterTryCount = 0;
long iRegisterTryTick = 0;

int iLoginTryCount = 0;
long iLoginTryTick = 0;

//---------------------------------------------------------------------------
__fastcall TfrmLogin::TfrmLogin(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::btnLoginClick(TObject *Sender)
{
	if(iLoginTryCount >= 3)
	{
		if(GetTickCount() - iLoginTryTick < 600000)	//10dk
		{
			MessageBox(this->Handle,"Login deneme limitiniz dolmustur! Login yapabilmek icin 10 dakika beklemelisiniz.","Hata",MB_ICONEXCLAMATION);
			return;
		}
		else
		{
			iLoginTryCount = 0;
		}
	}

	int iIDLen = txtloginid->Text.Length();
	int iPWLen = txtloginpw->Text.Length();

	if(!iIDLen || !iPWLen){
		MessageBox(this->Handle,"Hatali uyelik!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(iIDLen < 4 || iIDLen > 20){
		MessageBox(this->Handle,"ID min. 4 - max. 20 karakter olmalidir.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(iPWLen < 4 || iPWLen > 20){
		MessageBox(this->Handle,"Sifre min. 4 - max. 20 karakter olmalidir.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if( !CheckValidIDPW(txtloginid->Text.c_str()) || !CheckValidIDPW(txtloginpw->Text.c_str())  )
	{
		MessageBox(this->Handle,"ID veya Sifre hatali! Turkce veya ozel karakter kullanmayiniz!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(!_CWinsock.GetState()){
		bool bStart = _CWinsock.StartWinsock();
		if(!bStart){
			MessageBox(this->Handle,"Winsock error!","Hata",MB_ICONEXCLAMATION);
			return;
		}
	}

	CMainSocket = _CWinsock.NewSocket();
	if((long)CMainSocket.GetSocket() == INVALID_SOCKET){
		MessageBox(this->Handle,"Socket error!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	//DWORD dwTrue = 1;
	//ioctlsocket(CMainSocket.GetSocket(), FIONBIO, (u_long FAR*) &dwTrue);

	if(!CMainSocket.Client_Connect(strServerIP,iServerPort)){
		CMainSocket.Close();
		if(!bAutoLogin)
			MessageBox(this->Handle,"Baglanti hatasi!","Hata",MB_ICONEXCLAMATION);
		else
			SetAutoLogin(0,1);
		return;
	}

	DWORD dwCharBase = *(DWORD*)(KO_CHAR);
	if(dwCharBase == 0){
		CMainSocket.Close();
		MessageBox(this->Handle,"Char Init Error!","Hata!",MB_ICONSTOP);
		return;
	}

	WORD wSourceID = *(WORD*)(dwCharBase + KO_MYID);
	if(wSourceID == 0){
		CMainSocket.Close();
		MessageBox(this->Handle,"Char Init Error!","Hata!",MB_ICONSTOP);
		return;
	}

	DWORD dwSockBase = *(DWORD*)(KO_SOCK);
	if(dwSockBase == 0){
		CMainSocket.Close();
		MessageBox(this->Handle,"Sock Init Error!","Hata!",MB_ICONSTOP);
		return;
	}

	DWORD dwSockHandle = *(DWORD*)(dwSockBase + 0x4);
	if(dwSockHandle == 0 || dwSockHandle == (DWORD)-1){
		CMainSocket.Close();
		MessageBox(this->Handle,"Sock Init Error!","Hata!",MB_ICONSTOP);
		return;
	}
	dwSockHandle++;

	//BYTE bSendAdd = (BYTE)((DWORD)&SendFunction % 10);
	//__asm{NOT bSendAdd};
	DWORD dwSendAddr = (DWORD)&SendFunction;
	__asm{NOT dwSendAddr};

	int iLoginPacketSize = iIDLen + iPWLen + 27;
	char* pSend = new char[iLoginPacketSize];
	memset(pSend,0,iLoginPacketSize);
	int index(0);
	CMainSocket.PutPacketHead(pSend,index);							//head
	CMainSocket.PutBYTE(pSend,bSend_LOGIN,index);					//01
	CMainSocket.PutWORD(pSend,sBotVersion,index);
	CMainSocket.PutWORD(pSend,wSourceID,index);
	CMainSocket.PutDWORD(pSend,dwSockHandle,index);
	CMainSocket.PutDWORD(pSend,dwSendAddr,index);
	CMainSocket.PutDWORD(pSend,ulHDSerialNo,index);
	CMainSocket.PutDWORD(pSend,dwServerARG,index);
	CMainSocket.PutBYTE(pSend,(BYTE)iIDLen,index);
	CMainSocket.PutSTR(pSend,txtloginid->Text.c_str(),iIDLen,index);
	CMainSocket.PutBYTE(pSend,(BYTE)iPWLen,index);
	CMainSocket.PutSTR(pSend,txtloginpw->Text.c_str(),iPWLen,index);
	CMainSocket.PutPacketTail(pSend,index);							//tail
	CMainSocket.Client_Send(pSend,iLoginPacketSize);

	delete[] pSend;

	int SentTick = GetTickCount(); //gecikme test

	char* pRecv = new char[iRecvSizeLogin];
	memset(pRecv,0,iRecvSizeLogin);
	DWORD nRecv(0),nRecvd(0);

	while(!nRecv){
	ioctlsocket(CMainSocket.GetSocket(),FIONREAD,&nRecv);
		if(!nRecv && GetTickCount() - SentTick >= 5000)
		{
			CMainSocket.Close();
			MessageBox(this->Handle,"Sunucudan cevap alinamadi!","Hata",MB_ICONEXCLAMATION);
			return;
		}
		Sleep(100);
	}

	if((int)nRecv != iRecvSizeLogin){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (overflow)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	do{
		nRecvd += CMainSocket.Client_Recv(pRecv+nRecvd,nRecv-nRecvd);
	} while(nRecvd < nRecv);

	int iDataHead,iDataTail,iDataSize;
	char* pData;

	iDataHead = CMainSocket.FindPacketHead(pRecv,nRecvd);
	if((DWORD)iDataHead < sizeof(wPACKETHEAD)){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	iDataTail = CMainSocket.FindPacketTail(pRecv+iDataHead,nRecvd-iDataHead);
	if((DWORD)iDataTail < 1){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	iDataSize = iDataTail;
	pData = pRecv + iDataHead;

	index = 0;

	if(CMainSocket.GetBYTE(pData,index) != bRecv_LOGIN){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	CMainSocket.Close();	//end
	BYTE bPrefix = CMainSocket.GetBYTE(pData,index);
	switch(bPrefix){

		case bLogin_INVALID:
		{
			MessageBox(this->Handle,"Yanlis ID veya Sifre!\n Dikkat! 3 kez yanlis giris denemesi yaptiginizda 10 dakika beklemek zorunda kalacaksiniz!","Hata",MB_ICONEXCLAMATION);
			iLoginTryCount++;
			iLoginTryTick = GetTickCount();
			break;
		}

		case bLogin_SUCCESS:
		{
			bPacketKey = CMainSocket.GetBYTE(pData,index);
			bPacketKeyOld += bPacketKey;
			bPacketKeyOld -= Make0x54Byte();
			int iRemainingDays = CMainSocket.GetBYTE(pData,index);
			iMyUserIndex = CMainSocket.GetWORD(pData,index);
			iMyUserID = CMainSocket.GetWORD(pData,index);
			dwMyID = CMainSocket.GetWORD(pData,index);
			dwRecvdSockHandle = CMainSocket.GetDWORD(pData,index);
			//dwSendPointerAdd -= Make0x54Byte();
			__asm{NEG dwSendPointerAdd};
			BYTE bFix = CMainSocket.GetBYTE(pData,index);
			DWORD dwHDNo = CMainSocket.GetDWORD(pData,index);
			DWORD dwARG = CMainSocket.GetDWORD(pData,index);

			if(dwHDNo != ulRealHDSerialNo)
			{
				MessageBox(this->Handle,"Serial error 1!","Error!",MB_ICONSTOP);
				OwnTerminate();
			}

			if(dwARG != ( dwServerARG - ((DWORD)&SendFunction / 10) ))
			{
				MessageBox(this->Handle,"Serial error 2!","Error!",MB_ICONSTOP);
				OwnTerminate();
			}

			iMyLogOutKey = CMainSocket.GetWORD(pData,index);
			__asm{NOT bFix};
			dwSendPointerAdd += bFix;
			__asm{NOT dwSendPointerAdd};
			iRemaining = iRemainingDays;
			char tmp[16];	itoa(iRemainingDays,tmp,10);
			char* pMsg = new char[96];
			memset(pMsg,0,96);
			strcpy(pMsg,"Login basariyla yapildi. Bot kullanima hazirdir. Mevcut gun miktariniz: ");

			if(iRemainingDays == 0xFF)
				strcat(pMsg,"Limitsiz");
			else
				strcat(pMsg,tmp);

			if(bAutoLogin == false)
				MessageBox(this->Handle,pMsg,"Login",MB_ICONINFORMATION);

			delete [] pMsg;
			delete [] pRecv;

			tmdebugger->Enabled = false;

			frmBot = new TfrmBot(NULL);
			Application->CreateForm(__classid(TfrmBot), &frmBot);
			frmBot->Show();
			SetForegroundWindow(frmBot->Handle);

			frmLogin->Hide();
			break;
		}

		case bLogin_TRIALERR:
		{
			MessageBox(this->Handle,"Deneme uyeligi sadece 18:00 ile 21:00 saatleri arasýnda kullanýlabilir!","Hata",MB_ICONEXCLAMATION);
			iLoginTryCount++;
			iLoginTryTick = GetTickCount();
			break;
		}

		case bLogin_INITERR:
		{
			MessageBox(this->Handle,"Bot init error!","Hata",MB_ICONEXCLAMATION);
			iLoginTryCount++;
			iLoginTryTick = GetTickCount();
			break;
		}

		case bLogin_INVALIDV:
		{
			MessageBox(this->Handle,"Surum hatasi! lutfen bot'un son surumunu indirip kullaniniz.","Hata",MB_ICONEXCLAMATION);
			iLoginTryCount++;
			iLoginTryTick = GetTickCount();
			break;
        }

		case bLogin_INUSE:
		{
			iMyUserIndex = CMainSocket.GetWORD(pData,index);	//not used
			iMyUserIndex = CMainSocket.GetWORD(pData,index);	//index
			iMyUserID = CMainSocket.GetWORD(pData,index);	  	//id
			index += 15;	//
			iMyLogOutKey = CMainSocket.GetWORD(pData,index);	//key

			if(bAutoLogin == true)
			{
				BanOther();
				btnLoginClick(NULL);
			}
			else
			{
				int iAnswer = MessageBox(this->Handle,"Bu uyelik su anda baska birisi tarafindan kullanilmaktadir. Diger baglantiyi kapatmak istiyor musunuz?","Hata",MB_ICONEXCLAMATION + MB_YESNO);
				if(iAnswer == IDYES){
					BanOther();
					btnLoginClick(NULL);
				}
			}
			break;
        }

		case bLogin_BANNED:
		{
			MessageBox(this->Handle,"Yazmis oldugunuz uyelik yasaklanmistir.","Hata",MB_ICONEXCLAMATION);
			iLoginTryCount++;
			iLoginTryTick = GetTickCount();
			break;
		}

		case bLogin_TRYLATER:
		{
			MessageBox(this->Handle,"Bot su an kullanima kapalidir, lutfen daha sonra tekrar deneyiniz. Detayli bilgi icin www.onlinehile.org ziyaret ediniz.","Hata",MB_ICONEXCLAMATION);
			break;
		}

		case bLogin_TIMEOUT:
		{
			MessageBox(this->Handle,"Uyelik limitiniz dolmustur. Lutfen yeni uyelik satin aliniz. www.kojdbot.com","Hata",MB_ICONEXCLAMATION);
			iLoginTryCount++;
			iLoginTryTick = GetTickCount();
			break;
		}

	}

	delete [] pRecv;

}
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::FormShow(TObject *Sender)
{

	MakeCaption();
	tmdebugger->Enabled = true;

	if(bAutoLogin == true)
	{
		tmloginbot->Enabled = true;
	}


}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::btnRegisterClick(TObject *Sender)
{
	if(iRegisterTryCount >= 3)
	{
		if(GetTickCount() - iRegisterTryTick < 600000)	//10dk
		{
			MessageBox(this->Handle,"Kayit deneme limitiniz dolmustur! Kayit yapabilmek icin 10 dakika beklemelisiniz.","Hata",MB_ICONEXCLAMATION);
			return;
		}
		else
		{
			iRegisterTryCount = 0;
		}
	}

	if(txtKey->Text.Length() != iKeySize * 2){
		MessageBox(this->Handle,"Hatali key!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	int iIDLen = txtregisterid->Text.Length();
	int iPWLen = txtregisterpw->Text.Length();

	if(!iIDLen || !iPWLen || !txtregisterpw2->Text.Length()){
		MessageBox(this->Handle,"Hatali id veya sifre!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(iIDLen < 4 || iIDLen > 20){
		MessageBox(this->Handle,"ID min. 4 - max. 20 karakter olmalidir.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(iPWLen < 4 || iPWLen > 20){
		MessageBox(this->Handle,"Sifre min. 4 - max. 20 karakter olmalidir.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(strcmp(txtregisterpw->Text.c_str(),txtregisterpw2->Text.c_str())){
		MessageBox(this->Handle,"Sifreler uyumsuz!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	char* pTmpStr = txtregisterid->Text.c_str();
	if( !CheckValidIDPW(pTmpStr) )
	{
		MessageBox(this->Handle,"ID ve sifrenizde ozel karakterler bulunamaz. Lutfen turkce karakter ve ozel karakter kullanmayiniz.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	pTmpStr = txtregisterpw->Text.c_str();
	if( !CheckValidIDPW(pTmpStr) )
	{
		MessageBox(this->Handle,"ID ve sifrenizde ozel karakterler bulunamaz. Lutfen turkce karakter ve ozel karakter kullanmayiniz.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	pTmpStr = txtKey->Text.c_str();
	if( !CheckIllegal(pTmpStr) )
	{
		MessageBox(this->Handle,"Hatali key!","Hata",MB_ICONEXCLAMATION);
		return;
    }

	if(!_CWinsock.GetState()){
		bool bStart = _CWinsock.StartWinsock();
		if(!bStart){
			MessageBox(this->Handle,"Winsock error!","Hata",MB_ICONEXCLAMATION);
			return;
		}
	}

	CMainSocket = _CWinsock.NewSocket();
	if((long)CMainSocket.GetSocket() == INVALID_SOCKET){
		MessageBox(this->Handle,"Socket error!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	//DWORD dwTrue = 1;
	//ioctlsocket(CMainSocket.GetSocket(), FIONBIO, (u_long FAR*) &dwTrue);

	if(!CMainSocket.Client_Connect(strServerIP,iServerPort)){
		CMainSocket.Close();
		MessageBox(this->Handle,"Baglanti hatasi!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	int iRegisterPacketSize = iKeySize + 7 + iIDLen + iPWLen;
	char *pSend = new char[iRegisterPacketSize];
	memset(pSend,0,iRegisterPacketSize);

	int index(0);

	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_REGISTER,index);

	long lBuffer;

	lBuffer = HexToByte(txtKey->Text.c_str());
	CMainSocket.PutBYTE(pSend,lBuffer,index);

	lBuffer = HexToByte(txtKey->Text.c_str() + 2);
	CMainSocket.PutBYTE(pSend,lBuffer,index);

	lBuffer = HexToShort(txtKey->Text.c_str() + 4);
	CMainSocket.PutWORD(pSend,lBuffer,index);

	lBuffer = HexToShort(txtKey->Text.c_str() + 8);
	CMainSocket.PutWORD(pSend,lBuffer,index);

	lBuffer = HexToLong(txtKey->Text.c_str() + 12);
	CMainSocket.PutDWORD(pSend,lBuffer,index);

	CMainSocket.PutBYTE(pSend,iIDLen,index);
	CMainSocket.PutSTR(pSend,txtregisterid->Text.c_str(),iIDLen,index);
	CMainSocket.PutBYTE(pSend,iPWLen,index);
	CMainSocket.PutSTR(pSend,txtregisterpw->Text.c_str(),iPWLen,index);
	CMainSocket.PutPacketTail(pSend,index);
	CMainSocket.Client_Send(pSend,iRegisterPacketSize);

	delete[] pSend;

	int SentTick = GetTickCount(); //gecikme test

	char* pRecv = new char[iRecvSizeRegister];
	memset(pRecv,0,iRecvSizeRegister);
	DWORD nRecv(0),nRecvd(0);

	while(!nRecv){
	ioctlsocket(CMainSocket.GetSocket(),FIONREAD,&nRecv);
		if(!nRecv && GetTickCount() - SentTick >= 5000)
		{
			CMainSocket.Close();
			MessageBox(this->Handle,"Sunucudan cevap alinamadi!","Hata",MB_ICONEXCLAMATION);
			return;
		}
		Sleep(100);
	}

	if((int)nRecv != iRecvSizeRegister){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (overflow)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	do{
		nRecvd += CMainSocket.Client_Recv(pRecv+nRecvd,nRecv-nRecvd);
	} while(nRecvd < nRecv);

	int iDataHead,iDataTail,iDataSize;
	char* pData;

	iDataHead = CMainSocket.FindPacketHead(pRecv,nRecvd);
	if((DWORD)iDataHead < sizeof(wPACKETHEAD)){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	iDataTail = CMainSocket.FindPacketTail(pRecv+iDataHead,nRecvd-iDataHead);
	if((DWORD)iDataTail < 1){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	iDataSize = iDataTail;
	pData = pRecv + iDataHead;

	index = 0;

	if(CMainSocket.GetBYTE(pData,index) != bRecv_REGISTER){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	CMainSocket.Close();	//end
	BYTE bPrefix = CMainSocket.GetBYTE(pData,index);
	switch(bPrefix){

		case bRegister_INUSE:
		{
			MessageBox(this->Handle,"Yazmis oldugunuz ID baskasi tarafindan kullanilmaktadir.","Hata",MB_ICONEXCLAMATION);
			break;
		}

		case bRegister_SUCCESS:
		{
			int iDays = CMainSocket.GetBYTE(pData,index);
			char tmp[8];	itoa(iDays,tmp,10);
			char* pMsg = new char[64];
			memset(pMsg,0,64);
			strcpy(pMsg,"Yeni kayit basariyla yapildi. Mevcut gun limiti: ");
			strcat(pMsg,tmp);
			MessageBox(this->Handle,pMsg,"Register",MB_ICONINFORMATION);
			delete [] pMsg;
			txtloginid->Text = txtregisterid->Text;
			txtloginpw->Text = txtregisterpw->Text;
			pageMain->ActivePageIndex = 0;
			break;
		}

		case bRegister_PAUSED:
		{
			MessageBox(this->Handle,"Su an kayit yapilamamaktadir. Lutfen daha sonra tekrar deneyin.","Hata",MB_ICONEXCLAMATION);
			break;
		}

		case bRegister_BANNED:
		{
			MessageBox(this->Handle,"Yazdiginiz key yasaklanmistir!\n Dikkat! 3 kez hatali kayit denediginizde 10 dakika engelleneceksiniz!","Hata",MB_ICONEXCLAMATION);
			iRegisterTryCount++;
			iRegisterTryTick = GetTickCount();
			break;
		}

		case bRegister_KEYUSED:
		{
			MessageBox(this->Handle,"Yazdiginiz key zaten kullanilmistir!\n Dikkat! 3 kez hatali kayit denediginizde 10 dakika engelleneceksiniz!","Hata",MB_ICONEXCLAMATION);
			iRegisterTryCount++;
			iRegisterTryTick = GetTickCount();
			break;
		}

		case bRegister_IDPWERROR:
		{
			MessageBox(this->Handle,"ID veya Sifre hatali! Turkce ve ozel karakter kullanmayiniz!\n Dikkat! 3 kez hatali kayit denediginizde 10 dakika engelleneceksiniz!","Hata",MB_ICONEXCLAMATION);
			iRegisterTryCount++;
			iRegisterTryTick = GetTickCount();
			break;
		}

		case bRegister_INVALID:
		{
			MessageBox(this->Handle,"Yanlis key!\n Dikkat! 3 kez hatali kayit denediginizde 10 dakika engelleneceksiniz!","Hata",MB_ICONEXCLAMATION);
			iRegisterTryCount++;
			iRegisterTryTick = GetTickCount();
			break;
		}

	}

	delete [] pRecv;

}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::btnAddClick(TObject *Sender)
{
	if(iRegisterTryCount >= 3)
	{
		if(GetTickCount() - iRegisterTryTick < 600000)	//10dk
		{
			MessageBox(this->Handle,"Ekleme yapabilmek icin 10 dakika beklemelisiniz.","Hata",MB_ICONEXCLAMATION);
			return;
		}
		else
		{
			iRegisterTryCount = 0;
		}
	}

	if(txtaddkey->Text.Length() != iKeySize * 2){
		MessageBox(this->Handle,"Hatali key!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	int iIDLen = txtaddid->Text.Length();
	int iPWLen = txtaddpw->Text.Length();

	if(!iIDLen || !iPWLen){
		MessageBox(this->Handle,"Hatali id veya sifre!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(iIDLen < 4 || iIDLen > 20){
		MessageBox(this->Handle,"ID min. 4 - max. 20 karakter olmalidir.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(iPWLen < 4 || iPWLen > 20){
		MessageBox(this->Handle,"Sifre min. 4 - max. 20 karakter olmalidir.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	char *pTmpStr = txtaddkey->Text.c_str();
	if( !CheckIllegal(pTmpStr) )
	{
		MessageBox(this->Handle,"Hatali key!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	pTmpStr = txtaddid->Text.c_str();
	if( !CheckValidIDPW(pTmpStr) )
	{
		MessageBox(this->Handle,"Hatali ID veya Sifre!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	pTmpStr = txtaddpw->Text.c_str();
	if( !CheckValidIDPW(pTmpStr) )
	{
		MessageBox(this->Handle,"Hatali ID veya Sifre!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(!_CWinsock.GetState()){
		bool bStart = _CWinsock.StartWinsock();
		if(!bStart){
			MessageBox(this->Handle,"Winsock error!","Hata",MB_ICONEXCLAMATION);
			return;
		}
	}

	CMainSocket = _CWinsock.NewSocket();
	if((long)CMainSocket.GetSocket() == INVALID_SOCKET){
		MessageBox(this->Handle,"Socket error!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	//DWORD dwTrue = 1;
	//ioctlsocket(CMainSocket.GetSocket(), FIONBIO, (u_long FAR*) &dwTrue);

	if(!CMainSocket.Client_Connect(strServerIP,iServerPort)){
		CMainSocket.Close();
		MessageBox(this->Handle,"Baglanti hatasi!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	int iAddPacketSize = iKeySize + 7 + iIDLen + iPWLen;
	char *pSend = new char[iAddPacketSize];
	memset(pSend,0,iAddPacketSize);

	int index(0);

	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_ADD,index);

	long lBuffer;

	lBuffer = HexToByte(txtaddkey->Text.c_str());
	CMainSocket.PutBYTE(pSend,lBuffer,index);

	lBuffer = HexToByte(txtaddkey->Text.c_str() + 2);
	CMainSocket.PutBYTE(pSend,lBuffer,index);

	lBuffer = HexToShort(txtaddkey->Text.c_str() + 4);
	CMainSocket.PutWORD(pSend,lBuffer,index);

	lBuffer = HexToShort(txtaddkey->Text.c_str() + 8);
	CMainSocket.PutWORD(pSend,lBuffer,index);

	lBuffer = HexToLong(txtaddkey->Text.c_str() + 12);
	CMainSocket.PutDWORD(pSend,lBuffer,index);

	CMainSocket.PutBYTE(pSend,iIDLen,index);
	CMainSocket.PutSTR(pSend,txtaddid->Text.c_str(),iIDLen,index);
	CMainSocket.PutBYTE(pSend,iPWLen,index);
	CMainSocket.PutSTR(pSend,txtaddpw->Text.c_str(),iPWLen,index);
	CMainSocket.PutPacketTail(pSend,index);
	CMainSocket.Client_Send(pSend,iAddPacketSize);

	delete[] pSend;

	int SentTick = GetTickCount(); //gecikme test

	char* pRecv = new char[iRecvSizeAdd];
	memset(pRecv,0,iRecvSizeAdd);
	DWORD nRecv(0),nRecvd(0);

	while(!nRecv){
	ioctlsocket(CMainSocket.GetSocket(),FIONREAD,&nRecv);
		if(!nRecv && GetTickCount() - SentTick >= 5000)
		{
			CMainSocket.Close();
			MessageBox(this->Handle,"Sunucudan cevap alinamadi!","Hata",MB_ICONEXCLAMATION);
			return;
		}
		Sleep(100);
	}

	if((int)nRecv != iRecvSizeAdd){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (overflow)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	do{
		nRecvd += CMainSocket.Client_Recv(pRecv+nRecvd,nRecv-nRecvd);
	} while(nRecvd < nRecv);

	int iDataHead,iDataTail,iDataSize;
	char* pData;

	iDataHead = CMainSocket.FindPacketHead(pRecv,nRecvd);
	if((DWORD)iDataHead < sizeof(wPACKETHEAD)){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	iDataTail = CMainSocket.FindPacketTail(pRecv+iDataHead,nRecvd-iDataHead);
	if((DWORD)iDataTail < 1){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	iDataSize = iDataTail;
	pData = pRecv + iDataHead;

	index = 0;

	if(CMainSocket.GetBYTE(pData,index) != bRecv_ADD){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	CMainSocket.Close();	//end
	BYTE bPrefix = CMainSocket.GetBYTE(pData,index);
	switch(bPrefix){

		case bAdd_IDPWINVALID:
		{
			MessageBox(this->Handle,"ID veya sifre hatali!","Hata",MB_ICONEXCLAMATION);
			iRegisterTryCount++;
			iRegisterTryTick = GetTickCount();
			break;
		}

		case bAdd_SUCCESS:
		{
			int iDays = CMainSocket.GetBYTE(pData,index);
			char tmp[8];	itoa(iDays,tmp,10);
			char* pMsg = new char[64];
			memset(pMsg,0,64);
			strcpy(pMsg,"Gun ekleme basariyla yapildi! Mevcut gun miktariniz: ");
			strcat(pMsg,tmp);
			MessageBox(this->Handle,pMsg,"Ekleme",MB_ICONINFORMATION);
			delete [] pMsg;
			break;
		}

		case bAdd_PAUSED:
		{
			MessageBox(this->Handle,"Ekleme islemi su an icin kullanilamiyor.","Hata",MB_ICONEXCLAMATION);
			break;
		}

		case bAdd_NOTIMEOUT:
		{
			MessageBox(this->Handle,"Mevcut suresi henuz bitmemis bir hesaba ekleme yapamazsiniz.","Hata",MB_ICONEXCLAMATION);
			break;
        }

		case bAdd_IDBANNED:
		{
			MessageBox(this->Handle,"Uyelik yasaklanmistir.","Hata",MB_ICONEXCLAMATION);
			break;
		}

		case bAdd_KEYBANNED:
		{
			MessageBox(this->Handle,"Yazdiginiz key yasaklanmistir.","Hata",MB_ICONEXCLAMATION);
			iRegisterTryCount++;
			iRegisterTryTick = GetTickCount();
			break;
		}

		case bAdd_KEYUSED:
		{
			MessageBox(this->Handle,"Yazdiginiz key zaten kullanilmistir.","Hata",MB_ICONEXCLAMATION);
			iRegisterTryCount++;
			iRegisterTryTick = GetTickCount();
			break;
		}

		case bAdd_KEYINVALID:
		{
			MessageBox(this->Handle,"Yanlis key!","Hata",MB_ICONEXCLAMATION);
			break;
		}

	}

	delete [] pRecv;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::FormClose(TObject *Sender, TCloseAction &Action)
{
	//FreeLibraryAndExitThread(GetModuleHandle("kojdbot.dll"),0);

	TerminateProcess((void*)-1,0);

	/*
	DWORD dwAPI1 = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"GetModuleFileNameA");
	DWORD dwAPI2 = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"GetModuleHandleA");
	DWORD dwAPI3 = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"GetProcAddress");
	DWORD dwAPI4 = (DWORD)GetProcAddress(GetModuleHandle("user32.dll"),"GetWindowThreadProcessId");
	DWORD dwAPI5 = (DWORD)GetProcAddress(GetModuleHandle("user32.dll"),"GetWindowTextA");
	*(DWORD*)KO_API1 = dwAPI1;
	*(DWORD*)KO_API2 = dwAPI2;
	*(DWORD*)KO_API3 = dwAPI3;
	*(DWORD*)KO_API4 = dwAPI4;
	*(DWORD*)KO_API5 = dwAPI5;

	DWORD dwTerminate = (DWORD)GetProcAddress(GetModuleHandle("kernel32.dll"),"TerminateProcess");
	*(DWORD*)KO_QUIT = dwTerminate;

	RepairCreateThread();

	if(bHookPatched == true)
		_CPacket.RepairGameFunction();

	*/

	HMODULE hModule = NULL;
	bOwnCallHandleExA = true;
	GetModuleHandleEx(NULL,"kojdbot.dll",&hModule);
	bOwnCallHandleExA = false;
	FreeLibraryAndExitThread(hModule,0);

}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::chloadClick(TObject *Sender)
{
	bLoadSettings = chload->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::btntrialClick(TObject *Sender)
{
	MessageBox(this->Handle,"Deneme surumu gecici olarak kapatilmistir. Lutfen deneme yapabilmek icin www.kojdbot.com adresinden belirli saatler arasi kullanimi ucretsiz olan kojdbot - deneme kart uyeligini aliniz.","deneme",MB_ICONINFORMATION);
	return;
	/*
	if(!_CWinsock.GetState()){
		bool bStart = _CWinsock.StartWinsock();
		if(!bStart){
			MessageBox(this->Handle,"Winsock error!","Hata",MB_ICONEXCLAMATION);
			return;
		}
	}

	CMainSocket = _CWinsock.NewSocket();
	if((long)CMainSocket.GetSocket() == INVALID_SOCKET){
		MessageBox(this->Handle,"Socket error!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(!CMainSocket.Client_Connect(strServerIP,iServerPort)){
		CMainSocket.Close();
		MessageBox(this->Handle,"Baglanti hatasi!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	char* strAccountID = (char*)&strTrialID[0];

	int	iIDLen = strlen(strAccountID);
	if(iIDLen == 0)
	{
		MessageBox(this->Handle,"Deneme yapabilmek icin oyuna loader ile girmeli veya bot'u char secme ekranýnda acmalisiniz.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	char tmpID[22];	memset(tmpID,0,22);
	strcpy(tmpID,(char*)(*(DWORD*)KO_ACID));
	BYTE bKey = *strAccountID;
	StrEncrypt(&tmpID[0],bKey);

	if( memcmp(strAccountID,tmpID,iIDLen) == 1 )
	{
		BanTrialID();
		return;
	}

	DWORD dwCharBase = *(DWORD*)(KO_CHAR);
	if(dwCharBase == 0){
		CMainSocket.Close();
		MessageBox(this->Handle,"Char Init Error!","Hata!",MB_ICONSTOP);
		return;
	}

	WORD wSourceID = *(WORD*)(dwCharBase + KO_MYID);
	if(wSourceID == 0){
		CMainSocket.Close();
		MessageBox(this->Handle,"Char Init Error!","Hata!",MB_ICONSTOP);
		return;
	}

	DWORD dwSockBase = *(DWORD*)(KO_SOCK);
	if(dwSockBase == 0){
		CMainSocket.Close();
		MessageBox(this->Handle,"Sock Init Error!","Hata!",MB_ICONSTOP);
		return;
	}

	DWORD dwSockHandle = *(DWORD*)(dwSockBase + 0x4);
	if(dwSockHandle == 0 || dwSockHandle == (DWORD)-1){
		CMainSocket.Close();
		MessageBox(this->Handle,"Sock Init Error!","Hata!",MB_ICONSTOP);
		return;
	}


	char *pSend = new char [iIDLen + 14];
	memset(pSend,0,iIDLen + 14);

	int index(0);

	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_TRIAL,index);
	CMainSocket.PutWORD(pSend,sBotVersion,index);
	CMainSocket.PutWORD(pSend,wSourceID,index);
	CMainSocket.PutDWORD(pSend,dwSockHandle,index);
	CMainSocket.PutBYTE(pSend,(BYTE)iIDLen,index);
	CMainSocket.PutSTR(pSend,strAccountID,iIDLen,index);
	CMainSocket.PutPacketTail(pSend,index);
	CMainSocket.Client_Send(pSend,iIDLen + 14);

	delete[] pSend;

	int SentTick = GetTickCount(); //gecikme test

	char* pRecv = new char[iRecvSizeLogin];
	memset(pRecv,0,iRecvSizeLogin);
	DWORD nRecv(0),nRecvd(0);

	while(!nRecv){
	ioctlsocket(CMainSocket.GetSocket(),FIONREAD,&nRecv);
		if(!nRecv && GetTickCount() - SentTick >= 5000)
		{
			CMainSocket.Close();
			MessageBox(this->Handle,"Sunucudan cevap alinamadi!","Hata",MB_ICONEXCLAMATION);
			return;
		}
		Sleep(100);
	}

	if((int)nRecv != iRecvSizeLogin){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (overflow)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	do{
		nRecvd += CMainSocket.Client_Recv(pRecv+nRecvd,nRecv-nRecvd);
	} while(nRecvd < nRecv);

	int iDataHead,iDataTail,iDataSize;
	char* pData;

	iDataHead = CMainSocket.FindPacketHead(pRecv,nRecvd);
	if((DWORD)iDataHead < sizeof(wPACKETHEAD)){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	iDataTail = CMainSocket.FindPacketTail(pRecv+iDataHead,nRecvd-iDataHead);
	if((DWORD)iDataTail < 1){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	iDataSize = iDataTail;
	pData = pRecv + iDataHead;

	index = 0;

	if(CMainSocket.GetBYTE(pData,index) != bRecv_TRIAL){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	CMainSocket.Close();	//end
	BYTE bPrefix = CMainSocket.GetBYTE(pData,index);
	switch(bPrefix){

		case bTrial_INVALID:
		{
			MessageBox(this->Handle,"Account ID Hatali!","Hata",MB_ICONEXCLAMATION);
			break;
		}

		case bTrial_SUCCESS:
		{
			bPacketKey = CMainSocket.GetBYTE(pData,index);
			int iRemainingDays = CMainSocket.GetBYTE(pData,index);
			iMyUserIndex = CMainSocket.GetWORD(pData,index);
			iMyUserID = CMainSocket.GetWORD(pData,index);
			dwMyID = CMainSocket.GetWORD(pData,index);
			dwRecvdSockHandle = CMainSocket.GetDWORD(pData,index);
			iRemaining = iRemainingDays;
			char tmp[16];	itoa(iRemainingDays,tmp,10);
			char* pMsg = new char[96];
			memset(pMsg,0,96);
			strcpy(pMsg,"Login basariyla yapildi. Bot kullanima hazirdir. Mevcut gun miktariniz: ");

			if(iRemainingDays == 0xFF)
				strcat(pMsg,"Limitsiz");
			else
				strcat(pMsg,tmp);

			if(bAutoLogin == false)
				MessageBox(this->Handle,pMsg,"Login",MB_ICONINFORMATION);
			delete [] pMsg;
			delete [] pRecv;

			frmBot = new TfrmBot(NULL);
			Application->CreateForm(__classid(TfrmBot), &frmBot);
			frmBot->Show();
			SetForegroundWindow(frmBot->Handle);

			frmLogin->Hide();
			break;
		}

		case bTrial_IDUSED:
		{
			MessageBox(this->Handle,"Bu hesapta onceden deneme surumu zaten kullanilmistir. Bir daha kullanamazsiniz. Lutfen www.kojdbot.com adresinden key satin aliniz.","Hata",MB_ICONEXCLAMATION);
			break;
		}

		case bTrial_BANNED:
		{
			MessageBox(this->Handle,"Bu hesapta deneme hakki yasaklanmistir.","Ban",MB_ICONEXCLAMATION);
			break;
		}

		case bTrial_INVALIDV:
		{
			MessageBox(this->Handle,"Surum hatasi! lutfen bot'un son surumunu indirip kullaniniz.","Hata",MB_ICONEXCLAMATION);
			break;
        }

		case bTrial_DISABLED:
		{
			MessageBox(this->Handle,"Deneme surumu su an icin kapalidir. Daha sonra tekrar deneyiniz.","Hata",MB_ICONEXCLAMATION);
			break;
		}

	}

	delete [] pRecv;
	*/

}
//---------------------------------------------------------------------------

void BanTrialID()
{
	char* pStrID = (char*)&strTrialID[0];

	int iLen = strlen(pStrID);
	if( iLen == 0)
	{
		return;
	}

	if(!_CWinsock.GetState()){
		bool bStart = _CWinsock.StartWinsock();
		if(!bStart){
			MessageBox(frmLogin->Handle,"Winsock error!","Hata",MB_ICONEXCLAMATION);
			return;
		}
	}

	CMainSocket = _CWinsock.NewSocket();
	if((long)CMainSocket.GetSocket() == INVALID_SOCKET){
		MessageBox(frmLogin->Handle,"Socket error!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	//DWORD dwTrue = 1;
	//ioctlsocket(CMainSocket.GetSocket(), FIONBIO, (u_long FAR*) &dwTrue);

	if(!CMainSocket.Client_Connect(strServerIP,iServerPort)){
		CMainSocket.Close();
		MessageBox(frmLogin->Handle,"Baglanti hatasi!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	char *pSend = new char [iLen + 6];
	memset(pSend,0,iLen + 6);

	int index(0);

	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_BANTRIAL,index);
	CMainSocket.PutBYTE(pSend,(BYTE)iLen,index);
	CMainSocket.PutSTR(pSend,pStrID,iLen,index);
	CMainSocket.PutPacketTail(pSend,index);
	CMainSocket.Client_Send(pSend,iLen + 6);

	delete[] pSend;

    Sleep(1000);
	MessageBox(frmLogin->Handle,"Bu hesapta deneme hakki yasaklanmistir.","Ban",MB_ICONEXCLAMATION);
}


void __fastcall TfrmLogin::tmloginbotTimer(TObject *Sender)
{
		tmloginbot->Enabled = false;

		if(bTrialLogin == true)
		{
			this->btntrialClick(NULL);
		}
		else
		{
			txtloginid->Text = strBotID;
			txtloginpw->Text = strBotPW;
			this->btnLoginClick(NULL);
		}
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::chinterclientClick(TObject *Sender)
{
	bInterClient = chinterclient->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogin::btnchangeClick(TObject *Sender)
{

	if(txtchangekey->Text.Length() != iKeySize * 2){
		MessageBox(this->Handle,"Hatali key!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	int iPWLen = txtchangepw->Text.Length();
	int iNewPWLen = txtchangenewpw->Text.Length();
	int iNewPWLen2 = txtchangenewpw2->Text.Length();

	if(!iPWLen || !iNewPWLen || !iNewPWLen2){
		MessageBox(this->Handle,"Hatali sifre!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(iPWLen < 4 || iPWLen > 20 || iNewPWLen < 4 || iNewPWLen > 20 || iNewPWLen2 < 4 || iNewPWLen2 > 20){
		MessageBox(this->Handle,"Sifre min. 4 - max. 20 karakter olmalidir.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(iNewPWLen != iNewPWLen2 || strcmp(txtchangenewpw->Text.c_str(),txtchangenewpw2->Text.c_str()) > 0)
	{
		MessageBox(this->Handle,"Sifreler uyusmuyor!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	char* pTmpStr = txtchangenewpw->Text.c_str();
	if( !CheckValidIDPW(pTmpStr) )
	{
		MessageBox(this->Handle,"Sifrenizde ozel karakterler bulunamaz. Lutfen turkce karakter ve ozel karakter kullanmayiniz.","Hata",MB_ICONEXCLAMATION);
		return;
	}

	pTmpStr = txtchangekey->Text.c_str();
	if( !CheckIllegal(pTmpStr) )
	{
		MessageBox(this->Handle,"Hatali key!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	pTmpStr = txtchangepw->Text.c_str();
	if( !CheckValidIDPW(pTmpStr) )
	{
		MessageBox(this->Handle,"Hatali Sifre!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	if(!_CWinsock.GetState()){
		bool bStart = _CWinsock.StartWinsock();
		if(!bStart){
			MessageBox(this->Handle,"Winsock error!","Hata",MB_ICONEXCLAMATION);
			return;
		}
	}

	CMainSocket = _CWinsock.NewSocket();
	if((long)CMainSocket.GetSocket() == INVALID_SOCKET){
		MessageBox(this->Handle,"Socket error!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	//DWORD dwTrue = 1;
	//ioctlsocket(CMainSocket.GetSocket(), FIONBIO, (u_long FAR*) &dwTrue);

	if(!CMainSocket.Client_Connect(strServerIP,iServerPort)){
		CMainSocket.Close();
		MessageBox(this->Handle,"Baglanti hatasi!","Hata",MB_ICONEXCLAMATION);
		return;
	}

	int iChangePacketSize = iKeySize + 7 + iNewPWLen + iPWLen;
	char *pSend = new char[iChangePacketSize];
	memset(pSend,0,iChangePacketSize);

	int index(0);

	CMainSocket.PutPacketHead(pSend,index);
	CMainSocket.PutBYTE(pSend,bSend_CHANGEPW,index);

	long lBuffer;

	lBuffer = HexToByte(txtchangekey->Text.c_str());
	CMainSocket.PutBYTE(pSend,lBuffer,index);

	lBuffer = HexToByte(txtchangekey->Text.c_str() + 2);
	CMainSocket.PutBYTE(pSend,lBuffer,index);

	lBuffer = HexToShort(txtchangekey->Text.c_str() + 4);
	CMainSocket.PutWORD(pSend,lBuffer,index);

	lBuffer = HexToShort(txtchangekey->Text.c_str() + 8);
	CMainSocket.PutWORD(pSend,lBuffer,index);

	lBuffer = HexToLong(txtchangekey->Text.c_str() + 12);
	CMainSocket.PutDWORD(pSend,lBuffer,index);

	CMainSocket.PutBYTE(pSend,iPWLen,index);
	CMainSocket.PutSTR(pSend,txtchangepw->Text.c_str(),iPWLen,index);
	CMainSocket.PutBYTE(pSend,iNewPWLen,index);
	CMainSocket.PutSTR(pSend,txtchangenewpw->Text.c_str(),iNewPWLen,index);
	CMainSocket.PutPacketTail(pSend,index);
	CMainSocket.Client_Send(pSend,iChangePacketSize);

	delete[] pSend;

	int SentTick = GetTickCount(); //gecikme test

	char* pRecv = new char[iRecvSizeChange];
	memset(pRecv,0,iRecvSizeChange);
	DWORD nRecv(0),nRecvd(0);

	while(!nRecv){
	ioctlsocket(CMainSocket.GetSocket(),FIONREAD,&nRecv);
		if(!nRecv && GetTickCount() - SentTick >= 5000)
		{
			CMainSocket.Close();
			MessageBox(this->Handle,"Sunucudan cevap alinamadi!","Hata",MB_ICONEXCLAMATION);
			return;
		}
		Sleep(100);
	}

	if((int)nRecv != iRecvSizeChange){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (overflow)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	do{
		nRecvd += CMainSocket.Client_Recv(pRecv+nRecvd,nRecv-nRecvd);
	} while(nRecvd < nRecv);

	int iDataHead,iDataTail,iDataSize;
	char* pData;

	iDataHead = CMainSocket.FindPacketHead(pRecv,nRecvd);
	if((DWORD)iDataHead < sizeof(wPACKETHEAD)){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	iDataTail = CMainSocket.FindPacketTail(pRecv+iDataHead,nRecvd-iDataHead);
	if((DWORD)iDataTail < 1){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	iDataSize = iDataTail;
	pData = pRecv + iDataHead;

	index = 0;

	if(CMainSocket.GetBYTE(pData,index) != bRecv_CHANGEPW){
		CMainSocket.Close();
		MessageBox(this->Handle,"Yanlis paket (tanimsiz)","Hata",MB_ICONEXCLAMATION);
		return;
	}

	CMainSocket.Close();	//end
	BYTE bPrefix = CMainSocket.GetBYTE(pData,index);
	switch(bPrefix)
	{

		case bChange_INVALID:
		{
			MessageBox(this->Handle,"Sifre veya Key Yanlis!","Hata",MB_ICONEXCLAMATION);
			break;
		}

		case bChange_SUCCESS:
		{
			MessageBox(this->Handle,"Sifre basariyla degistirildi!","Sifre",MB_ICONINFORMATION);
			break;
		}

		case bChange_PAUSED:
		{
			MessageBox(this->Handle,"Sifre degistirme gecici olarak kapatilmistir. Daha sonra tekrar deneyin.","Hata",MB_ICONEXCLAMATION);
			break;
		}

	}

	delete [] pRecv;
}
//---------------------------------------------------------------------------

void MakeCaption()
{
	srand(GetTickCount());
	char abc[16]; memset(abc,0,16);

	int iLen = rand() % 15;

	for (int i = 0; i < iLen; i++)
	{
		*(abc+i) = (char)(rand() % 0xFF);
	}

	frmLogin->Caption = abc;
}

void __fastcall TfrmLogin::tmdebuggerTimer(TObject *Sender)
{
	dwCheckFuncRet = 0xFFFFFFFF;
	CheckDebugger();

	if( dwCheckFuncRet == 1 )
		return;

	LogError(1);
	OwnTerminate();
}
//---------------------------------------------------------------------------

