//----------------------------------------------------------------------------
#ifndef MainH
#define MainH
//----------------------------------------------------------------------------
#include "ChildWin.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Messages.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <StdCtrls.hpp>
#include <Menus.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>
//----------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *FileNewItem;
	TMenuItem *FileOpenItem;
	TMenuItem *FileCloseItem;
	TMenuItem *Window1;
	TMenuItem *Help1;
	TMenuItem *N1;
	TMenuItem *FileExitItem;
	TMenuItem *WindowCascadeItem;
	TMenuItem *WindowTileItem;
	TMenuItem *WindowArrangeItem;
	TMenuItem *HelpAboutItem;
	TOpenDialog *OpenDialog;
	TMenuItem *FileSaveItem;
	TMenuItem *FileSaveAsItem;
	TMenuItem *Edit1;
	TMenuItem *CutItem;
	TMenuItem *CopyItem;
	TMenuItem *PasteItem;
	TMenuItem *WindowMinimizeItem;
        TStatusBar *StatusBar;
        TActionList *ActionList1;
        TEditCut *EditCut1;
        TEditCopy *EditCopy1;
        TEditPaste *EditPaste1;
        TAction *FileNew1;
        TAction *FileSave1;
        TAction *FileExit1;
        TAction *FileOpen1;
        TAction *FileSaveAs1;
        TWindowCascade *WindowCascade1;
        TWindowTileHorizontal *WindowTileHorizontal1;
        TWindowArrange *WindowArrangeAll1;
        TWindowMinimizeAll *WindowMinimizeAll1;
        TAction *HelpAbout1;
        TWindowClose *FileClose1;
        TWindowTileVertical *WindowTileVertical1;
        TMenuItem *WindowTileItem2;
        TToolBar *ToolBar2;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TToolButton *ToolButton7;
        TToolButton *ToolButton8;
        TToolButton *ToolButton9;
        TToolButton *ToolButton10;
        TToolButton *ToolButton11;
        TImageList *ImageList1;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TButton *Button8;
	TButton *Button9;
	TLabel *Label1;
	TLabel *currentPlayerSymbolLabel;
	TLabel *winnerLabel;
	TButton *NewGameButton;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *CounterXWins;
	TLabel *CounterOWins;
        void __fastcall FileNew1Execute(TObject *Sender);
        void __fastcall FileOpen1Execute(TObject *Sender);
        void __fastcall HelpAbout1Execute(TObject *Sender);
        void __fastcall FileExit1Execute(TObject *Sender);
	void __fastcall ButtonClick(TObject *Sender);
	void __fastcall NewGameButtonClick(TObject *Sender);
private:
	void __fastcall CreateMDIChild(const String Name);
	bool TMainForm::checkIsWinner();
public:
	virtual __fastcall TMainForm(TComponent *Owner);
};
//----------------------------------------------------------------------------
extern TMainForm *MainForm;
extern TMDIChild *__fastcall MDIChildCreate(void);
//----------------------------------------------------------------------------
#endif    
