; Example Inno Setup script

#include "external.iss"

[Setup]
AppName=Example Application
AppVersion=1.2.3
AppPublisher=example.com
AppPublisherURL=http://www.example.com/
WizardStyle=modern
DefaultDirName={autopf}\Example Application
DefaultGroupName=Example Application
Compression=lzma2/max
SolidCompression=yes
Uninstallable=not PortableCheck
UninstallDisplayIcon={app}\Compil32.exe
LicenseFile=license.txt
#ifdef SIGNTOOL
SignTool=issigntool
SignTool=issigntool256
SignedUninstaller=yes
#endif

[Files]
Source: "license.txt"; DestDir: "{app}"; Flags: ignoreversion touch
Source: "files\MyApp.exe"; DestDir: "{app}"; Flags: ignoreversion signonce touch

[Code]
function PortableCheck: Boolean;
begin
  Result := ExpandConstant('{param:portable|0}') = '1';
end;
