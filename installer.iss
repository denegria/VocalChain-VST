; VocalChain VST Installer
; Built with Inno Setup 6

#define MyAppName "VocalChain"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "VocalChain"
#define MyAppURL "https://github.com/vocalchain"

[Setup]
AppId={{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
DefaultDirName={commonpf}\Common Files\VST3\{#MyAppName}.vst3
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
OutputDir=C:\tmp\installer
OutputBaseFilename=VocalChain-Setup-v{#MyAppVersion}
Compression=lzma2
SolidCompression=yes
PrivilegesRequired=admin
WizardStyle=modern
WizardSizePercent=100
DisableDirPage=yes
UninstallDisplayIcon={app}\Contents\x86_64-win\VocalChain.vst3

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Messages]
WelcomeLabel1=Welcome to VocalChain
WelcomeLabel2=This will install VocalChain VST3 v{#MyAppVersion} on your computer.%n%nVocalChain gives you one-click access to industry-level vocal chains inspired by your favorite artists.%n%nPresets: Yeat, Drake, Juice WRLD, Kanye, Lil Uzi Vert%n%nClick Next to continue.

[Files]
; VST3 bundle (entire folder structure)
Source: "C:\tmp\vst-build\build\VocalChain_artefacts\Release\VST3\VocalChain.vst3\*"; DestDir: "{commonpf}\Common Files\VST3\VocalChain.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\Uninstall VocalChain"; Filename: "{uninstallexe}"

[Run]
Filename: "cmd"; Parameters: "/c echo VocalChain installed successfully!"; Description: "Installation complete"; Flags: runhidden

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    MsgBox('VocalChain VST3 installed successfully!' + #13#10 + #13#10 +
           'To use in FL Studio:' + #13#10 +
           '1. Open FL Studio' + #13#10 +
           '2. Go to Options > Manage Plugins' + #13#10 +
           '3. Click "Find Plugins"' + #13#10 +
           '4. VocalChain will appear in your plugin list' + #13#10 + #13#10 +
           'Enjoy making fire vocals!',
           mbInformation, MB_OK);
  end;
end;
