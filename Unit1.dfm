object Form1: TForm1
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'Straw test bench'
  ClientHeight = 694
  ClientWidth = 662
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 657
    Height = 226
    Caption = 'COM-port'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object SpeedButton1: TSpeedButton
      Left = 16
      Top = 99
      Width = 106
      Height = 22
      AllowAllUp = True
      GroupIndex = 1
      Caption = 'Connect'
      OnClick = SpeedButton1Click
    end
    object Label6: TLabel
      Left = 312
      Top = 23
      Width = 21
      Height = 13
      Caption = 'Log:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      Visible = False
    end
    object Label3: TLabel
      Left = 16
      Top = 141
      Width = 112
      Height = 13
      Caption = 'Data for sending to SM:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      Visible = False
    end
    object StaticText1: TStaticText
      Left = 16
      Top = 19
      Width = 56
      Height = 17
      Caption = 'Port AER:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
    end
    object ComboBox1: TComboBox
      Left = 78
      Top = 13
      Width = 97
      Height = 21
      TabOrder = 1
      Text = 'COM1'
      Items.Strings = (
        'COM1'
        'COM2'
        'COM3'
        'COM4'
        'COM5'
        'COM6'
        'COM7'
        'COM8'
        'COM9'
        'COM10'
        'COM11'
        'COM12')
    end
    object CheckBox1: TCheckBox
      Left = 193
      Top = 40
      Width = 98
      Height = 21
      Caption = #1047#1072#1087#1080#1089#1100' '#1074' '#1092#1072#1081#1083
      Checked = True
      DoubleBuffered = False
      ParentDoubleBuffered = False
      State = cbChecked
      TabOrder = 2
    end
    object CheckBox2: TCheckBox
      Left = 193
      Top = 15
      Width = 80
      Height = 17
      Caption = #1054#1082#1085#1086' LOG'
      Checked = True
      State = cbChecked
      TabOrder = 3
    end
    object Memo1: TMemo
      Left = 312
      Top = 42
      Width = 337
      Height = 112
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      ScrollBars = ssBoth
      TabOrder = 4
      Visible = False
    end
    object BitBtn1: TBitBtn
      Left = 550
      Top = 11
      Width = 99
      Height = 25
      Caption = 'Clear log window'
      TabOrder = 5
      Visible = False
      OnClick = BitBtn1Click
    end
    object StaticText2: TStaticText
      Left = 16
      Top = 64
      Width = 51
      Height = 17
      Caption = 'Port SM:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 6
    end
    object ComboBox2: TComboBox
      Left = 78
      Top = 60
      Width = 97
      Height = 21
      TabOrder = 7
      Text = 'COM1'
      Items.Strings = (
        'COM1'
        'COM2'
        'COM3'
        'COM4'
        'COM5'
        'COM6'
        'COM7'
        'COM8'
        'COM9'
        'COM10'
        'COM11'
        'COM12')
    end
    object Edit1: TEdit
      Left = 16
      Top = 160
      Width = 275
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      MaxLength = 254
      ParentFont = False
      TabOrder = 8
      Visible = False
    end
    object Edit2: TEdit
      Left = 418
      Top = 189
      Width = 71
      Height = 21
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      MaxLength = 254
      ParentFont = False
      TabOrder = 9
      Text = '10'
      Visible = False
    end
    object StaticText3: TStaticText
      Left = 312
      Top = 195
      Width = 100
      Height = 17
      Caption = 'Step, mm (1-10):'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 10
      Visible = False
    end
    object BitBtn2: TBitBtn
      Left = 16
      Top = 187
      Width = 75
      Height = 25
      Caption = 'Send to SM'
      TabOrder = 11
      Visible = False
      OnClick = BitBtn2Click
    end
    object BitBtn3: TBitBtn
      Left = 512
      Top = 187
      Width = 137
      Height = 25
      Caption = 'Start measurment'
      TabOrder = 12
      Visible = False
      OnClick = BitBtn3Click
    end
    object StaticText4: TStaticText
      Left = 312
      Top = 164
      Width = 83
      Height = 17
      Caption = 'Measure from'
      TabOrder = 13
      Visible = False
    end
    object Edit3: TEdit
      Left = 401
      Top = 160
      Width = 32
      Height = 21
      TabOrder = 14
      Text = '0'
      Visible = False
    end
    object Edit4: TEdit
      Left = 486
      Top = 160
      Width = 35
      Height = 21
      TabOrder = 15
      Text = '600'
      Visible = False
    end
    object StaticText5: TStaticText
      Left = 439
      Top = 166
      Width = 41
      Height = 17
      Caption = 'mm to'
      TabOrder = 16
      Visible = False
    end
    object StaticText6: TStaticText
      Left = 527
      Top = 164
      Width = 26
      Height = 17
      Caption = 'mm'
      TabOrder = 17
      Visible = False
    end
    object BitBtn4: TBitBtn
      Left = 655
      Top = 189
      Width = 75
      Height = 25
      Caption = 'BitBtn4'
      TabOrder = 18
      Visible = False
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 232
    Width = 657
    Height = 457
    TabOrder = 1
  end
  object Chart1: TChart
    Left = 8
    Top = 232
    Width = 641
    Height = 449
    BackWall.Brush.Gradient.Direction = gdBottomTop
    BackWall.Brush.Gradient.EndColor = clWhite
    BackWall.Brush.Gradient.StartColor = 15395562
    BackWall.Brush.Gradient.Visible = True
    BackWall.Transparent = False
    Foot.Font.Color = clBlue
    Foot.Font.Name = 'Verdana'
    Gradient.Direction = gdBottomTop
    Gradient.EndColor = clWhite
    Gradient.MidColor = 15395562
    Gradient.StartColor = 15395562
    Gradient.Visible = True
    LeftWall.Color = 14745599
    Legend.Alignment = laTop
    Legend.CheckBoxes = True
    Legend.Font.Name = 'Verdana'
    Legend.Font.Quality = fqClearType
    Legend.LegendStyle = lsLastValues
    Legend.ResizeChart = False
    Legend.Shadow.Transparency = 0
    Legend.TextSymbolGap = 6
    Legend.Title.Font.Quality = fqClearType
    Legend.TopPos = -10
    RightWall.Color = 14745599
    Title.Font.Name = 'Verdana'
    Title.Font.Quality = fqClearType
    Title.Font.DefaultFont = True
    Title.Font.Shadow.Smooth = False
    Title.Font.Shadow.Visible = False
    Title.Text.Strings = (
      'Average diameter (dx+dy)/2')
    BottomAxis.Automatic = False
    BottomAxis.AutomaticMaximum = False
    BottomAxis.AutomaticMinimum = False
    BottomAxis.Axis.Color = 4210752
    BottomAxis.Grid.Color = 11119017
    BottomAxis.LabelsFormat.Font.Color = 6579300
    BottomAxis.LabelsFormat.Font.Style = [fsBold]
    BottomAxis.Maximum = 12.083839781956920000
    BottomAxis.Minimum = 11.916160218042990000
    BottomAxis.MinorTickLength = 1
    BottomAxis.MinorTicks.Color = 6579300
    BottomAxis.TickLength = 10
    BottomAxis.Ticks.Color = 6579300
    BottomAxis.TicksInner.Color = 11119017
    BottomAxis.Title.Caption = 'time'
    BottomAxis.Title.Font.Color = 6579300
    BottomAxis.Title.Font.Name = 'Verdana'
    BottomAxis.Title.Visible = False
    DepthAxis.Axis.Color = 4210752
    DepthAxis.Grid.Color = 11119017
    DepthAxis.LabelsFormat.Font.Name = 'Verdana'
    DepthAxis.TicksInner.Color = 11119017
    DepthAxis.Title.Font.Name = 'Verdana'
    DepthTopAxis.Axis.Color = 4210752
    DepthTopAxis.Grid.Color = 11119017
    DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
    DepthTopAxis.TicksInner.Color = 11119017
    DepthTopAxis.Title.Font.Name = 'Verdana'
    LeftAxis.Axis.Color = 4210752
    LeftAxis.Grid.Color = 11119017
    LeftAxis.LabelsFormat.Font.Name = 'Verdana'
    LeftAxis.MaximumOffset = 100
    LeftAxis.MinimumOffset = 20
    LeftAxis.TicksInner.Color = 11119017
    LeftAxis.Title.Font.Name = 'Verdana'
    Panning.MouseWheel = pmwNone
    RightAxis.Automatic = False
    RightAxis.AutomaticMaximum = False
    RightAxis.AutomaticMinimum = False
    RightAxis.Axis.Color = 4210752
    RightAxis.Grid.Color = 11119017
    RightAxis.LabelsFormat.Font.Name = 'Verdana'
    RightAxis.TicksInner.Color = 11119017
    RightAxis.Title.Font.Name = 'Verdana'
    TopAxis.Automatic = False
    TopAxis.AutomaticMaximum = False
    TopAxis.AutomaticMinimum = False
    TopAxis.Axis.Color = 4210752
    TopAxis.Grid.Color = 11119017
    TopAxis.LabelsFormat.Font.Name = 'Verdana'
    TopAxis.TicksInner.Color = 11119017
    TopAxis.Title.Font.Name = 'Verdana'
    View3D = False
    Zoom.MouseWheel = pmwNormal
    TabOrder = 2
    Visible = False
    DefaultCanvas = 'TGDIPlusCanvas'
    PrintMargins = (
      15
      19
      15
      19)
    ColorPaletteIndex = 8
    object Series1: TLineSeries
      HoverElement = [heCurrent]
      Brush.BackColor = clDefault
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object ChartTool1: TSeriesStatsTool
      ShowInEditor = False
      Series = Series1
    end
    object ChartTool2: TPageNumTool
      Active = False
      Shape.Text = 'Page 1 of 1'
      Shape.TextAlignment = taCenter
      Shape.Visible = False
      DisabledBrush.Gradient.EndColor = clSilver
      DisabledBrush.Gradient.Visible = True
      DisabledButton.EndColor = clSilver
      DisabledButton.Visible = True
      EnabledBrush.Gradient.EndColor = clSilver
      EnabledBrush.Gradient.StartColor = clBlack
      EnabledBrush.Gradient.Visible = True
      EnabledButton.EndColor = clSilver
      EnabledButton.StartColor = clBlack
      EnabledButton.Visible = True
    end
  end
end
