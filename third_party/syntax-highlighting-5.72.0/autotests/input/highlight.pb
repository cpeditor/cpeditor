; This is a test file for kate's PureBasic highlighting.

; BMP2x Converter by Sven Langenkamp

UseJPEGImageEncoder()
UsePNGImageEncoder()

Declare Convert(JPEG)

Enumeration 1
    #JPEG
    #PNG
EndEnumeration

; BEGIN section


; END

Global Count
Global file.s
Global filename.s
Global fileext.s
Global OutputFormat
Global JPEGQuality

Count        = 0
OutputFormat = 1
JPEGQuality  = -1

; MAIN PROGRAM------------------------------------------------------------------

;Request Options
PrintN("Output Format")
PrintN("  [1] JPEG")
PrintN("  [2] PNG")
Print ("> ")
OutputFormat = Int(Input())

Select OutputFormat
    Case #JPEG:
        fileext = ".jpg"
        ;Request JPEG Quality
        PrintN("")
        PrintN("JPEG Quality")
        PrintN("  [0-10]")
        Print ("> ")
        JPEGQuality = Int(Input())
    
    Case #PNG:
        fileext = ".png"
EndSelect

;Convert all BMP files in the current directory
ExamineDirectory(0, "", "*.bmp")
While NextDirectoryEntry()   
    file     = DirectoryEntryName()
    filename = GetFilePart(file)
    
    If LoadImage(0, file)
        Select OutputFormat
	    Case #JPEG:
	      Convert(JPEGQuality)
	      
	    Case #PNG:
	        Convert(-1)
	EndSelect
	Count = Count +1
    EndIf
    
Wend

PrintN(Str(Count) + " files converted")
CloseConsole()

; PROCUDURES--------------------------------------------------------------------

Procedure Convert(JPEG)
    Shared filename, fileext
    
    If JPEG > -1
        SaveImage(0, filename + fileext, #PB_ImagePlugin_JPEG, JPEG)
    Else
        SaveImage(0, filename + fileext, #PB_ImagePlugin_PNG)    
    EndIf
    PrintN(file + " converted to " + filename + fileext)
EndProcedure