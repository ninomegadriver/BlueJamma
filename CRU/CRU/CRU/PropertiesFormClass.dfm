object PropertiesForm: TPropertiesForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Display Properties'
  ClientHeight = 507
  ClientWidth = 170
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object DeviceIDGroupBox: TGroupBox
    Left = 8
    Top = 6
    Width = 154
    Height = 75
    Caption = ' Device ID '
    ParentBackground = False
    TabOrder = 0
    object DeviceIDLabel: TLabel
      Left = 9
      Top = 22
      Width = 15
      Height = 13
      Caption = 'ID:'
    end
    object SerialIDLabel: TLabel
      Left = 9
      Top = 47
      Width = 54
      Height = 13
      Caption = 'ID serial #:'
    end
    object DeviceID: TEdit
      Left = 28
      Top = 19
      Width = 69
      Height = 21
      CharCase = ecUpperCase
      MaxLength = 7
      TabOrder = 0
      OnChange = DeviceIDChange
      OnExit = DeviceIDExit
    end
    object DeviceIDResetButton: TButton
      Left = 104
      Top = 19
      Width = 42
      Height = 21
      Caption = 'Reset'
      TabOrder = 1
      OnClick = DeviceIDResetButtonClick
    end
    object SerialID: TEdit
      Left = 67
      Top = 44
      Width = 79
      Height = 21
      MaxLength = 10
      TabOrder = 2
      OnChange = SerialIDChange
      OnExit = SerialIDExit
    end
  end
  object ColorFormatsGroupBox: TGroupBox
    Left = 8
    Top = 86
    Width = 154
    Height = 107
    Caption = ' Color formats '
    TabOrder = 1
    object ColorDepthLabel: TLabel
      Left = 9
      Top = 57
      Width = 105
      Height = 13
      Caption = 'Maximum color depth:'
    end
    object YCbCr422: TCheckBox
      Left = 9
      Top = 17
      Width = 77
      Height = 17
      Caption = 'YCbCr 4:2:2'
      TabOrder = 0
      OnClick = YCbCr422Click
    end
    object YCbCr444: TCheckBox
      Left = 9
      Top = 34
      Width = 77
      Height = 17
      Caption = 'YCbCr 4:4:4'
      TabOrder = 1
      OnClick = YCbCr444Click
    end
    object ColorDepthComboBox: TComboBox
      Left = 9
      Top = 76
      Width = 136
      Height = 21
      Style = csDropDownList
      DropDownCount = 25
      ItemHeight = 13
      TabOrder = 2
      OnChange = ColorDepthComboBoxChange
    end
  end
  object NameGroupBox: TGroupBox
    Left = 8
    Top = 198
    Width = 154
    Height = 71
    Caption = ' Name (13 characters max) '
    ParentBackground = False
    TabOrder = 2
    object Name: TEdit
      Left = 9
      Top = 19
      Width = 136
      Height = 21
      MaxLength = 13
      TabOrder = 0
      OnChange = NameChange
      OnExit = NameExit
    end
    object IncludeName: TCheckBox
      Left = 9
      Top = 46
      Width = 127
      Height = 17
      Caption = 'Include if slot available'
      TabOrder = 1
      OnClick = IncludeNameClick
    end
  end
  object SerialNumberGroupBox: TGroupBox
    Left = 8
    Top = 274
    Width = 154
    Height = 71
    Caption = ' Serial number '
    ParentBackground = False
    TabOrder = 3
    object SerialNumber: TEdit
      Left = 9
      Top = 19
      Width = 136
      Height = 21
      MaxLength = 13
      TabOrder = 0
      OnChange = SerialNumberChange
      OnExit = SerialNumberExit
    end
    object IncludeSerialNumber: TCheckBox
      Left = 9
      Top = 46
      Width = 127
      Height = 17
      Caption = 'Include if slot available'
      TabOrder = 1
      OnClick = IncludeSerialNumberClick
    end
  end
  object RangeLimitsGroupBox: TGroupBox
    Left = 8
    Top = 350
    Width = 154
    Height = 121
    Caption = ' Range limits '
    TabOrder = 4
    object VRateLabel: TLabel
      Left = 9
      Top = 22
      Width = 33
      Height = 13
      Caption = 'V rate:'
    end
    object VRateDash: TShape
      Left = 77
      Top = 29
      Width = 6
      Height = 1
      Brush.Color = clWindowText
      Pen.Color = clWindowText
    end
    object HzLabel: TLabel
      Left = 118
      Top = 22
      Width = 12
      Height = 13
      Caption = 'Hz'
    end
    object HRateLabel: TLabel
      Left = 9
      Top = 47
      Width = 34
      Height = 13
      Caption = 'H rate:'
    end
    object HRateDash: TShape
      Left = 77
      Top = 54
      Width = 6
      Height = 1
      Brush.Color = clWindowText
      Pen.Color = clWindowText
    end
    object kHzLabel: TLabel
      Left = 118
      Top = 47
      Width = 17
      Height = 13
      Caption = 'kHz'
    end
    object MaxPClockLabel: TLabel
      Left = 9
      Top = 72
      Width = 75
      Height = 13
      Caption = 'Max pixel clock:'
    end
    object MHzLabel: TLabel
      Left = 124
      Top = 72
      Width = 20
      Height = 13
      Caption = 'MHz'
    end
    object MinVRate: TEdit
      Left = 48
      Top = 19
      Width = 24
      Height = 21
      MaxLength = 3
      TabOrder = 0
      OnChange = MinVRateChange
      OnExit = MinVRateExit
    end
    object MaxVRate: TEdit
      Left = 88
      Top = 19
      Width = 24
      Height = 21
      MaxLength = 3
      TabOrder = 1
      OnChange = MaxVRateChange
      OnExit = MaxVRateExit
    end
    object MinHRate: TEdit
      Left = 48
      Top = 44
      Width = 24
      Height = 21
      MaxLength = 3
      TabOrder = 2
      OnChange = MinHRateChange
      OnExit = MinHRateExit
    end
    object MaxHRate: TEdit
      Left = 88
      Top = 44
      Width = 24
      Height = 21
      MaxLength = 3
      TabOrder = 3
      OnChange = MaxHRateChange
      OnExit = MaxHRateExit
    end
    object MaxPClock: TEdit
      Left = 88
      Top = 69
      Width = 30
      Height = 21
      MaxLength = 4
      TabOrder = 4
      OnChange = MaxPClockChange
      OnExit = MaxPClockExit
    end
    object IncludeRangeLimits: TCheckBox
      Left = 9
      Top = 96
      Width = 127
      Height = 17
      Caption = 'Include if slot available'
      TabOrder = 5
      OnClick = IncludeRangeLimitsClick
    end
  end
  object FormOKButton: TButton
    Left = 7
    Top = 477
    Width = 75
    Height = 23
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 5
  end
  object FormCancelButton: TButton
    Left = 88
    Top = 477
    Width = 75
    Height = 23
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 6
  end
end
