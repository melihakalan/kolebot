object dlgAddition: TdlgAddition
  Left = 227
  Top = 108
  BorderStyle = bsDialog
  Caption = 'Eklenti'
  ClientHeight = 232
  ClientWidth = 211
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object grpbdw: TGroupBox
    Left = 8
    Top = 8
    Width = 193
    Height = 113
    Caption = 'BDW Eklentisi'
    TabOrder = 0
    object lbmonu: TLabel
      Left = 127
      Top = 21
      Width = 53
      Height = 13
      Caption = 'ID Alinmadi'
    end
    object btngetmonuid: TButton
      Left = 3
      Top = 16
      Width = 118
      Height = 25
      Caption = 'Monu ID Al (Secili)'
      TabOrder = 0
      OnClick = btngetmonuidClick
    end
    object btngetmonu: TButton
      Left = 3
      Top = 56
      Width = 118
      Height = 25
      Caption = 'Uzaktan Monu Al'
      TabOrder = 1
      OnClick = btngetmonuClick
    end
    object chautomonu: TCheckBox
      Left = 3
      Top = 87
      Width = 118
      Height = 17
      Caption = 'Otomatik Monu Al'
      TabOrder = 2
      OnClick = chautomonuClick
    end
  end
  object btnclose: TButton
    Left = 8
    Top = 199
    Width = 89
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = btncloseClick
  end
  object grpchaos: TGroupBox
    Left = 8
    Top = 127
    Width = 193
    Height = 66
    Caption = 'Chaos Eklentisi'
    TabOrder = 2
    object chdisablesaw: TCheckBox
      Left = 3
      Top = 16
      Width = 134
      Height = 17
      Caption = 'testere damage kapat'
      TabOrder = 0
      OnClick = chdisablesawClick
    end
    object chlootcubes: TCheckBox
      Left = 3
      Top = 39
      Width = 134
      Height = 17
      Caption = 'k'#252'pleri otomatik topla'
      TabOrder = 1
      OnClick = chlootcubesClick
    end
  end
  object tmmonu: TTimer
    Enabled = False
    OnTimer = tmmonuTimer
    Left = 160
    Top = 72
  end
  object tmlootcubes: TTimer
    Enabled = False
    OnTimer = tmlootcubesTimer
    Left = 152
    Top = 16
  end
  object tmchaoscheck: TTimer
    Enabled = False
    OnTimer = tmchaoscheckTimer
    Left = 160
    Top = 136
  end
end
