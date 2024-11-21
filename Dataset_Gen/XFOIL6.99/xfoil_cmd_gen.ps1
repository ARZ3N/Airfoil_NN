$CURDIR = Get-Location
Write-Host $CURDIR
$RAW_AF_FILES = "$CURDIR\..\Dataset\Raw-data\coord_seligFmt"
$CMD_FL_PATH = "$CURDIR\cmds"
$XDATA_FL_PATH = "$CURDIR\xdata"
cd $RAW_AF_FILES
$letters = "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" 

foreach ($ltr in $letters)
{
    foreach ($file in Get-ChildItem $RAW_AF_FILES\$ltr*.dat)
    {
        $filename = (Get-Item $file).Basename
        # Write-Host $filename
        $cmd_fl_name = "cmd_" + $filename + ".txt"
        $data_fl_name = "xdata_" + $filename + ".txt" 

        New-Item -Path $CMD_FL_PATH -Name "$cmd_fl_name" -Force
        Clear-Content $CMD_FL_PATH\$cmd_fl_name

        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value "LOAD ..\Dataset\Raw-data\coord_seligFmt\$filename.dat"
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value "PPAR"
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value "N 200"
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value ""
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value ""
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value "OPER"
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value "VISC 1.3e7"
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value "MACH 0.3"
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value "ITER 100"
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value "PACC"
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value ".\xdata\$data_fl_name"
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value ""
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value "ASeq -2 10 1"
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value ""
        Add-Content -Path "$CMD_FL_PATH\$cmd_fl_name" -Value "quit"

    }
}
cd $CMD_FL_PATH

# Write-Host " # Command file dir contents: "
# ls

cd $CURDIR
