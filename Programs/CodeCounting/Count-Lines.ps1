#/*! *********************************************************************+
#\file    Count-Lines.ps1
#\author  Aaron Damyen
#\par     Secondary Authors: 
#\par     Course: GAM300
#\par     System: Tools CLOC
#
#\brief
#  Generates the CLOC data file of the project which can be viewed
#  with the Code Report Master.xlsx file
#
#+************************************************************************/
$path = "../../Project/ProjectBoomerang/"
$excludeDirs = ".vs,Assets,data,PropertyFiles,TestResults,x64,dependencies_working,ImGui,Dependencies" # comma separated, no spaces
$excludeFile = "CLOCExclude.txt"
$outputFile = "STICodeCount.csv"
$locText = (.\cloc-1.74.exe -xml --by-file -quiet --exclude-list-file=$excludeFile --exclude_dir=$excludeDirs $path 2> $null )
$loc = New-Object -TypeName System.Xml.XmlDocument
$locText = $locText.Replace("<?xml version=""1.0""?>", "");
$loc.LoadXml($locText)

for($i = 0 ; $i -lt $loc.results.files.file.Count;$i++) {
  $authorLine = (Get-Content $loc.results.files.file[$i].name | Select-String -Pattern "\\author").Line
  if($authorLine -eq $NULL) { $authorLine = "" }
  else { $authorLine = $authorLine.Substring($authorLine.IndexOf("\author") + 7).Trim() }
  $loc.results.files.file[$i].SetAttribute("author", $authorLine.ToString())
} 

#$loc.results.files.file | format-table -Property name,comment,code,language,author -autosize
$loc.results.files.file | export-csv $outputFile
