# Uploads the binary files from the program output to the 
# repository for long-term storage
#
# Parameter 1: build number
# Parameter 2: checkout root directory
#
# Assuming to be running from the directory with the binaries
#
param(
[parameter(mandatory=$true,Position=1)]
[string]$BuildNumber,

[parameter(mandatory=$true,Position=2)]
[string]$CheckoutRoot,

[parameter(mandatory=$true,Position=3)]
[string]$SVNusername,

[parameter(mandatory=$true,Position=4)]
[string]$SVNpassword
)

$DestinationPath = "$CheckoutRoot/Releases/$BuildNumber/"

# Create release directory
New-Item $DestinationPath -ItemType Directory

# Copy binaries into release directory
copy ./* $DestinationPath

# Commit changes
svn add $DestinationPath --parents
svn commit $CheckoutRoot --message "Automated upload of release $BuildNumber from continuous integration server." --username $SVNusername --password $SVNpassword
