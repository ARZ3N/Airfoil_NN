# Unzip Selig Airfoil archive @ .\Dataset\Raw-data
$CURDIR = Get-Location
Expand-Archive $CURDIR\Dataset\Raw-data\seligFmt.zip -DestinationPath $CURDIR\Dataset\Raw-data\coord_seligFmt
cd $CURDIR