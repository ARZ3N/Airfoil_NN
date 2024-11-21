$CURDIR = Get-Location
Write-Host $CURDIR
$RAW_AF_FILES = "$CURDIR\..\Dataset\Raw-data\coord_seligFmt"
$CMD_FL_PATH = "$CURDIR\cmds"
$XDATA_FL_PATH = "$CURDIR\xdata"

$cmd_fl_name = "xcmd-coord32.txt"
$letters = "x"
foreach ($ltr in $letters)
{
    foreach ($file in Get-ChildItem $RAW_AF_FILES\$ltr*.dat)
    {
            Clear-Content .\$cmd_fl_name
            $filename = (Get-Item $file).Basename
            # Write-Host $filename
            $data_fl_name = "n32_" + $filename + ".txt" 
     
            Add-Content -Path ".\$cmd_fl_name" -Value "LOAD ..\Dataset\Raw-data\coord_seligFmt\$filename.dat"
            Add-Content -Path ".\$cmd_fl_name" -Value "PPAR"
            Add-Content -Path ".\$cmd_fl_name" -Value "N 32"
            Add-Content -Path ".\$cmd_fl_name" -Value ""
            Add-Content -Path ".\$cmd_fl_name" -Value ""
            Add-Content -Path ".\$cmd_fl_name" -Value "PSAV ..\Dataset\Stage_2\N32\$data_fl_name"
            Add-Content -Path ".\$cmd_fl_name" -Value "QUIT"
     
            Write-Host $filename
            cmd /c ".\xfoil.exe < .\$cmd_fl_name"
    }
}
cd $CURDIR
