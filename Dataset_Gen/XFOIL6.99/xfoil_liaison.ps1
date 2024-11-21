$CURDIR = Get-Location
Write-Host $CURDIR
$RAW_AF_FILES = "$CURDIR\..\Dataset\Raw-data\coord_seligFmt"
$CMD_FL_PATH = "$CURDIR\cmds"
$XDATA_FL_PATH = "$CURDIR\xdata"
$letters = "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" 

foreach ($ltr in $letters)
{
    foreach ($cmdfile in Get-ChildItem $CMD_FL_PATH\cmd_$ltr*.txt)
    {
        $fname = (Get-Item $cmdfile).Basename
        cmd /c ".\xfoil.exe < .\cmds\$fname.txt"
    }
}
cd $CURDIR
