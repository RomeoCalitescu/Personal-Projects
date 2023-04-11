//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "About.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------

char currentPlayerSymbol = 'X';
int counterMoves = 0, counterXWins = 0, counterOWins = 0;

bool TMainForm::checkIsWinner()
{
	if(!Button1->Caption.IsEmpty() && Button1->Caption == Button2->Caption && Button1->Caption == Button3->Caption)
	{
		Button1->Font->Size = 11;
		Button2->Font->Size = 11;
		Button3->Font->Size = 11;
		Button1->Font->Style = TFontStyles() << fsBold;
		Button2->Font->Style = TFontStyles() << fsBold;
		Button3->Font->Style = TFontStyles() << fsBold;
		return true;
	}

	if(!Button4->Caption.IsEmpty() && Button4->Caption == Button5->Caption && Button4->Caption == Button6->Caption)
	{
		Button4->Font->Size = 11;
		Button5->Font->Size = 11;
		Button6->Font->Size = 11;
		Button4->Font->Style = TFontStyles() << fsBold;
		Button5->Font->Style = TFontStyles() << fsBold;
		Button6->Font->Style = TFontStyles() << fsBold;
		return true;
	}

	if(!Button7->Caption.IsEmpty() && Button7->Caption == Button8->Caption && Button7->Caption == Button9->Caption)
	{
		Button7->Font->Size = 11;
		Button8->Font->Size = 11;
		Button9->Font->Size = 11;
		Button7->Font->Style = TFontStyles() << fsBold;
		Button8->Font->Style = TFontStyles() << fsBold;
		Button9->Font->Style = TFontStyles() << fsBold;
		return true;
	}

	if(!Button1->Caption.IsEmpty() && Button1->Caption == Button4->Caption && Button1->Caption == Button7->Caption)
	{
		Button1->Font->Size = 11;
		Button4->Font->Size = 11;
		Button7->Font->Size = 11;
		Button1->Font->Style = TFontStyles() << fsBold;
		Button4->Font->Style = TFontStyles() << fsBold;
		Button7->Font->Style = TFontStyles() << fsBold;
		return true;
	}

	if(!Button2->Caption.IsEmpty() && Button2->Caption == Button5->Caption && Button2->Caption == Button8->Caption)
	{
		Button2->Font->Size = 11;
		Button5->Font->Size = 11;
		Button8->Font->Size = 11;
		Button2->Font->Style = TFontStyles() << fsBold;
		Button5->Font->Style = TFontStyles() << fsBold;
		Button8->Font->Style = TFontStyles() << fsBold;
		return true;
	}

	if(!Button3->Caption.IsEmpty() && Button3->Caption == Button6->Caption && Button3->Caption == Button9->Caption)
	{
		Button3->Font->Size = 11;
		Button6->Font->Size = 11;
		Button9->Font->Size = 11;
		Button3->Font->Style = TFontStyles() << fsBold;
		Button6->Font->Style = TFontStyles() << fsBold;
		Button9->Font->Style = TFontStyles() << fsBold;
		return true;
	}

	if(!Button1->Caption.IsEmpty() && Button1->Caption == Button5->Caption && Button1->Caption == Button9->Caption)
	{
		Button1->Font->Size = 11;
		Button5->Font->Size = 11;
		Button9->Font->Size = 11;
		Button1->Font->Style = TFontStyles() << fsBold;
		Button5->Font->Style = TFontStyles() << fsBold;
		Button9->Font->Style = TFontStyles() << fsBold;
		return true;
	}

	if(!Button3->Caption.IsEmpty() && Button3->Caption == Button5->Caption && Button3->Caption == Button7->Caption)
	{
		Button3->Font->Size = 11;
		Button5->Font->Size = 11;
		Button7->Font->Size = 11;
		Button3->Font->Style = TFontStyles() << fsBold;
		Button5->Font->Style = TFontStyles() << fsBold;
		Button7->Font->Style = TFontStyles() << fsBold;
		return true;
	}

	return false;
}

__fastcall TMainForm::TMainForm(TComponent *Owner)
	: TForm(Owner)
{
	currentPlayerSymbolLabel->Caption = currentPlayerSymbol;
	CounterXWins->Caption = counterXWins;
	CounterOWins->Caption = counterOWins;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CreateMDIChild(String Name)
{
	TMDIChild *Child;

	//--- create a new MDI child window ----
	Child = new TMDIChild(Application);
	Child->Caption = Name;
	if (FileExists (Name))
		Child->Memo1->Lines->LoadFromFile(Name);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileNew1Execute(TObject *Sender)
{
	CreateMDIChild("NONAME" + IntToStr(MDIChildCount + 1));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileOpen1Execute(TObject *Sender)
{
	if (OpenDialog->Execute())
		CreateMDIChild(OpenDialog->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HelpAbout1Execute(TObject *Sender)
{
	AboutBox->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileExit1Execute(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonClick(TObject *Sender)
{
		TButton* clickedButton = dynamic_cast<TButton*>(Sender);
		if(!clickedButton->Caption.IsEmpty() || checkIsWinner() || clickedButton == NewGameButton)
			return;
		clickedButton->Caption = currentPlayerSymbol;
		counterMoves++;

		if(checkIsWinner())
		{
			winnerLabel->Caption = "Ai castigat!!!";
			NewGameButton->Enabled = true;
			NewGameButton->Caption = "New game";
			if(currentPlayerSymbol == 'X')
			{
				counterXWins++;
				CounterXWins->Caption = counterXWins;
			} else
			{
				counterOWins++;
				CounterOWins->Caption = counterOWins;
			}
			return;
		}

		if(counterMoves == 9 && !checkIsWinner())
		{
			winnerLabel->Caption = "Remiza";
			NewGameButton->Enabled = true;
			NewGameButton->Caption = "New game";
			return;
        }

		if(currentPlayerSymbol == 'X')
			currentPlayerSymbol = 'O';
		else
			currentPlayerSymbol = 'X';
		currentPlayerSymbolLabel->Caption = currentPlayerSymbol;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::NewGameButtonClick(TObject *Sender)
{
		Button1->Caption = "";
		Button2->Caption = "";
		Button3->Caption = "";
		Button4->Caption = "";
		Button5->Caption = "";
		Button6->Caption = "";
		Button7->Caption = "";
		Button8->Caption = "";
		Button9->Caption = "";

		Button1->Font->Size = 8;
		Button2->Font->Size = 8;
		Button3->Font->Size = 8;
		Button4->Font->Size = 8;
		Button5->Font->Size = 8;
		Button6->Font->Size = 8;
		Button7->Font->Size = 8;
		Button8->Font->Size = 8;
		Button9->Font->Size = 8;

		Button1->Font->Style = TFontStyles();
		Button2->Font->Style = TFontStyles();
		Button3->Font->Style = TFontStyles();
		Button4->Font->Style = TFontStyles();
		Button5->Font->Style = TFontStyles();
		Button6->Font->Style = TFontStyles();
		Button7->Font->Style = TFontStyles();
		Button8->Font->Style = TFontStyles();
		Button9->Font->Style = TFontStyles();

		NewGameButton->Caption = "";
		NewGameButton->Enabled = false;
		winnerLabel->Caption = "";
		currentPlayerSymbol = 'X';
		currentPlayerSymbolLabel->Caption = currentPlayerSymbol;
		counterMoves = 0;
}
//---------------------------------------------------------------------------

