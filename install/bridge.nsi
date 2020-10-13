!include "MUI.nsh"
!insertmacro MUI_LANGUAGE "SimpChinese"

Name "Bridge"
OutFile "InstallBridge.exe"
InstallDir "$PROGRAMFILES\Bridge"	

Section
  SetOutPath "$INSTDIR\bearer"
  File "bearer\qgenericbearer.dll"
  SetOutPath "$INSTDIR\iconengines"
  File "iconengines\qsvgicon.dll"
  SetOutPath "$INSTDIR\imageformats"
  File "imageformats\qsvg.dll"
  SetOutPath "$INSTDIR\platforms"
  File "platforms\qwindows.dll"
  SetOutPath "$INSTDIR\styles"
  File "styles\qwindowsvistastyle.dll"
  SetOutPath $INSTDIR
  File "vccorlib140.dll"
  File "vcruntime140.dll"
  File "msvcp140.dll"
  File "msvcp140_1.dll"
  File "Qt5Core.dll"
  File "Qt5Gui.dll"
  File "Qt5Network.dll"
  File "Qt5Svg.dll"
  File "Qt5WebSockets.dll"
  File "Qt5Widgets.dll"
  File "bridge.exe"
  WriteUninstaller "$INSTDIR\uninstaller.exe"
  Exec "$INSTDIR\bridge.exe"
SectionEnd

Section "Uninstall"
  RMDir /r $INSTDIR
SectionEnd