<?php
/**
 * Nazov souboru: parse.php
 * Popis: Projekt 1 do předmětu IPP 2020/21, FIT VUT
 * Vytvořil: Kristian Kralovic (xkralo05)
 * Datum: 16.3.2021
 */
ini_set('display_errors', 'stderr');
$found_header = false;
$params_ok = false;
$is_ok_var = false;
$instruction_order = 0;
// Kontrola vstupnych parametrov 
if ($argc == 1 )
{
	$params_ok = true;
}
elseif ($argc == 2 && ( $argv[1] == "-h" || $argv[1] == "--help"))
{
	echo "parse.php napoveda : \n";
	echo "Skript typu filter (parse.php v jazyku PHP 7.4) nacita zo standardneho vstupu zdrojový kód v IPP-code21 a zkontroluje lexikální a syntaktickou správnost kódu a vypíše na standardnívýstup XML reprezentaci programu\n";
	echo "-h vypise napovedu \n";
	echo "--help vypise napovedu\n";
	exit(0);
}
elseif(	$argc > 2 )
{
	process_error (10);
}
elseif ( $argc == 2  && ($argv[1] != "-h" || $argv[1] != "--help"))
{
	process_error(10);
}	
 // Ak boli zadane spravne argumenty zacne spracovanie vstupneho suboru
if($params_ok)
{
	// kontrola spravnosti hlavicky (IPPcode21)
	while (!$found_header)
	{
		if(($line = fgets(STDIN)) == false)
		{
			process_error(11);
			break;
		}
		$line =strtoupper(trim($line));
		$helpline = explode('#',$line);
		if(preg_match('~^\s*#~', $line) || preg_match('~^\s*$~', $line))
		{
			continue;
		}
		elseif(strtoupper(trim($helpline[0]))!= ".IPPCODE21")
		{
			process_error(21);
		}	
		else
		{
			$found_header = true;
		}

	}
	$xml_out = new DOMDocument('1.0', 'UTF-8');
	$program_start_xml = $xml_out->createElement('program');
    $program_start_xml->setAttribute('language','IPPcode21');
	$xml_out->appendChild($program_start_xml);
    // Nacitanie riadku zo stdin
	while($line = fgets(STDIN))
	{
		$is_ok_var = false;
		$is_ok_symb1 = false;
		$is_ok_symb2 = false;
		$is_ok_label = false;
		$is_ok_type = false;
		$help_line = do_one_whitespace($line);
		$help_line = delete_comments($help_line);
		$line = $help_line;
		$line = trim($line);
		$split = explode(' ',$line);
		// spracovanie instrukcii podla poctu a typu argumentov
		switch(strtoupper($split[0]))
		{
			case null :
				break;
			case "CREATEFRAME" :
			case "PUSHFRAME" :
			case "POPFRAME" :
			case "RETURN" :
			case "BREAK" :
					if(count($split) == 1)
					{
						$instruction_order++;
						process_zero_params_instruction($split[0] ,$instruction_order);
					}
					else
					{	
					 	process_error(23);  
					}
					break;
			case "DEFVAR" :	
			case "POPS" :
					if(count($split) == 2)
					{
						$is_ok_var = check_var($split[1]); 
						if($is_ok_var)
						{
							$instruction_order++;
							process_one_var_instruction($split[0], $split[1],$instruction_order);
						}
						else
						{
							process_error(23);  
						}
					}
					else
					{
						process_error(23); 	
					}
					break;			
			case "PUSHS" :
			case "WRITE" :
			case "EXIT" :
			case "DPRINT" :
					if (count($split) == 2)
					{
						$is_ok_symb1 = check_symb($split[1]);
						if($is_ok_symb1)
						{
							$instruction_order++;
							process_one_symb_instruction($split[0], $split[1],$instruction_order);
						}
						else
						{
							process_error(23);  
						}
					}
					else
					{
						process_error(23);  
					}
					break;			
			case "JUMP" :
			case "LABEL" :
			case "CALL" :
					if (count($split) == 2)
					{
						$is_ok_label = check_label($split[1]);
						if($is_ok_label)
						{
							$instruction_order++;
							process_only_label_instruction($split[0], $split[1],$instruction_order);
						}
						else
						{							
							process_error(23); 
						}	
					}
					else
					{
						process_error(23); 					}
					break;
			case "READ" :
					if (count($split) == 3)
					{

						$is_ok_var = check_var($split[1]);
						$is_ok_type = check_type($split[2]);
						if($is_ok_var && $is_ok_type)
						{
							$instruction_order++;
							process_read($split[0],$split[1],$split[2],$instruction_order);
						}
						else
						{ 
							process_error(23);  
						}
					}	
					else
					{
						process_error(23); 
					}
					break;		
			case "TYPE" :
			case "MOVE"	:
			case "INT2CHAR" :
			case "STRLEN" :
					if (count($split) == 3)
					{
						$is_ok_var = check_var($split[1]);
						$is_ok_symb1 = check_symb($split[2]);
						if($is_ok_var && $is_ok_symb1)
						{
							$instruction_order++;
							process_one_var_one_symb($split[0],$split[1],$split[2],$instruction_order);
						}
						else
						{
							process_error(23); 
						}
					}	
					else
					{
						process_error(23); 
					}
					break;
			case "ADD" :
			case "SUB" :
			case "IDIV":
			case "MUL" :
			case "IMUL" :
			case "LT" :
			case "GT" :
			case "EQ" :
			case "AND" :
			case "OR" :
			case "NOT" :
			case "STRI2INT" :
			case "CONCAT" :
			case "GETCHAR" :
			case "SETCHAR" :
					if (count($split) == 4)
					{
						$is_ok_var = check_var($split[1]);
						$is_ok_symb1 = check_symb($split[2]);
						$is_ok_symb2 = check_symb($split[3]);
						if($is_ok_var && ($is_ok_symb1 && $is_ok_symb2))
						{
							$instruction_order++;
							process_one_var_two_symb($split[0],$split[1],$split[2],$split[3],$instruction_order);
						}
						else
						{
							process_error(23); 
						}
					}	
					else
					{

						process_error(23); 
					}
					break;
			case "JUMPIFEQ" :
			case "JUMPIFNEQ" :
					if (count($split) == 4)
					{
						$is_ok_label = check_label($split[1]);
						$is_ok_symb1 = check_symb($split[2]);
						$is_ok_symb2 = check_symb($split[3]);
						if($is_ok_label && ($is_ok_symb1 && $is_ok_symb2))
						{
							$instruction_order++;
							process_label_two_symb($split[0],$split[1],$split[2],$split[3],$instruction_order);
						}
						else
						{
							process_error(23); 
						}
					}	
					else
					{
						process_error(23);
					}	

					break;

			default :
						process_error(22);   
					break;
		}

	}
}

$xml_out->formatOutput = true;
echo ($xml_out->saveXML());
/*
 * Funkcia odstrani prebytocne biele znaky a vrati upraveny riadok
 * @param $line_to_change obsahuje jeden riadok nacitany zo stdin
*/
function do_one_whitespace($line_to_change)
{
	$changed_line = preg_replace('/\s+/',' ',$line_to_change);
	return $changed_line;
}
/*
 * Funkcia odstrani komentar a vrati upraveny riadok  
 * @param $line_to_change obsahuje jeden riadok nacitany zo stdin
*/
function delete_comments($line_to_change)
{
	$changed_line = preg_replace('/#.*/','',$line_to_change);
	return $changed_line;
}
/*
 * Funkcia vytvori XML output pre instrukcie s 0 parametrami (CREATEFRAME,POPFRAME...)
 * @Param $instruction_to_process obsahuje OPCODE pre spracovanie 
 * @Param $instruction_order obsahuje poradie spracovavanej instrukcie 
*/
function process_zero_params_instruction($instruction_to_process,$instruction_order)
{
	global $xml_out , $program_start_xml;

	$instruction = $xml_out->createElement('instruction');
	$instruction->setAttribute('order',$instruction_order);
	$instruction->setAttribute('opcode',strtoupper($instruction_to_process));
	$program_start_xml->appendChild($instruction);
}
/*
 * Funkcia vytvori XML output pre instrukcie s 1 argumentom a to variable
 * @Param $instruction_to_process obsahuje OPCODE pre spracovanie 
 * @Param $var_to_process obsahuje nazov variable 
 * @Param $instruction_order obsahuje poradie spracovavanej instrukcie
*/

function process_one_var_instruction($instruction_to_process,$var_to_process,$instruction_order)
{
	global $xml_out , $program_start_xml;
	
	$instruction = $xml_out->createElement('instruction');
	$instruction->setAttribute('order',$instruction_order);
	$instruction->setAttribute('opcode',strtoupper($instruction_to_process));

	$var = $xml_out->createElement('arg1',htmlspecialchars($var_to_process));
	$var->setAttribute('type','var');
	$instruction->appendChild($var);
	$program_start_xml->appendChild($instruction);
}
/*
 * Funkcia vytvori XML output pre instrukcie s 1 argumentom a to label
 * @Param $instruction_to_process obsahuje OPCODE pre spracovanie 
 * @Param $label_to_process obsahuje nazov labelu
 * @Param $instruction_order obsahuje poradie spracovavanej instrukcie
*/

function process_only_label_instruction($instruction_to_process,$label_to_process,$instruction_order)
{
	global $xml_out , $program_start_xml;
	
	$instruction = $xml_out->createElement('instruction');
	$instruction->setAttribute('order',$instruction_order);
	$instruction->setAttribute('opcode',strtoupper($instruction_to_process));

	$label = $xml_out->createElement('arg1',htmlspecialchars($label_to_process));
	$label->setAttribute('type','label');
	$instruction->appendChild($label);
	$program_start_xml->appendChild($instruction);
}
/*
 * Funkcia vytvori XML output pre instrukcie s 1 argumentom a to symbolom
 * @Param $instruction_to_process obsahuje OPCODE pre spracovanie 
 * @Param $symb_to_process obsahuje spracovavany symbol
 * @Param $instruction_order obsahuje poradie spracovavanej instrukcie
*/ 

function process_one_symb_instruction($instruction_to_process,$symb_to_process,$instruction_order)
{	
	global $xml_out , $program_start_xml;
	
	$instruction = $xml_out->createElement('instruction');
	$instruction->setAttribute('order',$instruction_order);
	$instruction->setAttribute('opcode',strtoupper($instruction_to_process));
	
	$help_symbol = $symb_to_process;
	$help_symbol = explode('@',$symb_to_process);
	$help_string = explode('string@',$symb_to_process);	
    
	// switch pre vytvorenie spravneho XML atributu 
	switch ($help_symbol[0])
	{
		case "int":
			$symb = $xml_out->createElement('arg1',$help_symbol[1]);
			$symb->setAttribute('type',$help_symbol[0]);

			break;
		case "string" :
			$symb = $xml_out->createElement('arg1',htmlspecialchars($help_string[1]));
			$symb->setAttribute('type',$help_symbol[0]);
			break;
		case "bool"	:
			$symb = $xml_out->createElement('arg1',$help_symbol[1]);
			$symb->setAttribute('type',$help_symbol[0]);
			break;
		case "nil" :
			$symb = $xml_out->createElement('arg1',$help_symbol[1]);
			$symb->setAttribute('type',$help_symbol[0]);
			break;
		case "GF" :
		case "LF" :
		case "TF" :
			$symb = $xml_out->createElement('arg1',$symb_to_process);
			$symb->setAttribute('type','var');
		break;		
	}

	$instruction->appendChild($symb);
	$program_start_xml->appendChild($instruction);
}
/*
 * Funkcia vytvori XML output pre instrukcie s 2 argumentami a to variable a symbol
 * @Param $instruction_to_process obsahuje OPCODE pre spracovanie 
 * @Param $var_to_process obsahuje nazov variable 
 * @Param $symb_to_process obsahuje spracovavany symbol
 * @Param $instruction_order obsahuje poradie spracovavanej instrukcie
*/
function process_one_var_one_symb($instruction_to_process ,$var_to_process,$symb_to_process,$instruction_order)
{
	global $xml_out , $program_start_xml;
	
	$instruction = $xml_out->createElement('instruction');
	$instruction->setAttribute('order',$instruction_order);
	$instruction->setAttribute('opcode',strtoupper($instruction_to_process));

	$var = $xml_out->createElement('arg1',htmlspecialchars($var_to_process));
	$var->setAttribute('type','var');

	$help_symbol = $symb_to_process;
	$help_symbol = explode('@',$symb_to_process);
	$help_string = explode('string@',$symb_to_process);	

	// switch pre vytvorenie spravneho XML atributu
	switch ($help_symbol[0])
	{
		case "int":
			$symb = $xml_out->createElement('arg2',$help_symbol[1]);
			$symb->setAttribute('type',$help_symbol[0]);

			break;
		case "string" :
			$symb = $xml_out->createElement('arg2',htmlspecialchars($help_string[1]));
			$symb->setAttribute('type',$help_symbol[0]);
			break;
		case "bool"	:
			$symb = $xml_out->createElement('arg2',$help_symbol[1]);
			$symb->setAttribute('type',$help_symbol[0]);
			break;
		case "nil" :
			$symb = $xml_out->createElement('arg2',$help_symbol[1]);
			$symb->setAttribute('type',$help_symbol[0]);
			break;
		case "GF" :
		case "LF" :
		case "TF" :
			$symb = $xml_out->createElement('arg2',$symb_to_process);
			$symb->setAttribute('type','var');
		break;		
	}
	$instruction->appendChild($var);
	$instruction->appendChild($symb);
	$program_start_xml->appendChild($instruction);
}
/*
 * Funkcia vytvori XML output pre instrukciu READ
 * @Param $instruction_to_process obsahuje OPCODE(READ) pre spracovanie 
 * @Param $var_to_process obsahuje nazov variable 
 * @Param $type_to_process obsahuje typ dat ktore sa maju nacitat 
 * @Param $instruction_order obsahuje poradie spracovavanej instrukcie
 */

function process_read($instruction_to_process ,$var_to_process,$type_to_process,$instruction_order)
{
	global $xml_out , $program_start_xml;

	$instruction = $xml_out->createElement('instruction');
	$instruction->setAttribute('order',$instruction_order);
	$instruction->setAttribute('opcode',strtoupper($instruction_to_process));
	$var = $xml_out->createElement('arg1',htmlspecialchars($var_to_process));
	$var->setAttribute('type','var');
	$instruction->appendChild($var);
	$type = $xml_out->createElement('arg2',$type_to_process);
	$type->setAttribute('type','type');
	$instruction->appendChild($type);
	$program_start_xml->appendChild($instruction);
}
/*
 * Funkcia vytvori XML output pre instrukcie s 3 argumentami a to variable a 2x symbol
 * @Param $instruction_to_process obsahuje OPCODE pre spracovanie 
 * @Param $var_to_process obsahuje nazov variable 
 * @Param $symb1_to_process obsahuje prvy spracovavany symbol
 * @Param $symb2_to_process obsahuje druhy spracovavany symbol
 * @Param $instruction_order obsahuje poradie spracovavanej instrukcie
*/

function process_one_var_two_symb($instruction_to_process ,$var_to_process,$symb1_to_process,$symb2_to_process,$instruction_order)
{
	global $xml_out , $program_start_xml;

	$instruction = $xml_out->createElement('instruction');
	$instruction->setAttribute('order',$instruction_order);
	$instruction->setAttribute('opcode',strtoupper($instruction_to_process));

	$var = $xml_out->createElement('arg1',htmlspecialchars($var_to_process));
	$var->setAttribute('type','var');
	$instruction->appendChild($var);

	$help_symbol1 = $symb1_to_process;
	$help_symbol1 = explode('@',$symb1_to_process);
	$help_string = explode('string@',$symb1_to_process);	

	// switch pre vytvorenie spravneho XML atributu pre prvy symbol
	switch ($help_symbol1[0])
	{
		case "int":
			$symb = $xml_out->createElement('arg2',$help_symbol1[1]);
			$symb->setAttribute('type',$help_symbol1[0]);

			break;
		case "string" :
			$symb = $xml_out->createElement('arg2',htmlspecialchars($help_string[1]));
			$symb->setAttribute('type',$help_symbol1[0]);
			break;
		case "bool"	:
			$symb = $xml_out->createElement('arg2',$help_symbol1[1]);
			$symb->setAttribute('type',$help_symbol1[0]);
			break;
		case "nil" :
			$symb = $xml_out->createElement('arg2',$help_symbol1[1]);
			$symb->setAttribute('type',$help_symbol1[0]);
			break;
		case "GF" :
		case "LF" :
		case "TF" :
			$symb = $xml_out->createElement('arg2',$symb1_to_process);
			$symb->setAttribute('type','var');
		break;		
	}

	$help_symbol2 = $symb1_to_process;
	$help_symbol2 = explode('@',$symb2_to_process);
	$help_string2 = explode('string@',$symb2_to_process);	
	// switch pre vytvorenie spravneho XML atributu pre druhy symbol
	switch ($help_symbol2[0])
	{
		case "int":
			$symb2 = $xml_out->createElement('arg3',$help_symbol2[1]);
			$symb2->setAttribute('type',$help_symbol2[0]);

			break;
		case "string" :
			$symb2 = $xml_out->createElement('arg3',htmlspecialchars($help_string2[1]));
			$symb2->setAttribute('type',$help_symbol2[0]);
			break;
		case "bool"	:
			$symb2 = $xml_out->createElement('arg3',$help_symbol2[1]);
			$symb2->setAttribute('type',$help_symbol2[0]);
			break;
		case "nil" :
			$symb2 = $xml_out->createElement('arg3',$help_symbol2[1]);
			$symb2->setAttribute('type',$help_symbol2[0]);
			break;
		case "GF" :
		case "LF" :
		case "TF" :
			$symb2 = $xml_out->createElement('arg3',$symb1_to_process);
			$symb2->setAttribute('type','var');
		break;		
	}
	$instruction->appendChild($var);
	$instruction->appendChild($symb);
	$instruction->appendChild($symb2);
	$program_start_xml->appendChild($instruction);
}
/*
 * Funkcia vytvori XML output pre instrukcie s 3 argumentami a to label a 2x symbol
 * @Param $instruction_to_process obsahuje OPCODE pre spracovanie 
 * @Param $label_to_process obsahuje nazov labelu
 * @Param $symb1_to_process obsahuje prvy spracovavany symbol
 * @Param $symb2_to_process obsahuje druhy spracovavany symbol
 * @Param $instruction_order obsahuje poradie spracovavanej instrukcie
*/
function process_label_two_symb($instruction_to_process, $label_to_process,$symb1_to_process, $symb2_to_process,$instruction_order)
{
	global $xml_out , $program_start_xml;
	
	$instruction = $xml_out->createElement('instruction');
	$instruction->setAttribute('order',$instruction_order);
	$instruction->setAttribute('opcode',strtoupper($instruction_to_process));

	$label = $xml_out->createElement('arg1',htmlspecialchars($label_to_process));
	$label->setAttribute('type','label');

	$help_symbol1 = $symb1_to_process;
	$help_symbol1 = explode('@',$symb1_to_process);
	$help_string = explode('string@',$symb1_to_process);	
	// switch pre vytvorenie spravneho XML atributu pre prvy symbol
	switch ($help_symbol1[0])
	{
		case "int":
			$symb = $xml_out->createElement('arg2',$help_symbol1[1]);
			$symb->setAttribute('type',$help_symbol1[0]);

			break;
		case "string" :
			$symb = $xml_out->createElement('arg2',htmlspecialchars($help_string[1]));
			$symb->setAttribute('type',$help_symbol1[0]);
			break;
		case "bool"	:
			$symb = $xml_out->createElement('arg2',$help_symbol1[1]);
			$symb->setAttribute('type',$help_symbol1[0]);
			break;
		case "nil" :
			$symb = $xml_out->createElement('arg2',$help_symbol1[1]);
			$symb->setAttribute('type',$help_symbol1[0]);
			break;
		case "GF" :
		case "LF" :
		case "TF" :
			$symb = $xml_out->createElement('arg2',$symb1_to_process);
			$symb->setAttribute('type','var');
		break;		
	}

	$help_symbol2 = $symb1_to_process;
	$help_symbol2 = explode('@',$symb2_to_process);
	$help_string2 = explode('string@',$symb2_to_process);	
	// switch pre vytvorenie spravneho XML atributu pre druhy symbol
	switch ($help_symbol2[0])
	{
		case "int":
			$symb2 = $xml_out->createElement('arg3',$help_symbol2[1]);
			$symb2->setAttribute('type',$help_symbol2[0]);

			break;
		case "string" :
			$symb2 = $xml_out->createElement('arg3',htmlspecialchars($help_string2[1]));
			$symb2->setAttribute('type',$help_symbol2[0]);
			break;
		case "bool"	:
			$symb2 = $xml_out->createElement('arg3',$help_symbol2[1]);
			$symb2->setAttribute('type',$help_symbol2[0]);
			break;
		case "nil" :
			$symb2 = $xml_out->createElement('arg3',$help_symbol2[1]);
			$symb2->setAttribute('type',$help_symbol2[0]);
			break;
		case "GF" :
		case "LF" :
		case "TF" :
			$symb2 = $xml_out->createElement('arg3',$symb1_to_process);
			$symb2->setAttribute('type','var');
		break;		
	}
	$instruction->appendChild($label);
	$instruction->appendChild($symb);
	$instruction->appendChild($symb2);
	$program_start_xml->appendChild($instruction);
}
/*
 * Funkcia pre kontrolu lexikalnej spravnosti spracovavanej variable 
 * @Param $var_to_check obsahuje nazov variable ktory kontrolujem pomocou regexu 
 */
function check_var($var_to_check)
{
	if (preg_match('/^(GF|LF|TF)@(_|-|\$|&|%|\*|!|\?|[a-z]|[A-Z])(_|-|\$|&|%|\*|!|\?|[a-z]|[A-Z]|[0-9])*$/', $var_to_check))
	{
		return true;
	}
	else
	{
		return false;
	}	
	
}
/*
 * Funkcia pre lexikalnu a syntakticku kontrolu symbolov
 * @Param $symb_to_check obsahuje symbol ktory kontrolujem pomocou regexu
 *
*/

function check_symb($symb_to_check)
{
	if(preg_match('/^(GF|LF|TF)@(_|-|\$|&|%|\*|!|\?|[a-z]|[A-Z])(_|-|\$|&|%|\*|!|\?|[a-z]|[A-Z]|[0-9])*$/', $symb_to_check))
	{
		return true;
	}
	else if (preg_match('/^((bool@)(true|false))$/', $symb_to_check))
	{
		return true;
	}
	else if (preg_match('/^int@([-\+]?[0-9]+$)/', $symb_to_check))
	{
		return true;
	}

	else if (preg_match('/^string@(([^\s\#\\\\]|\\\\[0-9]{3,3})*$)/', $symb_to_check))
	{
		return true;
	}
	else if (preg_match('/^nil@nil$/', $symb_to_check))
	{
		return true;
	}
	else
	{
		return false;
	}	
		
}

/*
 * Funkcia pre lexikalnu kontorlu nazvu labelu
 * @Param $label_to_check nazov labelu ktory kontrolujem pomocou regexu 
*/
function check_label($label_to_check)
{
	if(preg_match('/^(_|-|\$|&|%|\*|!|\?|[a-z]|[A-Z])(_|-|\$|&|%|\*|!|\?|[a-z]|[A-Z]|[0-9])*$/', $label_to_check))
	{	
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * Fukncia pre kontrolu typu pri instrukcii READ
 * @Param $type_to_check obsahuje typ ktory bol zadani pri instrukcii read 
 */
function check_type($type_to_check)
{
	switch ($type_to_check)
	{
		case "string":
		case "int":
		case "bool" :
			return true;
			break;
		default :
			return false;
			break;	
	}
}
/*
 * Funkcia spracuje error a vypise prislusnu chybovu hlasku 
 * @Param $error_code obsahuje cilenu hodnotu erroru (21,22,23,10,11)
*/
function process_error ($error_code)
{
	switch ($error_code)
	{
		case "21" :
			fputs(STDERR,"Chybna alebo chybajuca hlavicka v zdrojovom kode napisanom v IPPcode21\n");
			exit(21);
			break;
		case "22" :
			fputs(STDERR, "Neznamy alebo chybny operacny kod v zdrojovom kode napisanom v IPPcode21\n");
			exit(22);
			break;
		case "23" :
			fputs(STDERR, "Ina lexikalna alebo syntakticka chyba v zdrojovom kode napisanom v IPPcode21\n");
			exit(23);
			break;
		case "10" :
			fputs(STDERR , "Chybny alebo chybajuci parameter skriptu\n");
			exit(10);
			break;	
		case "11" :
			fputs(STDERR, "Chyba pri otvarani vstupneho suboru (neexistujuci subor , nedostatocne opravnenie , prazdny subor)\n");	
			exit(11);	
			break;
		default:
		break;	
	}
}