#!/usr/bin/env python3.8
import socket
import sys
import getopt
import re
import select
import os

def checkIP(NAMESERVER):
	IPcheck = re.compile("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}:[0-9]{1,5}$")	
	test = IPcheck.match(NAMESERVER)
	if test:
		IPok= True;
	else :
		sys.exit("Zly tvar IP adresy a portu")

def getIPandPort(NAMESERVER):
	NameIP = NAMESERVER.split(':')
	return NameIP[0],NameIP[1]


def getUDP(IP,PORT,ServerName):
	try:
		socketUDP = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	except Exception :
		sys.exit("Chyba pri vytvrani socketu")	
	try:	
		massage = "WHEREIS "+ServerName
		massage = massage.encode('utf-8')
		socketUDP.sendto(massage,(NameIP,int(NamePORT)))
		socketUDP.settimeout(30)
	except Exception:
		socketUDP.close()
		sys.exit("Chyba pri komunikacii so serverom")		

	try:	
		socketUDP.settimeout(30)
		resultUDP = socketUDP.recv(1028)
	except socket.error:
		socketUDP.close()
		sys.exit("Chyba pri komunikacii so serverom")	

	resultUDP = resultUDP.decode()
	socketUDP.close()
	return resultUDP

def checkSURL(SURLtoCheck):
	Protocol = SURLtoCheck.split(':')
	if Protocol[0] !="fsp" and Protocol[0] != "FSP" :
		sys.exit("Bol zadany nespravny vstup")
	
	ServerName = SURLtoCheck.split('/')
	ServerNameCheck=re.compile("^([a-z]|[A-Z]|[\-\_\.])+$")
	testName = ServerNameCheck.match(ServerName[2])
	if bool(testName) == False:
		sys.exit("Bol zadany nespravny vstup")
	
	if len(ServerName) < 4 :
		sys.exit("Bol zadany nespravny vstup")

	if len(ServerName[3]) == 0 :
		sys.exit("Bol zadany nespravny vstup")

	file = ""
	for i in range (3,len(ServerName)):
		file=file + ServerName[i]
		if(i < len(ServerName)-1):
			file = file + "/"
	return ServerName[2],file

def getTCP(IP,PORT,FileServerName,FileName):
	try:
		socketTCP=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	except Exception :
		sys.exit("Chyba pri vytvrani socketu")

	try:
		socketTCP.settimeout(30)
		socketTCP.connect((IP,int(PORT)))	
	except socket.error:
		sys.exit("Chyba pri komunikacii so serverom")

	msg = "GET " +FileName+ " FSP/1.0\r\n Agent: xkralo05\r\nHostname: "+FileServerName+"\r\n\r\n"
	msg = msg.encode('utf-8')
	socketTCP.sendall(msg)
	recived = ''
	recivedData = []
	while True:
		try:
			ready = select.select([socketTCP],[],[],5)
		except Exception:
			socketTCP.close()
			sys.exit("Chyba pri praci s file serverom")
			break
		if ready[0]:	
			recived = socketTCP.recv(8000)
		
			if not recived:
				break
		else:
			sys.exit("Chyba pri praci s file serverom")
			break
		recivedData.append(recived) 
	
	socketTCP.close()
	return recivedData

def WriteFile(TCPdata,NameOfFile,DataToWrite):
	f = open(NameOfFile,"wb")
	f.write(DataToWrite)
	f.close()

NAMESERVER = ''
SURL = ''
IPok = False;
if len(sys.argv) != 5:
	sys.exit("Zly pocet alebo tvar vstupnych parametrov")

if sys.argv[1] == '-n' or sys.argv[3] == '-n':
	if sys.argv[1] == '-n':
		NAMESERVER = sys.argv[2]
	else :
		NAMESERVER = sys.argv[4]	
else:
	sys.exit("Zly pocet alebo tvar vstupnych parametrov")

if sys.argv[3] == '-f' or sys.argv[1]== '-f':
	if sys.argv[3] == '-f':
		SURL = sys.argv[4]
	else : 
		SURL = sys.argv[2]
else:
	sys.exit("Zly pocet alebo tvar vstupnych parametrov")

FileServerName ,FileName = checkSURL(SURL);
IPcheck = re.compile("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}:[0-9]{1,5}$")	
test = IPcheck.match(NAMESERVER)
checkIP(NAMESERVER)
NameIP,NamePORT = getIPandPort(NAMESERVER)
resultUDP = getUDP(NameIP,NamePORT,FileServerName)
resultCheck = resultUDP.split(' ')

if resultCheck[0] == "OK":
	checkIP(resultCheck[1])
	fileServersplit= resultCheck[1].split(":")
	fileServerIP = fileServersplit[0]
	fileServerPORT = fileServersplit[1]

elif resultCheck[0] == "ERR":
	sys.exit(resultUDP)
else:
	sys.exit("Neocakavana odpoved od nameserver ")

FileNameSplit = FileName.split("/")
if FileNameSplit[len(FileNameSplit)-1] == "*":
	if len(FileNameSplit) == 1 :
		TCPdata = getTCP(fileServerIP,fileServerPORT,FileServerName,"index")
		TCPdataWork = b''.join(TCPdata)
		headers, sep, body = TCPdataWork.partition(b'\r\n\r\n')
		body = body.decode()
		bodySplit=body.split('\r\n')
		bodylength = len(bodySplit)
		
		
		for i in range(0,bodylength-1):
			cwd = os.getcwd()
			NameOfFileHelp = bodySplit[i]
			NameOfFileW = NameOfFileHelp.split('/')
		#	print(len(NameOfFileW))
			#print(NameOfFileW[len(NameOfFileW)-1])
			DirectoryP = '/'.join(NameOfFileW[:-1])
			cwd += '/'+DirectoryP 
			if(len(DirectoryP)) > 0 :
				os.makedirs(cwd, exist_ok=True)
				TCPdata = getTCP(fileServerIP,fileServerPORT,FileServerName,bodySplit[i])
				TCPdataWork = b''.join(TCPdata)
				headers, sep, body = TCPdataWork.partition(b'\r\n\r\n')
				search = 'Success'
				if bool(re.findall(r"\b" + search + r"\b", headers.decode())):
					filepath = os.path.join(cwd, NameOfFileW[len(NameOfFileW)-1])
					WriteFile(TCPdata,filepath ,body)	
					
				else :
					sys.exit("Nastala chyba pri praci s file serverom alebo dany subor neexistuje na file servery")

			else:
				TCPdata = getTCP(fileServerIP,fileServerPORT,FileServerName,bodySplit[i])
				TCPdataWork = b''.join(TCPdata)
				headers, sep, body = TCPdataWork.partition(b'\r\n\r\n')
				search = 'Success'
				if bool(re.findall(r"\b" + search + r"\b", headers.decode())):
					WriteFile(TCPdata,NameOfFileW[len(NameOfFileW)-1],body)	
				else :
					sys.exit("Nastala chyba pri praci s file serverom alebo dany subor neexistuje na file servery")
	else :
		TCPdata = getTCP(fileServerIP,fileServerPORT,FileServerName,"index")
		TCPdataWork = b''.join(TCPdata)
		headers, sep, body = TCPdataWork.partition(b'\r\n\r\n')
		body = body.decode()
		bodySplit=body.split('\r\n')
		bodylength = len(bodySplit)
		PathToDir = FileName.split("*")
		counter= 0
		for i in range(0,bodylength-1):
			if bool(re.findall(r"\b" + PathToDir[0] + r"\b", bodySplit[i])): 
				counter = counter + 1
				NameOfFileHelp = bodySplit[i]
				NameOfFileW = NameOfFileHelp.split('/')
				TCPdata = getTCP(fileServerIP,fileServerPORT,FileServerName,bodySplit[i])
				TCPdataWork = b''.join(TCPdata)
				headers, sep, body = TCPdataWork.partition(b'\r\n\r\n')
				search = 'Success'
				if bool(re.findall(r"\b" + search + r"\b", headers.decode())):
					WriteFile(TCPdata,NameOfFileW[len(NameOfFileW)-1],body)	
				else :
					sys.exit("Nastala chyba pri praci s file serverom alebo dany subor neexistuje na file servery")
			else:
				continue;
			
		if counter == 0:
			sys.exit("Dany priecinok sa nenachadza na servery")
else:
	TCPdata = getTCP(fileServerIP,fileServerPORT,FileServerName,FileName)
	TCPdataWork = b''.join(TCPdata)
	headers, sep, body = TCPdataWork.partition(b'\r\n\r\n')
	search = 'Success'
	if bool(re.findall(r"\b" + search + r"\b", headers.decode())):
		WriteFile(TCPdata,FileNameSplit[len(FileNameSplit)-1],body)	
	else :
		sys.exit("Nastala chyba pri praci s file serverom alebo dany subor neexistuje na file servery")









