; Inno Setup Script for Snake Game with Icon

[Setup]
AppName=Snake Game
AppVersion=1.0.0
AppPublisher=Ahmad Elshawadfy
AppPublisherURL=https://github.com/ahmedelshawadfy/snake-game
DefaultDirName={pf}\Snake Game
DefaultGroupName=Snake Game
OutputDir=Installer
OutputBaseFilename=SnakeGame-Setup
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
WizardStyle=modern
SetupIconFile=game_icon.ico

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Main executable
Source: "x64\Release\Snake_Game.exe"; DestDir: "{app}"; Flags: ignoreversion
; Game icon
Source: "game_icon.ico"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Snake Game"; Filename: "{app}\Snake_Game.exe"; IconFilename: "{app}\game_icon.ico"
Name: "{group}\{cm:UninstallProgram,Snake Game}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Snake Game"; Filename: "{app}\Snake_Game.exe"; IconFilename: "{app}\game_icon.ico"; Tasks: desktopicon

[Run]
Filename: "{app}\Snake_Game.exe"; Description: "{cm:LaunchProgram,Snake Game}"; Flags: nowait postinstall skipifsilent