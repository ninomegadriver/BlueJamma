//---------------------------------------------------------------------------
#include "Common.h"
#pragma hdrstop

#include "HDMIResolutionFormClass.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
THDMIResolutionForm *HDMIResolutionForm;
//---------------------------------------------------------------------------
__fastcall THDMIResolutionForm::THDMIResolutionForm(TComponent *Owner) : TCommonForm(Owner)
{
	Refreshing = false;
}
//---------------------------------------------------------------------------
bool THDMIResolutionForm::Connect(HDMIResolutionClass &NewHDMIResolution)
{
	HDMIResolution = &NewHDMIResolution;
	return true;
}
//---------------------------------------------------------------------------
bool THDMIResolutionForm::Refresh(void *Value)
{
	char Text[TEXTSIZE];

	Refreshing = true;

	if (!HDMIResolution->IsOther())
	{
		FormatLabel->Visible = true;
		CodeLabel->Visible = false;
		CodeRangeLabel->Visible = false;
		FormatComboBox->Visible = true;
		Code->Visible = false;
	}
	else
	{
		FormatLabel->Visible = false;
		CodeLabel->Visible = true;
		CodeRangeLabel->Visible = true;
		FormatComboBox->Visible = false;
		Code->Visible = true;
	}

	if (Value != ModeComboBox)
		ModeComboBox->ItemIndex = HDMIResolution->GetMode();

	if (Value != FormatComboBox)
		FormatComboBox->ItemIndex = HDMIResolution->GetFormat();

	if (Value != Code && IntegerToText(HDMIResolution->GetCode(), Text, TEXTSIZE))
		Code->Text = Text;

	Code->Font->Color = GetTextColor(HDMIResolution->IsValidCode());

	if (Value != FormOKButton)
		FormOKButton->Enabled = HDMIResolution->IsValid();

	Refreshing = false;
	return true;
}
//---------------------------------------------------------------------------
bool THDMIResolutionForm::InitModeComboBox()
{
	int ItemIndex;
	int Index;
	char Text[TEXTSIZE];

	ItemIndex = ModeComboBox->ItemIndex;
	ModeComboBox->Items->BeginUpdate();
	ModeComboBox->Clear();

	for (Index = 0; HDMIResolution->GetModeText(Index, Text, TEXTSIZE); Index++)
		ModeComboBox->Items->Add(Text);

	ModeComboBox->ItemIndex = ItemIndex;
	ModeComboBox->Items->EndUpdate();
	return true;
}
//---------------------------------------------------------------------------
bool THDMIResolutionForm::InitFormatComboBox()
{
	int ItemIndex;
	int Index;
	char Text[TEXTSIZE];

	ItemIndex = FormatComboBox->ItemIndex;
	FormatComboBox->Items->BeginUpdate();
	FormatComboBox->Clear();

	for (Index = 0; HDMIResolution->GetFormatText(Index, Text, TEXTSIZE); Index++)
		FormatComboBox->Items->Add(Text);

	FormatComboBox->ItemIndex = ItemIndex;
	FormatComboBox->Items->EndUpdate();
	return true;
}
//---------------------------------------------------------------------------
bool THDMIResolutionForm::ScaleControls()
{
	ModeComboBox->Width = FormButtonsWidth - PaddingWidth * 2;
	ModeComboBox->Height = ComboBoxHeight;
	ModeComboBox->Left = PaddingWidth;
	ModeComboBox->Top = PaddingTop;

	FormatComboBox->Width = ModeComboBox->Width - FormatLabel->Width - LabelSpacing;
	FormatComboBox->Height = ComboBoxHeight;

	FormatLabel->Left = ModeComboBox->Left;
	FormatComboBox->Left = FormatLabel->Left + FormatLabel->Width + LabelSpacing;

	FormatLabel->Top = ModeComboBox->Top + ModeComboBox->Height + Scale + 4;
	FormatComboBox->Top = FormatLabel->Top - 4;

	Code->AutoSize = false;
	Code->Width = Code->MaxLength * NumberWidth + TextBoxPadding;
	Code->Height = TextBoxHeight;

	CodeLabel->Left = ModeComboBox->Left + (ModeComboBox->Width - CodeLabel->Width - LabelSpacing - Code->Width - PostLabelSpacing - CodeRangeLabel->Width + 1) / 2;
	Code->Left = CodeLabel->Left + CodeLabel->Width + LabelSpacing;
	CodeRangeLabel->Left = Code->Left + Code->Width + PostLabelSpacing;

	CodeLabel->Top = FormatLabel->Top - 1;
	Code->Top = CodeLabel->Top - 3;
	CodeRangeLabel->Top = Code->Top + 3;

	ResolutionGroupBox->Width = ModeComboBox->Left + ModeComboBox->Width + PaddingWidth;
	ResolutionGroupBox->Height = FormatComboBox->Top + FormatComboBox->Height + PaddingBottom;
	ResolutionGroupBox->Left = Scale;
	ResolutionGroupBox->Top = GroupBoxTop;

	FormOKButton->Width = FormButtonWidth;
	FormOKButton->Height = FormButtonHeight;
	FormOKButton->Top = ResolutionGroupBox->Top + ResolutionGroupBox->Height + GroupBoxBottom + Scale + ButtonTop;
	FixButtonCaption(FormOKButton, Canvas->TextWidth(FormOKButton->Caption));

	FormCancelButton->Width = FormButtonWidth;
	FormCancelButton->Height = FormButtonHeight;
	FormCancelButton->Top = FormOKButton->Top;
	FixButtonCaption(FormCancelButton, Canvas->TextWidth(FormCancelButton->Caption));

	FormCancelButton->Left = ResolutionGroupBox->Left + ResolutionGroupBox->Width - ButtonRight - FormCancelButton->Width;
	FormOKButton->Left = FormCancelButton->Left - ButtonLeft - Scale - ButtonRight - FormOKButton->Width;

	ClientWidth = FormCancelButton->Left + FormCancelButton->Width + ButtonRight + Scale;
	ClientHeight = FormCancelButton->Top + FormCancelButton->Height + ButtonBottom + Scale;
	return true;
}
//---------------------------------------------------------------------------
void __fastcall THDMIResolutionForm::FormCreate(TObject *Sender)
{
	ScaleControls();
}
//---------------------------------------------------------------------------
void __fastcall THDMIResolutionForm::FormShow(TObject *Sender)
{
	InitModeComboBox();
	InitFormatComboBox();
	Refresh(NULL);
}
//---------------------------------------------------------------------------
void __fastcall THDMIResolutionForm::ModeComboBoxChange(TObject *Sender)
{
	if (Refreshing)
		return;

	HDMIResolution->SetMode(ModeComboBox->ItemIndex);
	InitFormatComboBox();
	Refresh(ModeComboBox);
}
//---------------------------------------------------------------------------
void __fastcall THDMIResolutionForm::FormatComboBoxChange(TObject *Sender)
{
	if (Refreshing)
		return;

	HDMIResolution->SetFormat(FormatComboBox->ItemIndex);
	Refresh(FormatComboBox);
}
//---------------------------------------------------------------------------
void __fastcall THDMIResolutionForm::CodeChange(TObject *Sender)
{
	if (Refreshing)
		return;

	HDMIResolution->SetCode(TextToInteger(Code->Text.c_str()));
	Refresh(Code);
}
//---------------------------------------------------------------------------
void __fastcall THDMIResolutionForm::CodeExit(TObject *Sender)
{
	Refresh(NULL);
}
//---------------------------------------------------------------------------
