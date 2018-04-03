object dlgCZ: TdlgCZ
  Left = 227
  Top = 108
  BorderStyle = bsDialog
  Caption = 'intihar ve kacis'
  ClientHeight = 467
  ClientWidth = 342
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object grpsuicide: TGroupBox
    Left = 8
    Top = 8
    Width = 326
    Height = 65
    Caption = 'intihar et'
    TabOrder = 0
    object chsuicide1: TCheckBox
      Left = 3
      Top = 16
      Width = 174
      Height = 17
      Caption = 'karsi irktan oyuncu yaklastiginda '
      TabOrder = 0
      OnClick = chsuicide1Click
    end
    object chsuicide2: TCheckBox
      Left = 3
      Top = 39
      Width = 118
      Height = 17
      Caption = 'hp dustugunde < %'
      TabOrder = 1
      OnClick = chsuicide2Click
    end
    object txtsuicidelimit: TEdit
      Left = 127
      Top = 37
      Width = 34
      Height = 21
      TabOrder = 2
      Text = '25'
      OnChange = txtsuicidelimitChange
    end
  end
  object grpflee: TGroupBox
    Left = 8
    Top = 194
    Width = 326
    Height = 234
    Caption = 'karsi irktan kac'
    TabOrder = 1
    object Label1: TLabel
      Left = 3
      Top = 32
      Width = 304
      Height = 13
      Caption = 
        '[ karsi irk geldiginde bot cizdiginiz rotaya gider, belirttigini' +
        'z sure '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clPurple
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsUnderline]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 3
      Top = 51
      Width = 157
      Height = 13
      Caption = 'orada bekler ve slota geri doner]'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clPurple
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsUnderline]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 3
      Top = 70
      Width = 24
      Height = 13
      Caption = 'rota:'
    end
    object Label4: TLabel
      Left = 128
      Top = 70
      Width = 126
      Height = 13
      Caption = 'beklenecek s'#252're (saniye): '
    end
    object chflee: TCheckBox
      Left = 3
      Top = 16
      Width = 126
      Height = 17
      Caption = 'karsi irktan kacis aktif'
      TabOrder = 0
      OnClick = chfleeClick
    end
    object lstflee: TListBox
      Left = 33
      Top = 70
      Width = 89
      Height = 83
      ItemHeight = 13
      TabOrder = 1
    end
    object btnfleeadd: TButton
      Left = 33
      Top = 159
      Width = 88
      Height = 25
      Caption = 'XY Ekle'
      TabOrder = 2
      OnClick = btnfleeaddClick
    end
    object btnfleeclear: TButton
      Left = 33
      Top = 190
      Width = 88
      Height = 25
      Caption = 'Temizle'
      TabOrder = 3
      OnClick = btnfleeclearClick
    end
    object txtfleewait: TEdit
      Left = 260
      Top = 67
      Width = 33
      Height = 21
      TabOrder = 4
      Text = '20'
      OnChange = txtfleewaitChange
    end
    object chfleetown: TCheckBox
      Left = 128
      Top = 89
      Width = 195
      Height = 17
      Caption = 'd'#246'nd'#252#287#252'nde gitmemi'#351'se town at'
      TabOrder = 5
      OnClick = chfleetownClick
    end
    object chfleering: TCheckBox
      Left = 128
      Top = 112
      Width = 195
      Height = 17
      Caption = 'd'#246'nd'#252#287#252'nde gitmemi'#351'se zil cal'
      TabOrder = 6
      OnClick = chfleeringClick
    end
  end
  object btnclose: TButton
    Left = 8
    Top = 434
    Width = 65
    Height = 25
    Caption = 'OK'
    TabOrder = 2
    OnClick = btncloseClick
  end
  object grpother: TGroupBox
    Left = 8
    Top = 79
    Width = 326
    Height = 109
    Caption = 'diger'
    TabOrder = 3
    object chstoponnopt: TCheckBox
      Left = 3
      Top = 16
      Width = 158
      Height = 17
      Caption = 'party bozulunca atak durdur'
      TabOrder = 0
      OnClick = chstoponnoptClick
    end
    object chleaveondie: TCheckBox
      Left = 3
      Top = 39
      Width = 158
      Height = 17
      Caption = #246'l'#252'nce partiden ayril'
      TabOrder = 1
      OnClick = chleaveondieClick
    end
    object chstartinpt: TCheckBox
      Left = 3
      Top = 62
      Width = 158
      Height = 17
      Caption = 'party girince atak ba'#351'lat'
      TabOrder = 2
      OnClick = chstartinptClick
    end
    object chmeat: TCheckBox
      Left = 3
      Top = 85
      Width = 94
      Height = 17
      Caption = 'oto so'#287'an bas'
      TabOrder = 3
      OnClick = chmeatClick
    end
  end
  object tmsuicide: TTimer
    Enabled = False
    Interval = 250
    OnTimer = tmsuicideTimer
    Left = 256
    Top = 32
  end
  object tmptcheck: TTimer
    Enabled = False
    Interval = 3000
    OnTimer = tmptcheckTimer
    Left = 256
    Top = 128
  end
  object tmflee: TTimer
    Enabled = False
    Interval = 250
    OnTimer = tmfleeTimer
    Left = 248
    Top = 352
  end
end
