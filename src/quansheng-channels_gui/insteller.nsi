# Basic definitions
!define APPNAME "Quansheng channel tool"
!define COMPANYNAME "RadioZ.org"
!define PUBLISHER "Josip Tisljar Matausic"
!define DISPLAY_VERSION "0.0.1"
!define DESCRIPTION "Convert between channel formats for different UV-K5 firmwares"
!define MUI_ICON "installer-icon.ico"
!define MUI_UNICON "installer-icon.ico"

Icon "installer-icon.ico"

# Ask for administrative rights (only for system-wide install)
RequestExecutionLevel user

# Include logic library
!include LogicLib.nsh
!include MUI2.nsh

# Interface settings
Name "${APPNAME}"
OutFile "${APPNAME} setup.exe"
InstallDir "$PROGRAMFILES\${COMPANYNAME}\${APPNAME}"
InstallDirRegKey HKCU "Software\${COMPANYNAME}\${APPNAME}" "Install_Dir"

# License file (must use CRLF endings)
####LicenseData "license.txt"

# Installer pages
!insertmacro MUI_PAGE_WELCOME
####!insertmacro MUI_PAGE_LICENSE "license.txt"
Page custom PortablePage PortablePageLeave
!define MUI_PAGE_CUSTOMFUNCTION_PRE DirectoryPagePre
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_FINISHPAGE_RUN "$INSTDIR\quansheng-channels_gui.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Launch ${APPNAME}"
!insertmacro MUI_PAGE_FINISH

# Uninstaller pages
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_UNPAGE_FINISH

# Language
!insertmacro MUI_LANGUAGE "English"

# Variables
Var INSTALL_FOR_ALL_USERS
Var CREATE_DESKTOP_SHORTCUT
Var CREATE_START_MENU_ENTRY

Var RADIO_ONLY_ME
Var RADIO_ALL_USERS
Var CHECKBOX_START_MENU
Var CHECKBOX_DESKTOP

Function un.RelaunchAsAdmin
    GetFullPathName $0 "$INSTDIR"
    #System::Call "kernel32::GetCurrentDirectory(i ${NSIS_MAX_STRLEN}, t .r0)"

    # Build command line to run as admin
    StrCpy $1 "cmd.exe"
    StrCpy $2 '/c "cd /d "$0" && ".\uninstall.exe" $CMDLINE"'

    DetailPrint "Relaunching: $1 $2"

    ExecShell "runas" $1 $2
    Quit
FunctionEnd

Function PortablePage
    nsDialogs::Create 1018
    Pop $0
    ${If} $0 == error
        Abort
    ${EndIf}

    !insertmacro MUI_HEADER_TEXT "System integration" "Tell the wizard what system components should we integrate into."
 
    ${NSD_CreateLabel} 0 0 100% 12u "Who should be able to use this app?"
    Pop $R0

    ${NSD_CreateRadioButton} 0 16u 100% 12u "Only for me (recommended)"
    Pop $RADIO_ONLY_ME
    ${If} $INSTALL_FOR_ALL_USERS == "0"
        ${NSD_SetState} $RADIO_ONLY_ME ${BST_CHECKED}
    ${EndIf}

    ${NSD_CreateRadioButton} 0 32u 100% 12u "All users (requires administrator rights)"
    Pop $RADIO_ALL_USERS
    ${If} $INSTALL_FOR_ALL_USERS == "1"
        ${NSD_SetState} $RADIO_ALL_USERS ${BST_CHECKED}
    ${EndIf}

    ${NSD_CreateCheckbox} 0 56u 100% 12u "Create start menu entry"
    Pop $CHECKBOX_START_MENU
    ${NSD_SetState} $CHECKBOX_START_MENU $CREATE_START_MENU_ENTRY

    ${NSD_CreateCheckbox} 0 72u 100% 12u "Create desktop shortcut"
    Pop $CHECKBOX_DESKTOP
    ${NSD_SetState} $CHECKBOX_DESKTOP $CREATE_DESKTOP_SHORTCUT

    nsDialogs::Show
FunctionEnd

Function PortablePageLeave
    ${NSD_GetState} $RADIO_ONLY_ME $0
    ${NSD_GetState} $RADIO_ALL_USERS $1
    ${If} $0 == ${BST_CHECKED}
        StrCpy $INSTALL_FOR_ALL_USERS 0
    ${ElseIf} $1 == ${BST_CHECKED}
        StrCpy $INSTALL_FOR_ALL_USERS 1
    ${EndIf}

    ${NSD_GetState} $CHECKBOX_START_MENU $CREATE_START_MENU_ENTRY
    ${NSD_GetState} $CHECKBOX_DESKTOP $CREATE_DESKTOP_SHORTCUT

    ${If} $INSTALL_FOR_ALL_USERS == "1"
        UserInfo::GetAccountType
        Pop $1
        ${If} $1 != "admin"
            ExecShell "runas" "$EXEPATH" "$EXEDIR"
            Quit
        ${EndIf}
        SetShellVarContext all
    ${EndIf}
    Return
FunctionEnd

Function DirectoryPagePre
    ${If} $INSTALL_FOR_ALL_USERS == "1"
        SetShellVarContext all
        StrCpy $INSTDIR "$PROGRAMFILES\${COMPANYNAME}\${APPNAME}"
    ${Else}
        SetShellVarContext current
        StrCpy $INSTDIR "$LOCALAPPDATA\${COMPANYNAME}\${APPNAME}"
    ${EndIf}
FunctionEnd

Function .onInit
    UserInfo::GetAccountType
    Pop $0
    ${If} $0 == "admin"
        ; User is admin, default to all users install
        StrCpy $INSTALL_FOR_ALL_USERS 1
        SetShellVarContext all
    ${Else}
        ; User is not admin, default to current user install
        StrCpy $INSTALL_FOR_ALL_USERS 0
        SetShellVarContext current
    ${EndIf}

    ${If} $CREATE_START_MENU_ENTRY == "" ; default to checked
        StrCpy $CREATE_START_MENU_ENTRY ${BST_CHECKED}
    ${EndIf}
    ${If} $CREATE_DESKTOP_SHORTCUT == ""
        StrCpy $CREATE_DESKTOP_SHORTCUT ${BST_UNCHECKED}
    ${EndIf}
FunctionEnd

Section "install"
    SetOutPath $INSTDIR
    ${If} $INSTALL_FOR_ALL_USERS == "1"
        SetShellVarContext all
    ${Else}
        SetShellVarContext current
    ${EndIf}

    ${If} $INSTALL_FOR_ALL_USERS == "1"
        WriteRegStr HKLM "Software\${COMPANYNAME}\${APPNAME}" "AllUsers" "$INSTALL_FOR_ALL_USERS"
    ${Else}
        WriteRegStr HKCU "Software\${COMPANYNAME}\${APPNAME}" "AllUsers" "$INSTALL_FOR_ALL_USERS"
    ${EndIf}

    ${If} $INSTALL_FOR_ALL_USERS == "1"
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayIcon" "$INSTDIR\quansheng-channels_gui.exe"
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "Publisher" "${PUBLISHER}"
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayVersion" ${DISPLAY_VERSION}
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "InstallLocation" "$INSTDIR"
        WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoModify" 1
        WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoRepair" 1
    ${Else}
        WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
        WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
        WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayIcon" "$INSTDIR\quansheng-channels_gui.exe"
        WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "Publisher" "${PUBLISHER}"
        WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayVersion" ${DISPLAY_VERSION}
        WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "InstallLocation" "$INSTDIR"
        WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoModify" 1
        WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoRepair" 1
    ${EndIf}

    File quansheng-channels_gui.exe
    File libquansheng-channels.dll
    File iup.dll
    File iupimglib.dll

    WriteUninstaller "$INSTDIR\uninstall.exe"

    ${If} $CREATE_START_MENU_ENTRY == ${BST_CHECKED}
        CreateShortcut "$SMPROGRAMS\${APPNAME}.lnk" "$INSTDIR\quansheng-channels_gui.exe"
        CreateShortcut "$SMPROGRAMS\${APPNAME} Uninstall.lnk" "$INSTDIR\uninstall.exe"
    ${EndIf}

    ${If} $CREATE_DESKTOP_SHORTCUT == ${BST_CHECKED}
        CreateShortcut "$DESKTOP\${APPNAME}.lnk" "$INSTDIR\quansheng-channels_gui.exe"
    ${EndIf}
SectionEnd

Function un.onInit
    ReadRegStr $R0 HKLM "Software\${COMPANYNAME}\${APPNAME}" "AllUsers"
    ${If} $R0 == "1"
        UserInfo::GetAccountType
        Pop $1
        ${If} $1 != "admin"
            Call un.RelaunchAsAdmin
            Quit
        ${EndIf}
        SetShellVarContext all
    ${Else}
        SetShellVarContext current
    ${EndIf}
FunctionEnd

Section "uninstall"
    MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION "Please make sure quansheng-channels_gui.exe is closed before uninstalling. Otherwise garbage may be left on your system afterwards." IDOK +2 IDCANCEL +3
    Abort Quit;

    ReadRegStr $R0 HKLM "Software\${COMPANYNAME}\${APPNAME}" "AllUsers"
    ${If} $R0 == "1"
        SetShellVarContext all
        DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
        DeleteRegKey HKLM "Software\${COMPANYNAME}\${APPNAME}"
    ${Else}
        ReadRegStr $R1 HKCU "Software\${COMPANYNAME}\${APPNAME}" "AllUsers"
        ${If} $R1 == "1"
            SetShellVarContext all
            DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
            DeleteRegKey HKLM "Software\${COMPANYNAME}\${APPNAME}"
        ${Else}
            SetShellVarContext current
            DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
            DeleteRegKey HKCU "Software\${COMPANYNAME}\${APPNAME}"
        ${EndIf}
    ${EndIf}

    Delete "$SMPROGRAMS\${APPNAME}.lnk"
    Delete "$SMPROGRAMS\${APPNAME} Uninstall.lnk"
    Delete "$DESKTOP\${APPNAME}.lnk"

    Delete "$INSTDIR\quansheng-channels_gui.exe"
    Delete "$INSTDIR\libquansheng-channels.dll"
    Delete "$INSTDIR\iup.dll"
    Delete "$INSTDIR\iupimglib.dll"
    Delete "$INSTDIR\uninstall.exe"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"

    RMDir $INSTDIR
SectionEnd
