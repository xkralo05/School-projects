<?php
/*class File{
	var $nameOfFile;
	function _construct($fileName)
	{
		$this->name = $fileName;
	}
	function createAddfiles($fileName,$path)
	{
		if(!file_exists("$this->name"."in"))
		{
			$newFile = touch($path."/".)
		}
	}
}*/
$NumberOfTests = 0;
$wrongTests=0;
$passedTests=0;
$options = getopt("",["help","directory:","recursive","parse-script:","int-script:","parse-only","int-only","jexamxml:","jexamcfg:"]);


$help = array_key_exists("help", $options);
$directoryPath = array_key_exists("directory", $options);
$recursive = array_key_exists("recursive", $options);
$parsePath = array_key_exists("parse-script", $options);
$intPath = array_key_exists("int-script", $options);
$parseOnly = array_key_exists("parse-only", $options);
$intOnly= array_key_exists("int-only", $options);
$jexamxml = array_key_exists("jexamxml", $options);
$jexamcfg = array_key_exists("jexamcfg", $options);

$recursion = false;
$testOnlyInt = false;
$testOnlyParse = false;


if(($argc >2) && ($help))
{
	fputs(STDERR , "Neplatne vstupne argumenty skriptu\n");
	exit(10);
}
elseif(($argc == 2) && ($argv[1]=="--help"))
{
	printHelp();
}

if($argc == 1)
{
	if(!file_exists("parse.php"))
	{
		fputs(STDERR , "Subor parse.php nebol najdeny\n");
		exit(11);
	}
	if(!file_exists("interpert.py"))
	{
		fputs(STDERR , "Subor interpert.py nebol najdeny\n");
		exit(11);
	}
}

if ($argc-1 != count($options))
{
	fputs(STDERR , "Neplatne vstupne argumenty skriptu\n");
	exit(10);
}

if($argc >= 2 && $argc <10)
{
	if($directoryPath)
	{
		$testPath = $options["directory"];
		if (!is_dir($testPath))
		{
			fputs(STDERR , "Zadany subor neexistuje \n");
			exit(41);
		}

	}
	else
	{
		$testPath = getcwd();
	}

	if($parsePath)
	{
		$pathToParse = $options["parse-script"];
	}
	else
	{
		$pathToParse = getcwd()."/parse.php";
		
		if(!file_exists($pathToParse))
		{
			fputs(STDERR , "Subor parse.php nebol najdeny\n");
			exit(41);
		}
	}

	if($intPath)
	{
		$pathToInt = $options["int-only"];
	}
	else
	{
		$pathToInt = getcwd()."/interpret.py";
		if(!file_exists($pathToInt))
		{
			fputs(STDERR , "Subor interpert.py nebol najdeny\n");
			exit(41);
		}
	}

	if($parseOnly)
	{
		$testOnlyParse = true;
	}

	if($intOnly)
	{
		$testOnlyInt = true;
	}

	if($parseOnly && $intOnly)
	{
		fputs(STDERR , "Nepovolena kombinacia vstupnych parametrov skriptu\n");
		exit(10);
	}
	elseif($parseOnly && $intPath)
	{
		fputs(STDERR , "Nepovolena kombinacia vstupnych parametrov skriptu\n");
		exit(10);
	}
	elseif($intOnly && $parsePath)
	{
		fputs(STDERR , "Nepovolena kombinacia vstupnych parametrov skriptu\n");
		exit(10);
	}

	if($jexamxml)
	{
		$jexamxmlPath = $options["jexamxml"];
	}
	else
	{
		$jexamxmlPath = "/pub/courses/ipp/jexamxml/jexamxml.jar";
	}

	if($jexamcfg)
	{
		$jexamcfgPath = $options["jexamcfg"];
	}
	else{
		$jexamcfgPath = "/pub/courses/ipp/jexamxml/options";
	}
	
	if ($recursive)
	{
		$recursion = true;
	}
}
else
{
	fputs(STDERR , "Nespravny pocet argumentov\n");
	exit(10);
}

$testNumber = 0;


$htmlTable ="";
if ($testOnlyParse)
{
	
	$filesArr = getFiles($testPath,$recursion);
	//print_r($filesArr);
	
	foreach($filesArr as $element)
	{
		$NumberOfTests++;
		$helpNameRC = substr($element, 0, -4);
		$helpNameRC = $helpNameRC.".rc";
		$f = fopen($helpNameRC,"r");
		$expectedEC = fgets($f);
		fclose($f);
		
		exec("php7.4"." ".$pathToParse." < ".$element." >tmpTestXml_-",$exitArr,$exitCode);

		
		if($expectedEC != $exitCode){
			$htmlTable = generateTable($htmlTable ,$element,$exitCode,"",$expectedEC,"Wrong");
			$wrongTests++;
			
		}
		else if($exitCode == 0)
		{
			$helpNameOut = substr($element, 0, -4);
			$helpNameOut= $helpNameOut.".out";
			touch("DeLtA");
			$delta = getcwd();
			$delta = $delta."/"."DeLtA";
			exec("java -jar jexamxml.jar tmpTestXml_- ".$helpNameOut." ".$delta." ".$jexamcfgPath ." >TmpOUT_-");
			$file_line=array_slice(file('TmpOUT_-'), 0, 3);
			
			///print_r($file_line);
			if($file_line[2]=="Two files are identical" or $file_line[2]=="Two files are identical\n" )
			{
				$htmlTable = generateTable($htmlTable ,$element,$exitCode,"",$expectedEC,"Right");
				$passedTests++;
			}
			else{
				//echo"wrongText\n";
				$htmlTable = generateTable($htmlTable ,$element,$exitCode,"",$expectedEC,"Wrong");
				$wrongTests++;
			}

		}
		else
		{	
			$htmlTable = generateTable($htmlTable ,$element,$exitCode,"",$expectedEC,"Right");
			$passedTests++;
		}	
	}
	exec("rm -f tmpTestXml_-");
	exec("rm -f TmpOUT_-");
	exec("rm -f DeLtA");

}
elseif($testOnlyInt){
	$filesArr = getFiles($testPath,$recursion);
	foreach($filesArr as $element)
	{	
		$NumberOfTests++;
		$helpNameRC = substr($element, 0, -4);
		$helpNameRC = $helpNameRC.".rc";
		$helpNameIn = substr($element, 0, -4);
		$helpNameIn = $helpNameIn.".in";
		$f = fopen($helpNameRC,"r");
		$expectedEC = fgets($f);
		fclose($f);
		//exec("php7.4"." ".$pathToParse." < ".$element." >tmpTestXml_-",$exitArr,$exitCode);
		//echo"python3.8. ".$pathToInt." --source=".$element." --input=".$helpNameIn." >TmpOUT_- \n";
		exec("python3.8"." ".$pathToInt." --source=".$element." --input=".$helpNameIn." >TmpOUT--",$exitArr,$exitCode);
			
		

		if($expectedEC != $exitCode){
			$htmlTable = generateTable($htmlTable ,$element,"",$exitCode,$expectedEC,"Wrong");
			$wrongTests++;		
		}
		else if ($exitCode == 0)
		{
			$helpNameOut = substr($element, 0, -4);
			$helpNameOut= $helpNameOut.".out";
			//echo"$helpNameOut\n";

			exec("diff TmpOUT-- ".$helpNameOut." >TmpOut_--",$exitArr2,$exitCode);
			$f = fopen("TmpOut_--","r");
			$expectedString = fgets($f,);
			fclose($f);
			if($expectedString == "")
			{
				$htmlTable = generateTable($htmlTable ,$element,"",$exitCode,$expectedEC,"Right");
				$passedTests++;
			}
			else
			{
				$htmlTable = generateTable($htmlTable ,$element,"",$exitCode,$expectedEC,"Wrong");
				$wrongTests++;
			}
		}
		else {
			$htmlTable = generateTable($htmlTable ,$element,"",$exitCode,$expectedEC,"Right");
			$passedTests++;
		}	
	}
	exec("rm -f TmpOUT--");
	exec("rm -f TmpOut_--");
		
}
else
{
	$filesArr = getFiles($testPath,$recursion);
	foreach($filesArr as $element)
	{
		
		$NumberOfTests++;
		$helpNameRC = substr($element, 0, -4);
		$helpNameRC = $helpNameRC.".rc";
		$helpNameIn = substr($element, 0, -4);
		$helpNameIn = $helpNameIn.".in";
		$f = fopen($helpNameRC,"r");
		$expectedEC = fgets($f);
		fclose($f);
			
		exec("php7.4"." ".$pathToParse." < ".$element." >tmpTestXml_-",$exitArr,$exitCodeParse);
		if($exitCodeParse != 0)
		{
			$wrongTests++;
			$htmlTable = generateTable($htmlTable ,$element,$exitCodeParse," ",$expectedEC,"Wrong");
		}
		else
		{
			exec("python3.8"." ".$pathToInt." --source=tmpTestXml_-"." --input=".$helpNameIn." >TmpOUT--",$exitArr,$exitCodeInter);
			if($expectedEC != $exitCodeInter)
			{
				$wrongTests++;
				$htmlTable = generateTable($htmlTable ,$element,0,$exitCodeInter,$expectedEC,"Wrong");
			}
			else if ($exitCodeInter == 0)
			{
				$helpNameOut = substr($element, 0, -4);
				$helpNameOut= $helpNameOut.".out";
				exec("diff TmpOUT-- ".$helpNameOut." >TmpOut_--",$exitArr2,$exitCode);
				$f = fopen("TmpOut_--","r");
				$expectedString = fgets($f,);
				fclose($f);
				if($expectedString == "")
				{
					$passedTests++;
					$htmlTable = generateTable($htmlTable ,$element,0,$exitCodeInter,$expectedEC,"Right");
					

				}
				else
				{
					$wrongTests++;
					$htmlTable = generateTable($htmlTable ,$element,0,$exitCodeInter,$expectedEC,"Wrong");
				}
			}
			else{
				$htmlTable = generateTable($htmlTable ,$element,0,$exitCodeInter,$expectedEC,"Right");
				$passedTests++;
				
				
			}
		}
	}
	exec("rm -f tmpTestXml_-");
	exec("rm -f TmpOut_--");
	exec("rm -f TmpOUT--");

}

	/*echo"------------\n";
	echo"$passedTests\n";
	echo"$wrongTests\n";
*/
generateHTMLstart();
generateRestHTML($NumberOfTests,$passedTests,$wrongTests);
GenerateTableStart();
echo"$htmlTable";
generateTableEnd();
//generateRestHTML($NumberOfTests,$passedTests,$wrongTests);



function generateHTMLstart()
{
	echo"<!DOCTYPE html>\n";
	echo'<html lang="cs">'."\n";
	echo"<head>\n";
	echo"  <title>IPPcode21 - testy </title>\n";
	echo'  <meta charset="utf-8">'."\n";
	echo"  <style>\n";
	echo"    html{\n   font-family: Arial;\n   }\n";
	echo"    h1{\n     font-size: 30px;\n   margin: 0;\n   color: #b15e23;\n   padding-top: 20px;\n }\n";
	echo"    table, td , th {\n   border: 1px solid black;\n   }\n";
	echo"    table {\n   border-collapse: collapse;\n   \nmargin-bottom: 10px;   }\n"; 
	echo"   .Dobre{\n   color: green;\n   }\n";
	echo"   .Zle{\n   color: red;\n   }\n";
	echo" th{  \n padding-left:10px; \n padding-right:10px;";
	echo"  </style>\n";
	echo"</head>\n";
}
function GenerateTableStart(){
	echo"<table>\n
		<thead>\n
          <tr>\n
             <th>Testovaný Súbor</th>\n
             <th>parse.php</th>\n
             <th>interpret.py</th>\n
             <th>Očakávaná hodnota</th>\n
             <th>Výsledok</th>\n
          </tr>\n";
      
}

function generateTable($table,$filePath,$parseRC,$interRC,$expectedEC,$result)
{
	if ($result == "Right")
	{
		$table = $table."<tr>\n
    <td>$filePath</td>\n
    <td>$parseRC</td>\n
    <td>$interRC</td>\n
    <td>$expectedEC</td>\n".' 
    <td style="color:green;"'." >$result</td>\n" ."</tr>\n";
	}
	else{
		$table = $table."<tr>\n
   	 <td>$filePath</td>\n
    <td>$parseRC</td>\n
    <td>$interRC</td>\n
    <td>$expectedEC</td>\n".' 
    <td style="color:red;"'." >$result</td>\n" ."</tr>\n";
	}
 
	return $table;
}

function generateTableEnd(){
	echo" </thead>\n  </table>\n";
}

function generateRestHTML($NumberOfTests,$passedTests,$wrongTests)
{
	echo'<div id="main">'."\n"; 
	echo"  <h1>Výsledky testov :  <h1>\n";
	echo"  <h2>Počet vykonaných testov : ".$NumberOfTests."<h2>\n";
	echo'  <p class="Dobre"'.">Počet úspešných testov : ".$passedTests."</p>"."\n";
	echo'  <p class="Zle"'.">Počet neúspešných testov : ".$wrongTests."</p>"."\n";
	echo"</div>\n";



}
function getFiles($testPath,$recursion){
	$filesPathSrcArr = array();
	if(!$recursion){
		//echo"$testPath\n";
		$files = scandir($testPath);
		unset($files[0]);
		unset($files[1]);
		$filesSrcArr = array();
		foreach($files as $file)
		{
			if(preg_match("/.src$/",$file))
			{	
				array_push($filesSrcArr,$file);	
				array_push($filesPathSrcArr,$testPath."/".$file);
			}
			//$files = array_values($files);
		}		
		foreach($filesSrcArr as $srcFiles)
		{
			$srcFiles=substr($srcFiles, 0, -4);
			$fileRc=$testPath."/"."$srcFiles".".rc";
			//echo"$fileRc\n";

			if(!file_exists($testPath."/".$srcFiles.".in"))
			{
				touch("$testPath"."/"."$srcFiles".".in");
			}
			if(!file_exists($testPath."/".$srcFiles.".out"))
			{
				touch("$testPath"."/"."$srcFiles".".out");
			}
			
			if(!file_exists($fileRc))
			{
				
				touch("$testPath"."/"."$srcFiles".".rc");
				file_put_contents($testPath."/".$srcFiles.'.rc', "0");
			}
		}	
	}
	else
	{
		$Directory = new RecursiveDirectoryIterator($testPath);
		$Iterator = new RecursiveIteratorIterator($Directory);
		$filesSrcArr = array();	
		foreach($Iterator as $fileName)
		{	
			if(preg_match("/.src$/",$fileName))
			{	
				$filePath = $fileName->getPathName();
				array_push($filesPathSrcArr,$filePath);	
			}
		}
		
		
		foreach($filesPathSrcArr as $srcFiles)
		{
			$filee=substr($srcFiles, 0, -4);	
			if(!file_exists($filee.".in"))
			{
				
				touch("$filee".".in");
			}
			if(!file_exists($filee.".out"))
			{
				
				touch("$filee".".out");
			}
			if(!file_exists($filee.".rc"))
			{
				touch("$filee".".rc");
				file_put_contents($filee.".rc", "0");
			}
		}
	}
	return $filesPathSrcArr;
}

function printHelp()
{
	echo"test.php napoveda:\n";
	echo"--help vypise napovedu skriptu";
	echo"--directory=path sluzi na zadanie cesty v k suborovemu adresaru s testami.\n";
	echo"--recursive  testy nebude hladat len v zadanom adresari ale aj v jeho podadresaroch\n";
	echo"--parse-script=file subor so skriptom parse.php , implicitne hlada v aktualnom adresari\n";
	echo"--int-script=file subor so skriptom interpret.py, implicitne hlada v aktualnom adresari\n";
	echo"--parse-only bude testovany iba skript parse.php\n";
	echo"--int-only bude testovany iba skript interpret.py\n";
	echo"--jexamxml=file subor s JAR balikom s nastrojom A7Soft JExamXML\n";
	echo"--jexamcfg=file subor s konfiguraciou nastroja A7Soft JExamXML\n";
	exit(0);
}