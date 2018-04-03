object dlgInterClient: TdlgInterClient
  Left = 227
  Top = 108
  BorderStyle = bsDialog
  Caption = 'InterClient Y'#246'netici'
  ClientHeight = 595
  ClientWidth = 241
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object grpinterclient: TGroupBox
    Left = 8
    Top = 8
    Width = 225
    Height = 548
    Caption = 'interclient y'#246'netici -  toplu islemler'
    TabOrder = 0
    object Label1: TLabel
      Left = 23
      Top = 267
      Width = 39
      Height = 13
      Caption = 'Mesafe:'
    end
    object btnmakeparty: TButton
      Left = 15
      Top = 55
      Width = 194
      Height = 25
      Caption = 'Party kur'
      TabOrder = 0
      OnClick = btnmakepartyClick
    end
    object btnstartall: TButton
      Left = 15
      Top = 24
      Width = 94
      Height = 25
      Caption = 'Bot'#39'lar'#305' ba'#351'lat'
      TabOrder = 1
      OnClick = btnstartallClick
    end
    object btnstopall: TButton
      Left = 115
      Top = 24
      Width = 94
      Height = 25
      Caption = 'Bot'#39'lar'#305' durdur'
      TabOrder = 2
      OnClick = btnstopallClick
    end
    object btnaddmob: TButton
      Left = 15
      Top = 86
      Width = 194
      Height = 25
      Caption = 'Se'#231'ili yarat'#305#287#305' hepsine ekle'
      TabOrder = 3
      OnClick = btnaddmobClick
    end
    object btntown: TButton
      Left = 15
      Top = 322
      Width = 194
      Height = 25
      Caption = 'Town '
      TabOrder = 4
      OnClick = btntownClick
    end
    object btnsit: TButton
      Left = 15
      Top = 384
      Width = 194
      Height = 25
      Caption = 'Otur'
      TabOrder = 5
      OnClick = btnsitClick
    end
    object btnstand: TButton
      Left = 15
      Top = 415
      Width = 194
      Height = 25
      Caption = 'Kalk'
      TabOrder = 6
      OnClick = btnstandClick
    end
    object btnexit: TButton
      Left = 15
      Top = 508
      Width = 194
      Height = 25
      Caption = 'Oyunlar'#305' kapat'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 7
      OnClick = btnexitClick
    end
    object btnwear: TButton
      Left = 15
      Top = 117
      Width = 194
      Height = 25
      Caption = 'Ezik silah'#305'n'#305' giy'
      TabOrder = 8
      OnClick = btnwearClick
    end
    object btnexpand: TButton
      Left = 15
      Top = 233
      Width = 194
      Height = 25
      Caption = 'Slota yay'#305'l'
      TabOrder = 9
      OnClick = btnexpandClick
    end
    object btnfollow: TButton
      Left = 15
      Top = 148
      Width = 194
      Height = 25
      Caption = 'PT Ba'#351'kan'#305' takip aktifle'#351'tir'
      TabOrder = 10
      OnClick = btnfollowClick
    end
    object btnnofollow: TButton
      Left = 15
      Top = 179
      Width = 194
      Height = 25
      Caption = 'PT Ba'#351'kan'#305' takip durdur'
      TabOrder = 11
      OnClick = btnnofollowClick
    end
    object btnsave: TButton
      Left = 15
      Top = 477
      Width = 194
      Height = 25
      Caption = 'Ayarlar'#305' kaydet'
      TabOrder = 12
      OnClick = btnsaveClick
    end
    object txtexpand: TEdit
      Left = 68
      Top = 264
      Width = 57
      Height = 21
      TabOrder = 13
      Text = '10'
    end
    object btnsuicide: TButton
      Left = 15
      Top = 353
      Width = 194
      Height = 25
      Caption = #304'ntihar'
      TabOrder = 14
      OnClick = btnsuicideClick
    end
    object btncall: TButton
      Left = 15
      Top = 291
      Width = 194
      Height = 25
      Caption = 'Yan'#305'na '#231'a'#287#305'r'
      TabOrder = 15
      OnClick = btncallClick
    end
    object chstick: TCheckBox
      Left = 23
      Top = 210
      Width = 199
      Height = 17
      Caption = #304#231' '#304#231'e Takip (Tren Speedhack)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 16
      OnClick = chstickClick
    end
    object btnrota: TButton
      Left = 15
      Top = 446
      Width = 194
      Height = 25
      Caption = 'Rotalar'#305' hepsine da'#287#305't'
      TabOrder = 17
      OnClick = btnrotaClick
    end
  end
  object btnclose: TButton
    Left = 8
    Top = 562
    Width = 225
    Height = 25
    Caption = 'Tamam'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = btncloseClick
  end
end
