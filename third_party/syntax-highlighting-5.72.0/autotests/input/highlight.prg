// Test file to test kate's clipper highlighting
// kate: hl Clipper;

//BEGIN INCLUDES
#include <clip-ui.ch>
#include "logo.ch"

#define PRGVERSION	"0.0.1"
//END

//BEGIN CODE
static ws, win
static driver := getDriver()
/* a multiline
     comment
*/

function main( formName )
	local form
	local fileName

	if empty(formName)
		?? "Usage: ./form_ui <form.xfl>&\n"
		CANCEL
	else
		fileName := formName
	endif
	ws  := UIWorkSpace()

	form := UIForm( fileName )
	win  := form:parseFile()
//	?? valtype(win),chr(10)
	if win == NIL
		CANCEL
	endif
	win:show()

	ws:run()
	ws:quit()
return 0

/* Setting dialog */
function settingsDialog()
	?? "TODO: Settings dialog&\n"
return

/* About dialog */
function aboutDialog()
	local dlg := UIWindow("About", win, "aboutDlg", .F.)
	local hl, lside, t, bb, bD

	hl  := UIHBox(,4,8)
	lside := UIVBox()
	lside:add(UIImage(eas_logo_mini,.T.))
	hl:add(lside,.F.,.F.)
	dlg:userSpace:add(hl,.T.,.T.)
	t   := UIVBox()
	hl:add(t,.T.,.T.)

	t:add(UILabel("License: GPL version 2 or later"))
	bb  := UIButtonBar()
	t:add(bb)
	bD := UIButton(win, "&Close", {|o,e| dlg:close() } )
	bb:add( bD )

	dlg:setFocus(bD)
	dlg:setDefault(bD)
	dlg:setPlacement( .T. )
	dlg:show()
return
//END