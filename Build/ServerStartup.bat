rem Make P4 sure port is set
p4 set P4PORT=perforce.digipen.edu:1666
rem Checkout DockerBuildServer
p4 edit ./DockerBuildServer/...

rem Need Docker Installed: https://www.docker.com/get-docker Useing Lunix Containers
rem Get / Update Server
docker pull jetbrains/teamcity-server
rem Run Server
docker run -it --name teamcity-server-instance^
 -v %~dp0DockerBuildServer/data:/data/teamcity_server/datadir^
 -v %~dp0DockerBuildServer/logs:/opt/teamcity/logs^
 -p 8111:8111^
 jetbrains/teamcity-server

rem Send to source
p4 reconcile ./DockerBuildServer/...
p4 revert -a
rem p4 submit -d "Build Server Test Shutdown"

rem -e TEAMCITY_SERVER_MEM_OPTS="-Xmx4096m"^
