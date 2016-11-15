; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "FaceRecognition"
#define MyAppVersion "v1.0.2"
#define MyAppPublisher "龙脉安信"
#define MyAppURL "http://www.rmax.com/"
#define MyAppExeName "FaceRecognition.exe"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{E3489C5A-E816-4D94-B92F-9D83F9723415}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=C:\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputBaseFilename=消防人脸识别系统{#MyAppVersion}
Compression=lzma
ShowLanguageDialog=no
SolidCompression=yes
UsePreviousUserInfo=no
UsePreviousAppDir=no 
UsePreviousGroup=no 
UsePreviousLanguage=no 
UsePreviousSetupType=no 
UsePreviousTasks=no 
DisableDirPage=yes 
DisableFinishedPage=no 
DisableProgramGroupPage=yes 
DisableReadyMemo=yes 
DisableReadyPage=yes 
DisableStartupPrompt=yes 
DisableWelcomePage=no 

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; 		Description: "{cm:CreateDesktopIcon}"; 		GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1
Name: "quicklaunchicon"; 	Description: "{cm:CreateQuickLaunchIcon}"; 	GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\bin\*.dll"; 												DestDir: "{app}"; Flags: ignoreversion; Excludes: "*.d.dll"
Source: "..\bin\facerecog.db"; 										DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\feadb.db1"; 										DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\feadb.db3"; 										DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\*.jpg"; 										DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\*.json"; 										DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\*.wav"; 										DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\*.ini"; 										DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\facerecog.db"; 										DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\FaceRecognition.exe"; 								DestDir: "{app}"; Flags: ignoreversion
Source: "..\bin\faceMonitorServer.exe"; 								DestDir: "{app}"; Flags: ignoreversion

Source: "..\bin\sqldrivers\*"; 					  						DestDir: "{app}\sqldrivers"; 	Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\skins\*"; 		Excludes:"*.lib;*.pdb";				DestDir: "{app}\skins"; 		Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\skin\*"; 											DestDir: "{app}\skin"; 		Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\platforms\*"; 											DestDir: "{app}\platforms"; 	Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\photoshop\*"; 										DestDir: "{app}\photoshop"; 	Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\photo\*"; 											DestDir: "{app}\photo"; 	Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\mvp\*"; 												DestDir: "{app}\mvp"; 			Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\log\*"; 												DestDir: "{app}\log"; 			Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\bin\imageformats\*"; 												DestDir: "{app}\imageformats"; 			Flags: ignoreversion recursesubdirs createallsubdirs
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]
Name: "{group}\{#MyAppName}"; 													Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; 								Filename: "{#MyAppURL}"
Name: "{commondesktop}\{#MyAppName}"; 											Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; 	Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon