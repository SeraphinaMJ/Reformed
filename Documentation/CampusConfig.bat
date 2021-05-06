set PATH="c:\Program Files\Python37";c:\sandbox\openJDK10.0.2\jdk-10.0.2\bin;%PATH%
"c:\Program Files\Python37\Scripts\pip" install -U --user sphinx
"c:\Program Files\Python37\Scripts\pip" install -U --user sphinxcontrib-plantuml
"c:\Program Files\Python37\Scripts\pip" install -U --user sphinx_rtd_theme
"c:\Program Files\Python37\Scripts\pip" install -U --user m2r

rem set SPHINXBUILD="c:\Program Files\Python37\Scripts\sphinx-build.exe"
rem Above line doesn't work with user directory, find the file manually and set variable
for /F "tokens=*" %%g IN ('where /r %userprofile% sphinx-build.exe /f') do (set SPHINXBUILD=%%g)

rem set GRAPHVIZ_DOT=c:\sandbox\graphviz-2.38\release\bin\dot.exe
for /F "tokens=*" %%g IN ('where /r C:\sandbox dot.exe /f') do (set GRAPHVIZ_DOT=%%g)

rem set PLANTUML_JAR=c:\sandbox\plantuml\plantuml.?.jar
for /F "tokens=*" %%g IN ('where /r C:\sandbox plantuml*.jar /f') do (set PLANTUML_JAR=%%g)

pause
