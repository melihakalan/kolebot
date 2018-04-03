object dlgParty: TdlgParty
  Left = 227
  Top = 108
  BorderStyle = bsDialog
  Caption = 'party'
  ClientHeight = 314
  ClientWidth = 255
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object pnPartyAdd: TBevel
    Left = 8
    Top = 8
    Width = 241
    Height = 273
    Shape = bsFrame
  end
  object btnOk: TButton
    Left = 8
    Top = 287
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = btnOkClick
  end
  object lstpartyadd: TListBox
    Left = 16
    Top = 16
    Width = 129
    Height = 153
    ItemHeight = 13
    TabOrder = 1
  end
  object txtname: TEdit
    Left = 16
    Top = 175
    Width = 129
    Height = 21
    TabOrder = 2
  end
  object btnadd: TButton
    Left = 16
    Top = 202
    Width = 57
    Height = 23
    Caption = 'isim ekle'
    TabOrder = 3
    OnClick = btnaddClick
  end
  object btnclear: TButton
    Left = 88
    Top = 202
    Width = 57
    Height = 23
    Caption = 'temizle'
    TabOrder = 4
    OnClick = btnclearClick
  end
  object btnprintparty: TButton
    Left = 151
    Top = 16
    Width = 82
    Height = 23
    Caption = '<- Party aktar'
    TabOrder = 5
    OnClick = btnprintpartyClick
  end
  object btnaddselected: TButton
    Left = 151
    Top = 45
    Width = 82
    Height = 23
    Caption = '<- Se'#231'ili ekle'
    TabOrder = 6
    OnClick = btnaddselectedClick
  end
  object grpautoadd: TGroupBox
    Left = 16
    Top = 231
    Width = 212
    Height = 41
    Caption = 'chat'#39'e gore adam al'
    TabOrder = 7
    object txtautoadd: TEdit
      Left = 3
      Top = 16
      Width = 57
      Height = 21
      TabOrder = 0
      Text = 'addpls'
    end
    object chautochatadd: TCheckBox
      Left = 66
      Top = 16
      Width = 143
      Height = 17
      Caption = 'yazani party al (all-pm)'
      TabOrder = 1
      OnClick = chautochataddClick
    end
  end
  object chaccept: TCheckBox
    Left = 151
    Top = 74
    Width = 77
    Height = 17
    Caption = 'party kabul'
    Checked = True
    State = cbChecked
    TabOrder = 8
  end
  object chrequest: TCheckBox
    Left = 151
    Top = 97
    Width = 96
    Height = 17
    Caption = 'party gonder'
    Checked = True
    State = cbChecked
    TabOrder = 9
  end
end
