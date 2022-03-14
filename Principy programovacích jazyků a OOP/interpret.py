import argparse
import sys
import xml.etree.ElementTree as elementTree
import re

""" Trieda sluziaca na vyvorenie objektu instruckie 
	@Param oppcode uchovava oppcode prave spracovavanej instrukcie
	@Param arguments uchovava argumenty funkcie 
	@Param order uchovava poranie instrukcie 
	@Param nofA uchovava pocet argumentov funkcie
 """
class Inst:
	def __init__(self,oppcode,arguments,order,nofA):
		self.name = oppcode
		self.arguments = arguments
		self.order = int(order)
		self.numberOfArgs = nofA

""" Trieda sluziaca na vytvorenie objektu argument
	@Param text uchovava hodnotu argumentu
	@Param typeA uchovava typ argumentu
	@Param order uchovava poradie argumentu v instrukcii
 """
class Argument:
	def __init__(self,text,typeA,order):
		self.text = text
		self.typeA = typeA
		self.order = int(order)


""" Funkcia sluzi na spracovanie argumentov skriptu pomocou ArgumentParser , vukcia vracia hodnotu parametrov input a source"""
def argumentParse():
	sourceF="sys.stdin"
	inputF="sys.stdin"
	
	if len(sys.argv)<2 or len(sys.argv)>3:
		sys.stderr.write("Zly pocet alebo kombinacia argumentov skriptu\n")
		sys.exit(10) 

	ap = argparse.ArgumentParser(add_help=False,allow_abbrev=False)
	ap.add_argument("--help",help="Vypise na standardny vystup napovedu skriptu a ukonci skript s navratovou hodnotou 0.", action='store_true')
	ap.add_argument("--source",help="Sluzi na zadanie vstupneho suboru s XML reprezentaciou zdrojoveho kodu.",metavar='FILE',type=str,required=False)
	ap.add_argument("--input",help="Sluzi na zadanie vstupneho suboru ktory obsahuje vstupy pre samotnu iterpretaciu zadaneho zdrojoveho kodu.",metavar='FILE',type=str,required=False)
	try:
		arguments =	ap.parse_args()	
	except SystemExit:
		sys.exit(10)	
	
	if arguments.help:
		if(len(sys.argv)==2):
			ap.print_help()
			sys.exit(0)
		else:
			sys.exit(10)

	if not (arguments.source or arguments):
		sys.stderr.write("Zly pocet alebo kombinacia argumentov skriptu\n")
		sys.exit(10)		

	if arguments.source:
		sourceF=arguments.source[0:]
		
	
	if arguments.input:
		inputF=arguments.input[0:]
	
	return sourceF,inputF

""" Funkcia sluzi na kontrolu vstupne xml suboru 
	@Param sourceFileName obsahuje nazov vstupneho xml suboru
"""
def xmlCheck(sourceFileName):
	
	if sourceFileName =="sys.stdin":
		try:
			fline = sys.stdin.readline().splitlines()
			xmlParser=elementTree.parse(sys.stdin)
			rootXml = xmlParser.getroot()
		except FileNotFoundError:
			sys.stderr.write("Chyba pri otvarani vstupneho suboru\n")
			sys.exit(11)
		except Exception:
			sys.stderr.write("Chybny XML format vo vstupnom subore\n")
			sys.exit(31)
	else:

		try:
			fline=open(sourceFileName).readline().rstrip().splitlines()
			xmlParser=elementTree.parse(sourceFileName)
			rootXml = xmlParser.getroot()
		except FileNotFoundError:
			sys.stderr.write("Chyba pri otvarani vstupneho suboru\n")
			sys.exit(11)
		except Exception:
			sys.stderr.write("Chybny XML format vo vstupnom subore\n")
			sys.exit(31)	
	
	if fline[0] != '<?xml version="1.0" encoding="UTF-8"?>':
		sys.stderr.write("Neocakavana struktura XML\n")
		sys.exit(32)

	if rootXml.tag != "program":
		sys.stderr.write("Neocakavana struktura XML\n")
		sys.exit(32)

	if rootXml.attrib["language"].upper() != "IPPCODE21":
		sys.stderr.write("Nespravny jazyk vstupneho suboru\n")
		sys.exit(32)
	
	for attribute in rootXml.attrib:
		if attribute not in ["language","description","name"]:
			sys.stderr.write("Nepovoleny atribut v XML\n")
			sys.exit(32)
	orderArr=[]			
	insAttributesArr = []
	for instruction in rootXml:
		if instruction.tag != "instruction":
			sys.stderr.write("Nepovoleny atribut v XML\n")
			sys.exit(32)
		for insAtributes in instruction.attrib:
			if insAtributes not in ["opcode","order"]:
				sys.stderr.write("Neznamy atribut instrukcie\n")
				sys.exit(32)
			insAttributesArr.append(insAtributes)	
		if len(insAttributesArr) <2 :
			sys.stderr.write("Chyba atribut order alebo opcode\n")
			sys.exit(32)

		if not 	instruction.attrib["order"].isnumeric():
			sys.stderr.write("Nespravna hodnota order v xml subore\n")
			sys.exit(32)


		if int(instruction.attrib["order"]) < 1:
			sys.stderr.write("Nespravna hodnota order v xml subore\n")
			sys.exit(32)
		
		orderArr.append(instruction.attrib["order"])
		
		numberOfArguments = 0		
		
		argOrderArr=[]
		for arguments in instruction:
			numberOfArguments +=1
			for argt in arguments.attrib:
				if argt != "type" :
					sys.stderr.write("Chyba v arguementoch instrukcie\n")
					sys.exit(32)
				if arguments.attrib["type"] not in ["int","string","bool","var","label","type","nil"]:
					sys.stderr.write("Neznamy typ argumentu\n")
					sys.exit(32)
				if arguments.tag == "arg4":
					sys.exit(32)

			argcheck = arguments.tag.split('g')
			
			if argcheck[0] == "ar" and argcheck[1].isnumeric():
				argOrderArr.append(int(argcheck[1]))
			else:
				sys.stderr.write("Nespravny tvar argumentu v XML subore\n")
				sys.exit(32)	


		if argOrderArr:					
			if argOrderArr[len(argOrderArr)-1] > len(argOrderArr):
				sys.stderr.write("Chyba poradia argumentov v\n")
				sys.exit(32)

		seenA =[]
		for number in argOrderArr:
			if number in seenA:
				sys.stderr.write("Duplicitny argument instrukcie\n")
				sys.exit(32)
			else:
				seenA.append(number)	

		if numberOfArguments > 3 :
			sys.stderr.write("Prilis mnoho argumentov instrukcie\n")
			sys.exit(32)	
		
	seenO=[]
	for number in orderArr:
		if number in seenO:
			sys.stderr.write("Duplicitne hodnoty v atribute order\n")
			sys.exit(32)
		else:
			seenO.append(number)	

	return rootXml

"""Funkcia sluzi na kontrolu argumentu typu var 
   @Param arguments obsahuje argument na kontrolu
"""
"""Regex prevzaty z parse.php (xkralo05) """
def checkVar(arguments):
	if arguments.text is None:
		sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
		sys.exit(32)

	if arguments.attrib["type"] != "var":
		sys.stderr.write("Semanticka chyba variable\n")
		sys.exit(52)

	result = re.match('^(GF|LF|TF)@(_|-|\$|&|%|\*|!|\?|[a-z]|[A-Z])(_|-|\$|&|%|\*|!|\?|[a-z]|[A-Z]|[0-9])*$',arguments.text)
	if not result:
		sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
		sys.exit(52)

"""Funkcia sluzi na kontrolu argumentu typu Label
   @Param arguments obsahuje argument na kontrolu
"""
"""Regex prevzaty z parse.php (xkralo05) """
def checkLabel(arguments):
	if arguments.text is None:
		sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
		sys.exit(32)

	if arguments.attrib["type"] != "label":
		sys.stderr.write("Semanticka chyba labelu\n")
		sys.exit(52)

	result = re.match('^(_|-|\$|&|%|\*|!|\?|[a-z]|[A-Z])(_|-|\$|&|%|\*|!|\?|[a-z]|[A-Z]|[0-9])*$',arguments.text)
	if not result:
		sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
		sys.exit(32)

"""Funkcia sluzi na kontrolu argumentu typu symb
   @Param arguments obsahuje argument na kontrolu
"""
def checkSymbol(arguments):
	if arguments.text is not None:
		
		if arguments.attrib["type"] == "var":	
			if re.match('^(GF|LF|TF)@(_|-|\$|&|%|\*|!|\?|[a-z]|[A-Z])(_|-|\$|&|%|\*|!|\?|[a-z]|[A-Z]|[0-9])*$',arguments.text):
				result = True
			else:
				sys.stderr.write("Semanticka chyba symbolu\n")
				sys.exit(54)	

		elif  re.match('^true|false$',arguments.text):	
			if arguments.attrib["type"] == "bool":
				result = True				
			else:
				sys.stderr.write("Semanticka chyba symbolu\n")
				sys.exit(53)	
		elif  re.match('^[-\+]?[0-9]+$',arguments.text) :
			if arguments.attrib["type"] == "int":
				result = True				
			else:
				sys.stderr.write("Semanticka chyba symbolu\n")
				sys.exit(53)
		elif  arguments.attrib["type"] == "string":
			if  re.match('^([^\s\#\\\\]|\\\\[0-9]{3,3})*$',arguments.text):
				result = True			
			else:
				sys.stderr.write("Semanticka chyba symbolu\n")
				sys.exit(53)

		elif  arguments.text == "nil" :
			if arguments.attrib["type"] == "nil":
				result = True
			else:
				sys.stderr.write("Semanticka chyba symbolu\n")
				sys.exit(53)	
		else:
			result = False
	
	else:
		if arguments.attrib["type"] == "string":
			result = True			
		else:
			sys.stderr.write("Semanticka chyba symbolu\n")
			sys.exit(53)
	
	if result == False:
		sys.stderr.write("Semanticka chyba symbolu\n")
		sys.exit(55)

""" funkcia na kontrolu lexikalne a synataktickej spravnosti oppcode a dalej je z funkcie volana kontrola symbolu , variable a label  
	@Param rootXml obsahuje xml reprezentaciu kodu IPPcode21
"""
def checkOpcodeAndArgumens(rootXml):
	find = False
		
	for instruction in rootXml:
		arg =sorted(instruction,key=lambda child:child.tag)
		if instruction.attrib["opcode"].upper() in ["CREATEFRAME", "PUSHFRAME","POPFRAME","RETURN","BREAK"]:
			for arguments in instruction:
				if arguments != '':
					sys.stderr.write("Lexikalna alebo syntakticka chyba6\n")
					sys.exit(32)
			find = True
		
		elif instruction.attrib["opcode"].upper() in ["DEFVAR","POPS"]: 
			if len(list(instruction)) > 1 :
				sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
				sys.exit(32)

			if len(list(instruction)) < 1 :
				sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
				sys.exit(32)	

			for arguments in instruction:
				checkVar(arguments)	
			find = True
		
		elif instruction.attrib["opcode"].upper() in ["PUSHS","WRITE","EXIT","DPRINT"]:
			if len(list(instruction)) > 1 :
				sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
				sys.exit(32)

			if len(list(instruction)) < 1 :
				sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
				sys.exit(32)

			for arguments in instruction:
				checkSymbol(arguments)	
			find = True
		
		elif instruction.attrib["opcode"].upper() in ["JUMP","LABEL","CALL"]:
			if len(list(instruction)) > 1 :
				sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
				sys.exit(32)

			if len(list(instruction)) < 1 :
				sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
				sys.exit(32)	
			
			for arguments in instruction:
				checkLabel(arguments)	
			
			find = True
		
		elif instruction.attrib["opcode"].upper() in ["READ"]:
			if len(list(instruction)) > 2 or len(list(instruction)) <2:
				sys.stderr.write("Lexikalna alebo syntakticka chyba10\n")
				sys.exit(32)

			checkVar(arg[0])

			if arg[1].text in ["bool","int","string"]:
				pass
			else:
				sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
				sys.exit(32)

			find = True

		elif instruction.attrib["opcode"].upper() in ["TYPE","MOVE","INT2CHAR","STRLEN"]:
			if len(list(instruction)) > 2 :
				sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
				sys.exit(32)
			
			if len(list(instruction)) < 2 :
				sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
				sys.exit(32)
			
			checkVar(arg[0])
			checkSymbol(arg[1])
			
			find = True
		
		elif instruction.attrib["opcode"].upper() in ["ADD","SUB","IDIV","MUL","IMUL","LT","GT","EQ","AND","OR","STRI2INT","CONCAT","GETCHAR","SETCHAR"]:
			if len(list(instruction)) > 3 or len(list(instruction)) < 3:
				sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
				sys.exit(32)
			
			checkVar(arg[0])
			checkSymbol(arg[1])	
			checkSymbol(arg[2])
			
			find = True
		elif instruction.attrib["opcode"].upper() in ["NOT"]:
			if len(list(instruction)) > 2 or len(list(instruction)) < 2:
				sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
				sys.exit(32)

			checkVar(arg[0])
			checkSymbol(arg[1])		


		elif instruction.attrib["opcode"].upper() in ["JUMPIFNEQ","JUMPIFEQ"]:
			if len(list(instruction)) > 3 or len(list(instruction)) < 3 :
				sys.stderr.write("Lexikalna alebo syntakticka chyba\n")
				sys.exit(32)
				
			checkLabel(arg[0])
			checkSymbol(arg[1])
			checkSymbol(arg[2])

			find = True
		else :
			find = False	
			
		if not find :
			sys.stderr.write("Neznamy alebo nespravny opcode\n")
			sys.exit(32)		

""" Funkcia sluzi na prevedenie argumentu na integer 
	@Param argument obsahuje argument ktory je potrebne zmenit na integer (v pripade aritmetickych a pod.)
"""
def convertToInt(argument):
	try: 
		int(argument)
		return argument
	except ValueError:
		sys.stderr.write("Semanticka chyba pri praci so symbolom int\n")
		sys.exit(53)

""" Funkcia sluzi na vytvorenie instrukcneho listu z xml reprezentacie 
	@Param rootXml obsahuje xml reprezentaciu codi IPPcode21

	funkcia vracia list instrukcii.
"""

def makeInsList(rootXml):
	insList = []
	rootXml[:] = sorted(rootXml,key=lambda child:int(child.attrib["order"]))
	instructionOrder = 0
	for instruction in rootXml:
		instructionOrder += 1
		argumentsList = []
		arguments = sorted(instruction,key=lambda child:child.tag)
		numberOfArguments = 0
		argOrder = 0
		for i in range(0,len(arguments)):
			argOrder += 1
			numberOfArguments +=1
			argument = Argument(arguments[i].text,arguments[i].attrib["type"], argOrder)
			if argument.typeA == "string":
				argument.text= re.sub(r"\\(\d\d\d)",lambda x: chr(int(x.group(1), 10)), str(argument.text))
			argumentsList.append(argument)
		
		instructionA = Inst(instruction.attrib["opcode"],argumentsList,instructionOrder,numberOfArguments)
		insList.append(instructionA)
	
	return insList

""" Funkcia skontroluje semanticku spravnost Labelu
	@Param Instructionlist obsahuje list instrukcii 

	funckia vracia pole labelov ktore je potrebne pri funkciach ako je Jump a pod.
"""
def checkSemOfLabelandVariable(InstructionList):
	labelRedef=[]
	labelDict = {}
	labelList = []
	variableList=[]
	for instructionA in InstructionList:	
		if instructionA.name == "LABEL":
			if instructionA.arguments[0].text in labelRedef:
				sys.stderr.write("Semanticka chyba redefinicia Labelu\n")
				sys.exit(52)
			else:
				labelRedef.append(instructionA.arguments[0].text)
				labelDict[instructionA.arguments[0].text] =  instructionA.order	
	labelList.append(labelDict)
	return labelList		
""" Funkcia skontroluje ci existuje lokalny ramec a ci sa dana variable na ramci uz nachadza alebo nie  
	@Param LF obsahuje lokalny ramec
	@Param name obsahuje nazov premennej
"""

def checkFrameExistLF(LF,name):
	if len(LF) == 0:
		sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou \n")
		sys.exit(55)
	else:
		if name not in LF[len(LF)-1].keys():
			sys.stderr.write("Pristup k neexistujucej premennej \n")
			sys.exit(54)

""" Funkcia skontroluje ci existuje docasny ramec  a ci sa dana variable na ramci uz nachadza alebo nie  
	@Param TF obsahuje docasny ramec
	@Param name obsahuje nazov premennej
"""			
def checkFrameExistTF(TF,name):
	if TF == None:
		sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou \n")
		sys.exit(55)
	else:
		if name not in TF.keys():
			sys.stderr.write("Pristup k neexistujucej premennej \n")
			sys.exit(54)

""" Funkcia skontroluje  ci sa dana variable na globalnom ramci uz nachadza alebo nie  
	@Param GF obsahuje globalny ramec
	@Param name obsahuje nazov premennej
"""	
def checkFrameExistGF(GF,name):
	if name not in GF.keys():
		sys.stderr.write("Pristup k neexistujucej premennej\n")
		sys.exit(54)

""" Funckia skontroluje ci su dane 2 argumenty rovnakeho typu 
	@Param argument1 obsahuje prvy argument
	@Param argument2 obsahuje druhy argument
"""
def sameTypeCheck(argument1,argument2):
	if argument1 not in ("bool","int","string","nil","var") and argument2 not in ("bool","int","string","nil","var"):
		sys.stderr.write("Semanticka chyba zly typ operandu\n")
		sys.exit(53)
	else:
		if argument1 == "var" or argument2 == "var":
			pass
		elif argument1 != argument2:
			sys.stderr.write("Semanticka chyba zly typ operandu\n")
			sys.exit(53)	
""" Funkcia sluzi na ziskanie hodnoty variable ulozenej na jednom z ramcov 
	@Param GF obsahuje globalny ramec
	@Param TF obsahuje docasny ramec
	@Param LF obsahuje lokalny ramec
	@Param argument obsahuje argument z ktoreho ziskavam potrebne data ako je nazov premennej , jej hodnota a pod .

	funkcia vracia hodnotu premennej
"""
def getValueFromVarAndArg(GF,LF,TF,argument):
	if argument.typeA == "var":
		frameA,nameA= argument.text.split('@')
		if frameA == "GF":
			if nameA not in GF.keys():
				sys.stderr.write("Pristup k neexistujucej premennej \n")
				sys.exit(54)
			else:
				value = GF[nameA]	

		elif frameA == "LF":
			if len(LF) == 0:
				sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou\n")
				sys.exit(55)
			if nameA  not in LF[len(LF)-1].keys():
				sys.stderr.write("Pristup k neexistujucej premennej\n")
				sys.exit(54)
			else:
				value = LF[len(LF)-1][nameA]
		elif frameA == "TF":
			if TF == None :
				sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou\n")
				sys.exit(55)
			if nameA  not in TF.keys():
				sys.stderr.write("Pristup k neexistujucej premennej\n")
				sys.exit(54)
			else:	
				value = TF[nameA]
	else:
		value = argument.text	

	if value == None :
		if argument.typeA  in ["var","int","bool","nil"]:
			sys.stderr.write("Chybajuca hodnota premennej\n")
			sys.exit(56)			

	return value

""" Funkcia sluzi na zistenie typu hodnoty ulozenej vo variable
	@Param obsahuje hodnotu ktorej treba priradit typ
"""
def checkVarType(variable):
	if variable == None:
		result = "None"
	elif  re.match('^true|false$',variable):	
		result = "bool"				
	elif  re.match('^[-\+]?[0-9]+$',variable) :
		result = "int"
	elif  variable == "nil" :
		result = "nil"	
	elif  re.match('\w|\s',variable):
		result = "string"
	else :
		result = "nil"			
	
	return result

""" Funkcia sluzi na interpretovanie kodu napisaneho v jazyku IPPcode21
	Funkcia prechadza listom instrukcii podla atributu order a interpretuje jednotlive prikazy 
	V pripade ze sa ma skocit na label tak sa nahra hodnota atributu order z labelu do instructionOrderIndex ktory sa posunie na dany index v liste instrukcii.
	@Param instructionList obsahuje list instrukcii na spracovanie 
	@Param listOfLabels obsahuje list labelov ktore su potrebne pri skokoch
"""
def interpretCode(instructionList,listOfLabels):
	instructionOrderIndex = 0
	knownLabels=listOfLabels[0]
	GF={}
	LF=[]
	callStack=[]
	TF = None
	DATASTACK=[]
	currentIstrIndex = 1
	while instructionOrderIndex < len(instructionList):
		currentIstr = instructionList[instructionOrderIndex]
		instructionOrderIndex +=1
		currentIstr.name = currentIstr.name.upper() 
		if currentIstr.name == "CREATEFRAME":
			TF = {}		
		
		elif currentIstr.name =="PUSHFRAME":
			if TF == None :
				sys.stderr.write("Chyba pri praci s ramcom\n")
				sys.exit(55)
			else:
				LF.append(TF)
				TF = None
		elif currentIstr.name =="POPFRAME":
			if len(LF) == 0:
				sys.stderr.write("Chyba pri praci s ramcom\n")
				sys.exit(55)
			else:
				TF = LF.pop()

		elif currentIstr.name =="RETURN":
			if len(callStack)== 0:
				sys.stderr.write("Chybna hodnota na zasobniku volani: ")
				sys.exit(56)

			instructionOrderIndex = callStack.pop()	

			
	
		elif currentIstr.name =="BREAK":
			sys.stderr.write("Pozicia instrukcie v kode :")
			sys.stderr.write(str(instructionOrderIndex)+"\n")
			sys.stderr.write("Obsah globalneho ramca ulozeneho ako dict: ")
			sys.stderr.write(str(GF)+"\n")
			sys.stderr.write("Obsah lokalneho ramca ulozeneho ako list: ")
			sys.stderr.write(str(LF)+"\n")
			sys.stderr.write("Obsah docasneho ramca ulozeneho ako dict: ")
			sys.stderr.write(str(TF)+"\n")
		elif currentIstr.name =="DEFVAR":
			frame,name = currentIstr.arguments[0].text.split('@')
			if frame == "GF":
				if name not in GF.keys():
					GF[name] = None 
				else:
					sys.stderr.write("Semanticka chyba redefinicia premennej : ")
					sys.exit(52)

			elif frame == "LF":
				if len(LF) == 0:
					sys.stderr.write("Chyba pri praci s ramcom pri vytvarani premennej\n")
					sys.exit(55)
				else:
					LF[len(LF)-1][name] = None

			elif frame == "TF":
				if TF == None :
					sys.stderr.write("Chyba pri praci s ramcom pri vytvarani premennej\n")
					sys.exit(55)
				else:
					if name not in TF.keys():
						TF[name] = None
					else:
						sys.stderr.write("Semanticka chyba redefinicia premennej\n")
						sys.exit(52)
			
		elif currentIstr.name =="POPS":
			
			frame,name = currentIstr.arguments[0].text.split('@')
			if len(DATASTACK) == 0:
				sys.stderr.write("Chyba pri praci so zasobnikom\n")
				sys.exit(56)

			result= DATASTACK.pop()

			if frame == "GF":
				checkFrameExistGF(GF,name)
				GF[name]=result
				
			elif frame == "LF":
				checkFrameExistLF(LF,name)
				LF[len(LF)-1][name] = result
				
			elif frame ==  "TF":
				checkFrameExistTF(TF,name)
				TF[name]=result
		
		elif currentIstr.name =="PUSHS":
			value0 = getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[0])
			DATASTACK.append(value0)
			
			
		elif currentIstr.name =="WRITE":
			value0 = getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[0])
			value0 = str(value0)
			val1type = checkVarType(str(value0))
			
			if val1type == "nil":
				print('')	

			elif val1type == "bool":
				if value0 == "true":
					print("true",end='',flush=True)
				else:
					print("false",end='',flush=True)
			else:
				value0=  re.sub(r"\\(\d\d\d)",lambda x: chr(int(x.group(1), 10)), value0)
				print(value0,end='',flush=True)
		
		elif currentIstr.name =="EXIT":
			value0 = getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[0])
			val1type = checkVarType(str(value0))
			if val1type != "int":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii EXIT\n")
				sys.exit(53)

			if int(value0) >= 0 and int(value0) <= 49:
				sys.exit(int(value0))

			else:
				sys.stderr.write("Chyba interpretacie neplatny navratovy kod instrukcie EXIT\n")
				sys.exit(57)
				
		elif currentIstr.name =="DPRINT":
			value0 = getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[0])
			sys.stderr.write(value0)
			
		elif currentIstr.name =="JUMP":
			labelName = currentIstr.arguments[0].text
			if labelName not in knownLabels:
				sys.stderr.write("Skok na nedefinovane navestie\n")
				sys.exit(52)
			else:
				instructionOrderIndex = knownLabels[labelName]
	
		elif currentIstr.name =="LABEL":
			pass
			
		elif currentIstr.name =="CALL":
			
			labelName = currentIstr.arguments[0].text
			if labelName not in knownLabels:
				sys.stderr.write("Skok na nedefinovane navestie\n")
				sys.exit(52)
			else:
				callStack.append(int(instructionOrderIndex))
				
				instructionOrderIndex = knownLabels[labelName]
				
			
		elif currentIstr.name =="READ":
			pass;
			
		elif currentIstr.name =="TYPE":
			frame,name = currentIstr.arguments[0].text.split('@')
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			val1type = checkVarType(str(value1))

			if frame == "GF":
				checkFrameExistGF(GF,name)
				GF[name]=val1type 
				
			elif frame == "LF":
				checkFrameExistLF(LF,name)
				LF[len(LF)-1][name] = val1type 
				
			elif frame ==  "TF":
				checkFrameExistTF(TF,name)
				TF[name]=val1type 
	
		elif currentIstr.name =="MOVE":
			frame,name = currentIstr.arguments[0].text.split('@')
			sourceS = currentIstr.arguments[1].text
			if frame == "GF":
				if name not in GF.keys():
					sys.stderr.write("Pristup k neexistujucej premennej\n")
					sys.exit(54)
				else:
					if currentIstr.arguments[1].typeA == 'var':
						frameA,nameA = currentIstr.arguments[1].text.split('@')
						if frameA == "GF":
							if nameA not in GF.keys():
								sys.stderr.write("Pristup k neexistujucej premennej\n")
								sys.exit(54)
							else:
								GF[name] = GF[nameA]	

						elif frameA == "LF":
							if len(LF) == 0:
								sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou\n")
								sys.exit(55)
							if nameA  not in LF[len(LF)-1].keys():
								sys.stderr.write("Pristup k neexistujucej premennej\n")
								sys.exit(54)
							else:
								GF[name] = LF[len(LF)-1][nameA]
						elif frameA == "TF":
							if TF == None :
								sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou\n")
								sys.exit(55)
							if nameA  not in TF.keys():
								sys.stderr.write("Pristup k neexistujucej premennej\n")
								sys.exit(54)
							else:
								GF[name] = TF[nameA]
					else:
						GF[name] = currentIstr.arguments[1].text 								
			
			elif frame =="LF":
				if len(LF) == 0:
					sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou\n")
					sys.exit(55)
				else:
					if name not in LF[len(LF)-1].keys():
						sys.stderr.write("Pristup k neexistujucej premennej\n")
						sys.exit(54)
					else:
						if currentIstr.arguments[1].typeA == 'var':
							frameA,nameA = currentIstr.arguments[1].text.split('@')
							if frameA == "GF":
								if nameA not in GF.keys():
									sys.stderr.write("Pristup k neexistujucej premennej\n")
									sys.exit(54)
								else:
									LF[len(LF)-1][name] = GF[nameA]	

							elif frameA == "LF":
								if len(LF) == 0:
									sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou\n")
									sys.exit(55)
								if nameA  not in LF[len(LF)-1].keys():
									sys.stderr.write("Pristup k neexistujucej premennej\n")
									sys.exit(54)
								else:
									LF[len(LF)-1][name] = LF[len(LF)-1][nameA]	

							elif frameA == "TF":
								if TF == None :
									sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou\n")
									sys.exit(55)
								if nameA  not in TF.keys():
									sys.stderr.write("Pristup k neexistujucej premennej\n")
									sys.exit(54)
								else:
									LF[len(LF)-1][name] = TF[nameA]
						else:
							LF[len(LF)-1][name] = currentIstr.arguments[1].text
			
			elif frame == "TF":	
				if TF == None :
					sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou\n")
					sys.exit(55)
				else:
					if name not in TF.keys():
						sys.stderr.write("Pristup k neexistujucej premennej\n")
						sys.exit(54)
					else:
						if currentIstr.arguments[1].typeA == 'var':
							frameA,nameA = currentIstr.arguments[1].text.split('@')
							if frameA == "GF":
								if nameA not in GF.keys():
									sys.stderr.write("Pristup k neexistujucej premennej\n")
									sys.exit(54)
								else:
									TF[name] = GF[nameA]	

							elif frameA == "LF":
								if len(LF) == 0:
									sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou\n")
									sys.exit(55)

								if nameA  not in LF[len(LF)-1].keys():
									sys.stderr.write("Pristup k neexistujucej premennej\n")
									sys.exit(54)
								else:
									TF[name] = LF[len(LF)-1][nameA]

							elif frameA == "TF":
								if TF == None :
									sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou\n")
									sys.exit(55)
								if nameA  not in TF.keys():
									sys.stderr.write("Pristup k neexistujucej premennej\n")
									sys.exit(54)
								else:
									TF[name] = TF[nameA]	
						else:
							TF[name] = currentIstr.arguments[1].text

		elif currentIstr.name =="INT2CHAR":
			frame,name = currentIstr.arguments[0].text.split('@')
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			val1type = checkVarType(str(value1))
			
			if val1type != "int":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii INT2CHAR\n")
				sys.exit(53)
			else:
				try:
					result = chr(int(value1))
					
				except:
					sys.stderr.write("Neplatny znak v unicode v instrukcii INT2CHAR\n")
					sys.exit(58)
				if frame == "GF":
					checkFrameExistGF(GF,name)			
					GF[name]=result
				elif frame == "LF":
					checkFrameExistLF(LF,name)			
					LF[len(LF)-1][name] = result					
				elif frame ==  "TF":
					checkFrameExistTF(TF,name)
					TF[name]=result			
		
		elif currentIstr.name =="STRLEN":
			frame,name = currentIstr.arguments[0].text.split('@')
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			val1type = checkVarType(str(value1))
			if val1type != "string":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii STRLEN\n")
				sys.exit(53)
			else:
				result = str(len(bytes(value1, "utf-8").decode("unicode_escape")))
				if frame == "GF":
					checkFrameExistGF(GF,name)			
					GF[name]=result
				elif frame == "LF":
					checkFrameExistLF(LF,name)			
					LF[len(LF)-1][name] = result					
				elif frame ==  "TF":
					checkFrameExistTF(TF,name)
					TF[name]=result

		elif currentIstr.name =="ADD":
			frame,name = currentIstr.arguments[0].text.split('@')
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
			argSymbol1 =convertToInt(value1)
			argsybmol2 =convertToInt(value2)

			if frame == "GF":
				if name not in GF.keys():
					sys.stderr.write("Pristup k neexistujucej premennej\n")
					sys.exit(54)
				else:
					GF[name]=str(int(argSymbol1)+int(argsybmol2))
			
			elif frame == "LF":
				if len(LF) == 0:
					sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou\n")
					sys.exit(55)
				else:
					if name not in LF[len(LF)-1].keys():
						sys.stderr.write("Pristup k neexistujucej premennej\n")
						sys.exit(54)
					else:

						LF[len(LF)-1][name] =str( int(argSymbol1)+int(argsybmol2))
			
			elif frame =="TF":	
				if TF == None :
					sys.stderr.write("Chyba pri praci s ramcom pri praci s premennou\n")
					sys.exit(55)
				else:
					if name not in TF.keys():
						sys.stderr.write("Pristup k neexistujucej premennej\n")
						sys.exit(54)
					else:

						TF[name]=str(int(argSymbol1)+int(argsybmol2))
						
			
		elif currentIstr.name =="SUB":
			frame,name = currentIstr.arguments[0].text.split('@')
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
			argSymbol1 =convertToInt(value1)
			argsybmol2 =convertToInt(value2)
			if frame == "GF":
				checkFrameExistGF(GF,name)
				GF[name]=str(int(argSymbol1)-int(argsybmol2))			
			elif frame == "LF":
				checkFrameExistLF(LF,name)
				LF[len(LF)-1][name] = str(int(argSymbol1)-int(argsybmol2))								
			elif frame ==  "TF":
				checkFrameExistTF(TF,name)
				TF[name]=str(int(argSymbol1)-int(argsybmol2))
			
		elif currentIstr.name =="IDIV":
			frame,name = currentIstr.arguments[0].text.split('@')
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
			argSymbol1 =convertToInt(value1)
			argsybmol2 =convertToInt(value2)

			if(int(argsybmol2)) == 0:
				sys.stderr.write("Chyba interpretacie delenie 0\n")
				sys.exit(57)
			
			if frame == "GF":
				checkFrameExistGF(GF,name)
				GF[name]=str(int(argSymbol1)//int(argsybmol2))			
			elif frame == "LF":
				checkFrameExistLF(LF,name)			
				LF[len(LF)-1][name] = str(int(argSymbol1)//int(argsybmol2))
								
			elif frame ==  "TF":
				checkFrameExistTF(TF,name)
				TF[name]=str(int(argSymbol1)//int(argsybmol2))		
		
		elif currentIstr.name =="MUL":
			frame,name = currentIstr.arguments[0].text.split('@')
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])	
			argSymbol1 =convertToInt(value1)
			argsybmol2 =convertToInt(value2)
			if frame == "GF":
				checkFrameExistGF(GF,name)			
				GF[name]=str(int(argSymbol1)*int(argsybmol2))	
			elif frame == "LF":
				checkFrameExistLF(LF,name)			
				LF[len(LF)-1][name] = str(int(argSymbol1)*int(argsybmol2))						
			elif frame ==  "TF":
				checkFrameExistTF(TF,name)
				TF[name]=str(int(argSymbol1)*int(argsybmol2))

		elif currentIstr.name =="LT":
			frame,name = currentIstr.arguments[0].text.split('@')
			sameTypeCheck(currentIstr.arguments[1].typeA,currentIstr.arguments[2].typeA)
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])	
			val1type = checkVarType(str(value1))
			val2type = checkVarType(str(value2))

			if val1type == "None" :
				value1 = ""
			if val2type == "None":
				value2 = ""

			if val1type == "nil" or val2type == "nil":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii GT\n")
				sys.exit(53)
			
			if 	val1type == "int":		
				if int(value1) < int(value2):
					result = "true"
				else:
					result = "false"

			elif val1type == "string":
				if value1 < value2:
					result = "true"
				else: 
					result = "false"
			else:
				if 	value1 == "false" and value2 == "true":
					result = "true"
				else: 
					result = "false"	
							
			if frame == "GF":
				checkFrameExistGF(GF,name)
				GF[name]=result
				
			elif frame == "LF":
				checkFrameExistLF(LF,name)
				LF[len(LF)-1][name] = result
				
			elif frame ==  "TF":
				checkFrameExistTF(TF,name)
				TF[name]=result
				
			
		elif currentIstr.name =="GT":
			frame,name = currentIstr.arguments[0].text.split('@')
			sameTypeCheck(currentIstr.arguments[1].typeA,currentIstr.arguments[2].typeA)
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
			val1type = checkVarType(str(value1))
			val2type = checkVarType(str(value2))

			if val1type == "None" :
				value1 =""
			if val2type == "None":
				value2 = ""
			
			if val1type != val2type :
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii GT\n")
				sys.exit(53)
					
			if val1type == "nil" or val2type == "nil":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii GT\n")
				sys.exit(53)
			
			if 	val1type == "int":		
				if int(value1) > int(value2):
					result = "true"
				else:
					result = "false"

			elif val1type == "string":
				if value1 > value2:
					result = "true"
				else: 
					result = "false"
			else:
				if 	value1 == "true" and value2 == "false":
					result = "true"
				else: 
					result = "false"	
							
			if frame == "GF":
				checkFrameExistGF(GF,name)
				GF[name]=result
				
			elif frame == "LF":
				checkFrameExistLF(LF,name)
				LF[len(LF)-1][name] = result
				
			elif frame ==  "TF":
				checkFrameExistTF(TF,name)
				TF[name]=result
			
		elif currentIstr.name =="EQ":
			frame,name = currentIstr.arguments[0].text.split('@')
			sameTypeCheck(currentIstr.arguments[1].typeA,currentIstr.arguments[2].typeA)
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
			val1type = checkVarType(str(value1))
			val2type = checkVarType(str(value2))

			if val1type == "None" :
				value1 =""
			if val2type == "None":
				value2 = ""

			if val1type != val2type :
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii EQ\n")
				sys.exit(53)
			else:
				if value1 == value2:
					result = "true"
				else:
					result = "false"	
								
				if frame == "GF":
					checkFrameExistGF(GF,name)
					GF[name]=result
				
				elif frame == "LF":
					checkFrameExistLF(LF,name)
					LF[len(LF)-1][name] = result
				
				elif frame ==  "TF":
					checkFrameExistTF(TF,name)
					TF[name]=result

		
		elif currentIstr.name =="AND":
			frame,name = currentIstr.arguments[0].text.split('@')
			sameTypeCheck(currentIstr.arguments[1].typeA,currentIstr.arguments[2].typeA)
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
			if value1 not in ["true","false"]:
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii AND\n")
				sys.exit(53)

			if value2 not in ["true","false"]:
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii AND\n")
				sys.exit(53)

			if value1 == "false" and value2 == "true":
				result = "false"
			else:
				result = value1 and value2		
			
			if frame == "GF":
				checkFrameExistGF(GF,name)
				GF[name]= result	
			elif frame == "LF":
				checkFrameExistLF(LF,name)
				LF[len(LF)-1][name] = result

			elif frame == "TF":		
				checkFrameExistTF(TF,name)
				TF[name]=result 

		elif currentIstr.name =="OR":
			frame,name = currentIstr.arguments[0].text.split('@')
			sameTypeCheck(currentIstr.arguments[1].typeA,currentIstr.arguments[2].typeA)
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
			
			
			if value1 not in ["true","false"]:
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii OR\n")
				sys.exit(53)

			if value2 not in ["true","false"]:
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii OR\n")
				sys.exit(53)
			if value1 == "false" and value2 == "false":
				result = "false"
			else:
				result = "true"			

			if frame == "GF":
				checkFrameExistGF(GF,name)
				GF[name]= result	
			elif frame == "LF":
				checkFrameExistLF(LF,name)
				LF[len(LF)-1][name] = result

			elif frame == "TF":		
				checkFrameExistTF(TF,name)
				TF[name]=result		

		elif currentIstr.name =="NOT":
			frame,name = currentIstr.arguments[0].text.split('@')
			value = getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			if value not in ["true","false"]:
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii NOT\n")
				sys.exit(53)
			else:
				if value == "true":
					result = "false"
				else:
					result = "true"			
			
			if frame == "GF":
				checkFrameExistGF(GF,name)
				GF[name]= result	
			elif frame == "LF":
				checkFrameExistLF(LF,name)
				LF[len(LF)-1][name] = result
			elif frame == "TF":		
				checkFrameExistTF(TF,name)
				TF[name]= result			
		
		elif currentIstr.name =="STRI2INT":
			frame,name = currentIstr.arguments[0].text.split('@')
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
			val1type = checkVarType(str(value1))
			val2type = checkVarType(str(value2))

			if val1type != "string":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii STRI2INT\n")
				sys.exit(53)

			if val2type != "int":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii STRI2INT\n")
				sys.exit(53)
			
			try:
				result = ord(value1[int(value2)])
			except:
				sys.stderr.write("Zly znak alebo index mimo moznych medzi pre instrukciu STRI2INT\n")
				sys.exit(58)

			if frame == "GF":
				checkFrameExistGF(GF,name)			
				GF[name]=result
			elif frame == "LF":
				checkFrameExistLF(LF,name)			
				LF[len(LF)-1][name] = result					
			elif frame ==  "TF":
				checkFrameExistTF(TF,name)
				TF[name]=result

		elif currentIstr.name =="CONCAT":
			frame,name = currentIstr.arguments[0].text.split('@')
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
			val1type = checkVarType(str(value1))
			val2type = checkVarType(str(value2))
			if val1type != "string" and val2type != "None":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii CONCAT\n")
				sys.exit(53)

			if val2type != "string" and val2type != "None":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii CONCAT\n")
				sys.exit(53)
			
			if val1type == "None" and val2type == "None":
				result =''
			elif val1type == "None":
				value2 = str(bytes(value2, "utf-8").decode("unicode_escape"))
				result = value2

			elif val2type == "None":
				value1 = str(bytes(value1, "utf-8").decode("unicode_escape"))
				result = value1
			else:

				result = value1 + value2
		
			
			if frame == "GF":
				checkFrameExistGF(GF,name)			
				GF[name]=result
			elif frame == "LF":
				checkFrameExistLF(LF,name)			
				LF[len(LF)-1][name] = result					
			elif frame ==  "TF":
				checkFrameExistTF(TF,name)
				TF[name]=result

		elif currentIstr.name =="GETCHAR":
			frame,name = currentIstr.arguments[0].text.split('@')
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
			val1type = checkVarType(str(value1))
			val2type = checkVarType(str(value2))

			if val1type != "string":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii GETCHAR\n")
				sys.exit(53)

			if val2type != "int":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii GETCHAR\n")
				sys.exit(53)
					
			if int(value2) < 0:
				sys.stderr.write("Chyba pri praci z retazcom pri instrukcii GETCHAR\n")
				sys.exit(58)
						
			try :
				result = value1[int(value2)]
			except:
				sys.stderr.write("Chyba pri praci z retazcom pri instrukcii GETCHAR\n")
				sys.exit(58)
			

			if frame == "GF":
				checkFrameExistGF(GF,name)			
				GF[name]=result
			elif frame == "LF":
				checkFrameExistLF(LF,name)			
				LF[len(LF)-1][name] = result					
			elif frame ==  "TF":
				checkFrameExistTF(TF,name)
				TF[name]=result	
		elif currentIstr.name =="SETCHAR":
			frame,name = currentIstr.arguments[0].text.split('@')
			value0=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[0])
			value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
			value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
			val1type = checkVarType(str(value1))
			val2type = checkVarType(str(value2))

			if val1type != "int":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii SETCHAR\n")
				sys.exit(53)

			if val2type != "string":
				sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii SETCHAR\n")
				sys.exit(53)

			if value2 == "" or int(value1) > len(value0)-1:
				sys.stderr.write("Chyba pri praci z retazcom pri instrukcii SETCHAR\n")
				sys.exit(58)
			
			try:
				value0 = list(value0)
				value0[int(value1)] = value2[0]
				value0 ="".join(value0)
				result = value0
			except:
				sys.stderr.write("Chyba pri praci z retazcom pri instrukcii SETCHAR\n")
				sys.exit(58)	

			if frame == "GF":
				checkFrameExistGF(GF,name)			
				GF[name]=result
			elif frame == "LF":
				checkFrameExistLF(LF,name)			
				LF[len(LF)-1][name] = result					
			elif frame ==  "TF":
				checkFrameExistTF(TF,name)
				TF[name]=result	

		elif currentIstr.name =="JUMPIFEQ":
			labelName = currentIstr.arguments[0].text
			if labelName not in knownLabels:
				sys.stderr.write("Skok na nedefinovane navestie\n")
				sys.exit(52)
			else:
				value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
				value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
				val1type = checkVarType(str(value1))
				val2type = checkVarType(str(value2))
				if val1type != val2type:
					sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii JUMPIFEQ\n")
					sys.exit(53)

				if value1 == value2:
					instructionOrderIndex = knownLabels[labelName]
		elif currentIstr.name =="JUMPIFNEQ":
			labelName = currentIstr.arguments[0].text
			if labelName not in knownLabels:
				sys.stderr.write("Skok na nedefinovane navestie\n")
				sys.exit(52)
			else:
				value1=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[1])
				value2=getValueFromVarAndArg(GF,LF,TF,currentIstr.arguments[2])
				val1type = checkVarType(str(value1))
				val2type = checkVarType(str(value2))
				if val1type != val2type:
					sys.stderr.write("Semanticka chyba zly typ operandu v instrukcii JUMPIFEQ\n")
					sys.exit(53)

				if value1 != value2:
					instructionOrderIndex = knownLabels[labelName]
																		
def main():
	sourceFileName,inputFileName = argumentParse()
	checkedXml = xmlCheck(sourceFileName)
	checkOpcodeAndArgumens(checkedXml)
	instructionList = makeInsList(checkedXml)

	listOfLabels = checkSemOfLabelandVariable(instructionList)
	interpretCode(instructionList,listOfLabels)



if __name__== "__main__":
	main()