$client = new-object System.Net.WebClient
$client.DownloadFile("http://67.185.96.113:8111/update/agentInstaller.exe", "./AgentInstaller.exe")
Start-Process ./AgentInstaller.exe -Wait

$agent_install_location_popup = New-Object -ComObject Wscript.Shell
$agent_install_location_popup.Popup("Select agent install destination folder you just set.",0,"Press Okay To Continue",0x0)

Function Get-FolderName()
{
    $browseForFolderOptions = 0

    $app = New-Object -ComObject Shell.Application
    $folder = $app.BrowseForFolder(0, $Message, $browseForFolderOptions)
    if ($folder) { $selectedDirectory = $folder.Self.Path } else { $selectedDirectory = '' }
    [System.Runtime.Interopservices.Marshal]::ReleaseComObject($app) > $null
    return $selectedDirectory
}

$agent_install_location = Get-FolderName
$workspace_location = (Get-Item -Path "./../../").FullName -replace '\\','/'
Add-Content -Path "$agent_install_location\conf\buildAgent.properties" -Value "`r`nenv.P4WORKSPACE=$workspace_location"

$workspace = (Get-Item -Path "./../../").Name
$username = Read-Host 'USERNAME'

$agent_install_location_popup = New-Object -ComObject Wscript.Shell
Write-Host "Run: 'p4 -p perforce.digipen.edu:1666 sync'"
$agent_install_location_popup.Popup("Run 'p4 -p perforce.digipen.edu:1666 sync' in the window about to popup. The origonal terminal contains line for you to copy.",0,"INSTRUCTIONS:",0x0)

.\PSTools\psexec -i -s cmd.exe /k "p4 set P4USER=$username p4 set p4 set P4CLIENT=$workspace"
