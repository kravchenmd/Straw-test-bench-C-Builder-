//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include "VCLTee.TeeLinearGauge.hpp"
#include "VCLTee.TeeNumericGauge.hpp"
#include "VCLTee.CurvFitt.hpp"
#include "VCLTee.TeeSeriesStats.hpp"
#include <VCLTee.TeeFunci.hpp>
#include "VCLTee.TeePageNumTool.hpp"
#include "VCLTee.TeeTools.hpp"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TStaticText *StaticText1;
	TPanel *Panel1;
	TChart *Chart1;
	TLineSeries *Series1;
	TComboBox *ComboBox1;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox2;
	TSpeedButton *SpeedButton1;
	TLabel *Label6;
	TMemo *Memo1;
	TBitBtn *BitBtn1;
	TSeriesStatsTool *ChartTool1;
	TPageNumTool *ChartTool2;
	TStaticText *StaticText2;
	TComboBox *ComboBox2;
	TEdit *Edit1;
	TLabel *Label3;
	TEdit *Edit2;
	TStaticText *StaticText3;
	TBitBtn *BitBtn2;
	TBitBtn *BitBtn3;
	TStaticText *StaticText4;
	TEdit *Edit3;
	TEdit *Edit4;
	TStaticText *StaticText5;
	TStaticText *StaticText6;
	TBitBtn *BitBtn4;
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall BitBtn3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
