Sub Macro1()
'
' Macro1 Macro
'

'
Dim i As Integer
i = Range("a1").Value



    Sheets("ALLPDCHPCUFAIL10000").Columns(i).Copy
    'Selection.Copy
    Sheets("Sheet1").Select
    Columns(3).Select
    ActiveSheet.Paste
    Range("E6").Select

i = i + 1
 Range("a1").Value = i
    
End Sub
