/*
	VUT FIT ISA 2021 : Implementacia TFTP clienta
	Vypracoval : Kristan Kralovic (xkralo05)
*/
#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <boost/algorithm/string.hpp>
#include "mytftpclient.hpp"
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <netdb.h>
#include <stdio.h>
#include <sys/time.h>
#include <ctime>
#include <sys/ioctl.h>
#include <net/if.h>
using namespace std;


/*
	funkcia sluzi na rozdelenie stringu na casti a ulozenie jednotlivych casti do vektora
	string mystr - string ktory sa ma rozdelit
	string delimeter - string podla ktoreho sa ma rozdelit mystr
	vecor v - do vektoru sa uklada vysledok rozdelenia.
	funkcia vrati vector stringov 

	funkcia bola prevzana z tutorialu : https://slaystudy.com/c-split-string-by-space-into-vector/
*/

void Split_arguments(std::string mystr, std::vector<std::string> &v ,std::string delimeter)
{
	string temp = "";
	for(long unsigned int i=0;i<mystr.length();++i){		
		if(mystr[i]==delimeter[0]){
			v.push_back(temp);
			temp = "";
		}
		else{
			temp.push_back(mystr[i]);
		}		
	}
	v.push_back(temp);	
}

/*
	Funkcia sluzi na ziskanie velkosti suboru pri tsize option
	Funkcia vracia velkost suboru 
	Funkcia bola prevzana z tutorialu : https://www.codespeedy.com/cpp-program-to-get-the-size-of-a-file/
*/
long long int Calculate_size(const char* filename)
{
  FILE* fp = fopen(filename, "r");
  if (fp == NULL) 
  {
     cerr<< "file doesnt exist \n";
     return -1;
  }

  fseek(fp, 0L, SEEK_END);
  long int ans= ftell(fp);
  fclose(fp);
  return ans;
}

/*
	funkcia sluzi na vykonanie timeout pri odosielani alebo prijmani dat 
	funkcia vracia vysledok timeoutu
	funkcia bola inspirovana tutorialom : https://beej.us/guide/bgnet/html/#cb78-6
	author : Brain "Beej Jorgensen" Hall
*/
//  - mierne upravena verzia 
int recvtimeout(int socket ,int timeout_sec)
{ 
	fd_set fds;
	int n;
	struct timeval tv;

	FD_ZERO(&fds);
	FD_SET(socket,&fds);

	tv.tv_sec = timeout_sec;
	tv.tv_usec = 0;

	n = select(socket+1,&fds,NULL,NULL,&tv);
	if (n == 0) return -2; // timeout  
	if (n == -1) return -1;// error

	return n; 
}

/*
	Funkcia sluzi na vypisanie  casoveho razitka a erroru
*/

void Print_error(ErrorEnum Err)
{
	time_t now = time(0);
  tm *ltm = localtime(&now);
  cerr<<"["<< 1900 + ltm->tm_year<<"-"<< 1 + ltm->tm_mon<<"-"<<ltm->tm_mday<<" " << ltm->tm_hour << ":" << ltm->tm_min<< ":" << ltm->tm_sec<<"] "; 

	switch(Err){ 
    case Err_Comb  : cerr << "Nespravna kombinacia vstupnych parametrov"<<std::endl; break;
    case Err_Param: cerr << "Bol zadany neznamy parameter programu"<<std::endl; break;
    case Err_ParamForm: cerr << "Bola zadana nespravna forma vstupneho parametra"<<std::endl; break;
    case ErrSock : cerr<< "Chyba pri vytvarani UDP socketu"<<std::endl; break;
    case Err_SendRRQ: cerr<<"Nastala chyba pri odosielani RRQ requestu"<<std::endl; break;
    case Err_Recv: cerr<<"Nastala chyba pri primani dat zo servera"<<std::endl; break; 
    case Err_SendACK : cerr << "Nastala chyba pri posielani ACK packetu"<<std::endl; break;
	}
}

/*
	Funkcia sluzi na vypisanie casoveho razitka a prislusnej informacie.
	Ak na miesto informacie prislo "" tak sa na stderr vypise casove razitko.
	string information - obsahuje informaciu ktoru ma funkcia vypisat

*/
void Print_information(std::string information)
{
	time_t now = time(0);
  tm *ltm = localtime(&now);
  if(information !="")
  {
  	cout<<"["<< 1900 + ltm->tm_year<<"-"<< 1 + ltm->tm_mon<<"-"<<ltm->tm_mday<<" " << ltm->tm_hour << ":" << ltm->tm_min<< ":" << ltm->tm_sec<<"] ";
  	cout<< information <<std::endl;
  }
  else
  {
  	cerr<<"["<< 1900 + ltm->tm_year<<"-"<< 1 + ltm->tm_mon<<"-"<<ltm->tm_mday<<" " << ltm->tm_hour << ":" << ltm->tm_min<< ":" << ltm->tm_sec<<"] ";
  }
}

/*
	Funkcia sluzi na vypisanie erroru ktory prisiel od serveru
	int err_number - cislo erroru
*/
void Print_tftp_error(int err_number){
	switch(err_number){
		case 0: cerr << "Not defined"<<std::endl; break;
		case 1:	cerr << "File not found"<<std::endl; break;
		case 2:	cerr << "Access Violation"<<std::endl; break;
		case 3:	cerr << "Disk full or Quota exceeded"<<std::endl; break;
		case 4:	cerr << "Illegal TFTP operation"<<std::endl; break;
		case 5:	cerr << "Unknown port number"<<std::endl; break;
		case 6:	cerr << "File exists"<<std::endl; break;
		case 7:	cerr << "No such user"<<std::endl; break;
	}
}
/*
	Funkcia sluzi na vypis informacie o stave prenosu suboru 
	int Data_block_number - obsahuje block number packetu
	int Data_length - velkost dat v jednom packete
	string number_of_data - velkost uz obdrzanych alebo odoslanych dat
	string transfer_type obsahuje informaciu ci sa jedna o ziskavanie dat zo servera alebo o odosielanie dat na server
*/
void Print_transfer_information(int Data_block_number,std::string number_of_data,OACKreturn OACKvalues, int Data_length,std::string transfer_type)
{
	if(OACKvalues.tsize != "")
	{
		if(stoi(OACKvalues.tsize)> (60*Data_length))
  	{
  		if(Data_block_number % 30 == 0)
  		{
  			Print_information(transfer_type+number_of_data+" z "+OACKvalues.tsize);
  		}
 	 }
 	 else if(stoi(OACKvalues.tsize)> (30*Data_length))
 	 {
  		if(Data_block_number % 10 == 0)
  		{
  			Print_information(transfer_type+number_of_data+" z "+OACKvalues.tsize);
  		}
   }
 	 else
  	{
 	 		if(Data_block_number % 2 == 0)
  		{
  			Print_information(transfer_type+number_of_data+" z "+OACKvalues.tsize);
  		}
  	}
	}
	
}
/*
	Funkcia sluzi na spracovanie option ack odpovede od serveru
	Funkcia vracia vyplnenu strukturu OACKreturn
*/

OACKreturn Handle_OACK(char* buffer, Parameters ClientParameter, int recv_count) 
{
	OACKreturn ACK_values ;
	char *helpbuffer = new char[recv_count];
	int control_number;
	memcpy(helpbuffer,buffer,recv_count);
		
	if(ClientParameter.tparam == true && ClientParameter.sparamb == false) // ak som odosielal iba timeout option+ tsize
	{
		if(std::string(&helpbuffer[2]) == "tsize") // ak v OACK prisiel prvy tsize option
		{
			ACK_values.tsize = &helpbuffer[8]; // ziskanie velkosti suboru 
			control_number = 8+strlen(ACK_values.tsize.c_str())+1;
			if(control_number < recv_count)
			{
				if(std::string(&helpbuffer[8+strlen(ACK_values.tsize.c_str())+1]) == "timeout")
				{
					ACK_values.timeout = &helpbuffer[8+strlen(ACK_values.tsize.c_str())+1+7+1];  // ziskanie odpovede na timeout option 
				}
				else
				{
					Print_information("");
					cerr<<"error :  V ACK packete prisiel option o ktory klient nepoziadal";
					exit(15);
				}
			}
			else
			{			
				Print_information("Server neprial timeout parameter, prenos pokracuje so zakladnym nastavenim servera");
				delete[] helpbuffer;
				return ACK_values;
			}

		}
		else if(std::string(&helpbuffer[2]) == "timeout")  // ak v OACK prisiel prvy timeout option
		{
			ACK_values.timeout = &helpbuffer[10]; // ziskanie odpovede na timeout option 
			if(std::string(&helpbuffer[10+strlen(ACK_values.timeout.c_str())+1]) == "tsize")
			{
				ACK_values.tsize = &helpbuffer[10+strlen(ACK_values.timeout.c_str())+1+5+1];  // ziskanie velkosti suboru 
			}
			else
			{
				Print_information("");
				cerr<<"error :  V ACK packete prisiel option o ktory klient nepoziadal";
				exit(15);
			}
		} 
		else // ak neprisla odpoved na timeout  option
		{
			Print_information("");
			cerr<<"error :  V ACK packete prisiel option o ktory klient nepoziadal";
			exit(15);
		}	
	}
	else if (ClientParameter.sparamb == true && ClientParameter.tparam == false) //  ak som odosielal iba blocksize option + tsize
	{
		if(std::string(&helpbuffer[2]) == "tsize") // ak v OACK prisiel prvy tsize option
		{
			ACK_values.tsize = &helpbuffer[8]; // ziskanie velkosti suboru 
			control_number = 8+strlen(ACK_values.tsize.c_str())+1;
			if(control_number < recv_count)
			{
				if(std::string(&helpbuffer[8+strlen(ACK_values.tsize.c_str())+1]) == "blksize")	
				{
					ACK_values.blocksize = &helpbuffer[8+strlen(ACK_values.tsize.c_str())+1+7+1]; // ziskanie hodnoty blockzise z OACK
				}	
				else
				{
					Print_information("");
					cerr<<"error :  V ACK packete prisiel option o ktory klient nepoziadal";
					exit(15);
				}
			}
			else // ak neprisla odpoved na blksize option
			{	
				Print_information("Server odmietol blocksize option");
				delete[] helpbuffer;
				return ACK_values ;
			}
		}
		else if(std::string(&helpbuffer[2]) == "blksize") // ak prisiel prvy blocksize option
		{
			ACK_values.blocksize = &helpbuffer[10];
			if(std::string(&helpbuffer[10+strlen(ACK_values.blocksize.c_str())+1]) == "tsize")
			{
				ACK_values.tsize = &helpbuffer[10+strlen(ACK_values.blocksize.c_str())+1+5+1]; // ziskanie velkosti suboru 
			}
			else 
			{
				Print_information("");
				cerr<<"error :  V ACK packete prisiel option o ktory klient nepoziadal";
				exit(15);
			}
		}
		else // ak neprisla odpoved na blksize option
		{
			Print_information("");
			cerr<<"error :  V ACK packete prisiel option o ktory klient nepoziadal";
			exit(15);
		}		
	}	
	else if (ClientParameter.tparam == true && ClientParameter.sparamb == true) // ak som odosielal blocksize option aj timeout option + tsize
	{	
		std::string prase_string = "";	
		int size = recv_count / sizeof(char);
		for(int i=0; i<size; i++)
		{
			prase_string = prase_string + helpbuffer[i];
		}

		std::size_t found_tsize =prase_string.find("tsize",0);
		std::size_t found_timeout = prase_string.find("timeout",0);
		std::size_t found_blksize = prase_string.find("blksize",0);
			
		if(found_timeout == std::string::npos) // ak sa nenasiel timeout option
		{
			Print_information("Server odmietol timeout option");
		}
		else
		{
			ACK_values.timeout = &helpbuffer[found_timeout+8]; 
		}

		if(found_tsize ==std::string::npos) // ak sa nenasiel tsize option
		{
			Print_information("Server odmietol tsize option");
		}
		else
		{
			ACK_values.tsize = &helpbuffer[found_tsize+6];
		}

		if(found_blksize ==std::string::npos) // ak sa nenasiel blocksize option
		{
			Print_information("Server odmietol blocksize option");
		}
		else
		{
			ACK_values.blocksize = &helpbuffer[found_blksize+8];
		}
	}
	else
	{
		ACK_values.tsize = &helpbuffer[8];
	}	
	
	if(ClientParameter.tparam == true) 
	{
		if(ACK_values.timeout != ClientParameter.tparamnumber) // ak prisla ina hodnota timeout o aku som ziadal 
		{
			if(ACK_values.timeout != "")  // ukonci prenos
			{
				Print_information("");
				cerr<<"Server odoslal rozdielnu hodnotu parametru timeout. Pripojenie sa ukoncilo.";
				exit(20);
			}
		}		
	}
	if(ClientParameter.sparamb == true)
	{
		if(ACK_values.blocksize != ClientParameter.sparam) // ak prisla ina hodnota blocksize option o aku som ziadal tak prenos pokracuje dalej s novou hodnotou
		{
			Print_information("Server navrhol novu hodnotu blockize option, prenos pokracuje s hodnoutou blocksize :"+ACK_values.blocksize);
		}
	}
	delete[] helpbuffer;
 	return ACK_values;	
}

/*
	Funkcia confertuje data ziskane od serveru z netascii do linux verzie 
	
	char* outbuffer - buffer do ktore sa zapisuju upravene data
	char* precv - z s datami od serveru
	int recv_count - pocet bytov obdrzanych od serveru

	int input_iterator - sluzi na posuvanie v buffery ktory obsahuje data ziskane od serveru
	int output_iterator - sluzi na posuvanie v buffery do ktore sa zapisuje upraveny obsah bufferu

*/
int  Convert_from_netascii(char* out_buffer, char*precv  ,int recv_count)
{
	int input_iterator = 0;
	int output_iterator = 0;
  while(input_iterator<(recv_count-4)) // -4 -> (OPCODE + block number)
  {
  	if(precv[input_iterator]=='\r')//CR
  	{
  		input_iterator++;
  		if(precv[input_iterator]=='\n')//CRLF
  		{
 				out_buffer[output_iterator]='\n';//LF
  			output_iterator++;
  			input_iterator++;
  			continue;
  		}
 		 	if(precv[input_iterator]=='\0')//CRNULL
 	 		{
  			out_buffer[output_iterator]='\r';//CR
  			output_iterator++;
  			input_iterator++;
 	 		}
		}	
  	if(precv[input_iterator]!='\r')
  	{
  		out_buffer[output_iterator]=precv[input_iterator];
  		input_iterator++;
  		output_iterator++;                 
 		}				
 }
 return output_iterator;

}

// Funkcia sluzi na vypisanie help v pripadne zadanie parametra -h
void PrintHelp()
{
	cout<<"Priklad spustenia : > -R/W -d adresar/soubor -t timeout -s velikost -a adresa,port -c mód -m"<<endl;
	cout<<"\n";
	cout <<"-R (povinny)	- Ak bol zadany parameter -R, bude sa jednat o citanie suboru zo serveru." <<endl;
	cout <<"-W (povinny)	- Ak bol zadany parameter -W bude sa jednat o zapis suboru na server."<<endl;
	cout <<"Pritomnost oboch paramterov (-R ,-W) v jedom prikaze je vylucena."<<endl;
	cout <<"-d adresar/subor (povinny)	- subor urcuje subor ktory bude klient prijmat/odosielat, adresar specifikuje absolutnu cestu kam/odkial sa ma subor preniest."<<endl;
	cout <<"-t timeout (nepovinny)	- timeout urcuje aka je hodnota timeoutu."<<endl;
	cout <<"-s velkost (nepovinny)	- velkost specifikuje maximalnu velkost bloku v nasobkoch oktetu."<<endl;
	cout <<"-m (nepovinny)	- klient si od serveru cez multicast option vyziada transfer dat pomocou multicast."<<endl;
	cout <<"-c mod (nepovinny)	- mod moze obsahovat iba hodnoty 'ascii' (alebo 'netascii') a 'binary' (alebo 'octet'), cim sa specifikuje mod prenosu dat ."<<endl;
	cout <<"-a adresa,port (nepovinny)	- adresa je ipv4 alebo ipv6 adresa rozhrania na ktorom bezi sluzba (defalutne sa uvazuje ipv4 localhost), port je port na ktorom server naslucha (defalutne port 69)."<<endl;
	cout<<"\n";
	cout <<"exit	- parameter sluzi na ukoncenie programu."<<endl;
}

/*
	Funkcia sluzi na vytvorenie requestu ktory je potom zaslany na server
	char* buffer - Do premennej sa postupne ulozia hotnoty opcode a options ktore budu odoslane na server
	Parameters ClientParameter - Obsahuje hodnoty nutne k vytvoreniu requestu
	const char* charFilename - Nazov suboru ktory chcem stiahnut/odoslat
	string file_size - Velkost suboru ktory chcem stiahnut/odoslat
	int request_length - Velkost vysledneho requestu.
	
	Funkcia vracia dlzku vytvoreneho packetu 

*/

int Create_request(char* buffer, Parameters ClientParameter, const char* charFilename, std::string file_size)
{
	// Nastavenie OPCODE
	if(ClientParameter.Rparam == true)// -R
	{
		buffer[0] = 0;
 		buffer[1] = 1;
	}
	else // -W
	{
		buffer[0] = 0;
 		buffer[1] = 2;
	}

	int request_length ;

	strcpy(&buffer[2],charFilename);//pridanie mena suboru do bufferu (buffer = OPCODE+filename)
	strcpy(&buffer[2+strlen(charFilename)+1],ClientParameter.cparam.c_str()); // pridanie modu prenosu do bufferu (buffer = OPCODE+filename+'\0'+mode)
	strcpy(&buffer[2+strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1],"tsize");// pridanie transfer size option do bufferu (buffer = OPCODE+filename+'\0'+mode+'\0'+tsize)
	strcpy(&buffer[2+strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1],file_size.c_str());// pridanie hodnoty trasfer size option (buffer = OPCODE+filename+'\0'+mode+'\0'+tsize+'\0'+velkos suboru)

	if(ClientParameter.tparam == true && ClientParameter.sparamb == false) // pridanie timeout option
	{
		strcpy(&buffer[2+strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1+strlen(file_size.c_str())+1],"timeout");// pridanie timeout option (buffer = OPCODE+filename+'\0'+mode+'\0'+tsize+'\0'+velkos suboru+'\0'+timeout)
		strcpy(&buffer[2+strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1+strlen(file_size.c_str())+1+7+1],ClientParameter.tparamnumber.c_str());// pridanie timeout value (buffer = OPCODE+filename+'\0'+mode+'\0'+tsize+'\0'+velkos suboru+'\0'+timeout+'\0'+hodnota timeout)
		request_length = (int)(2 + strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1+strlen(file_size.c_str())+1+7+1+strlen(ClientParameter.tparamnumber.c_str())+1);
		return request_length;
	}

	if(ClientParameter.tparam == false && ClientParameter.sparamb == true)// pridanie blocksize option 
	{
		strcpy(&buffer[2+strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1+strlen(file_size.c_str())+1],"blksize");// pridanie blocksize option (buffer = OPCODE+filename+'\0'+mode+'\0'+tsize+'\0'+velkos suboru+'\0'+blksize)
		strcpy(&buffer[2+strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1+strlen(file_size.c_str())+1+7+1],ClientParameter.sparam.c_str()); // pridanie blocksize option (buffer = OPCODE+filename+'\0'+mode+'\0'+tsize+'\0'+velkos suboru+'\0'+blksize+'\0'+hodnota blksize)
		request_length = (int)(2 + strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1+strlen(file_size.c_str())+1+7+1+strlen(ClientParameter.sparam.c_str())+1);
		return request_length;
	}

	if(ClientParameter.tparam == true && ClientParameter.sparamb == true) // pridanie timeout option + blocksize option
	{
		strcpy(&buffer[2+strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1+strlen(file_size.c_str())+1],"timeout"); // pridanie timeout option
		strcpy(&buffer[2+strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1+strlen(file_size.c_str())+1+7+1],ClientParameter.tparamnumber.c_str());// pridanie hodnoty timeout
		strcpy(&buffer[2+strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1+strlen(file_size.c_str())+1+7+1+strlen(ClientParameter.tparamnumber.c_str())+1],"blksize"); // pridanie block size option
		strcpy(&buffer[2+strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1+strlen(file_size.c_str())+1+7+1+strlen(ClientParameter.tparamnumber.c_str())+1+7+1],ClientParameter.sparam.c_str()); // pridanie hodnoty block size
		request_length = (int)(2 + strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1+strlen(file_size.c_str())+1+7+1+strlen(ClientParameter.tparamnumber.c_str())+1+7+1+strlen(ClientParameter.sparam.c_str())+1);
		return request_length;
	}

	request_length = (int)(2 + strlen(charFilename)+1+strlen(ClientParameter.cparam.c_str())+1+5+1+strlen(file_size.c_str())+1);
	return request_length;
}

/*
	Funkcia kontroluje a spracuvava vstupne parametre

	vector HelpVector - Sluzi na ukladanie jednotlivych parametrov 
	Parameters HelpParamStruct - Sluzi na nastavovanie hodnot p strukutry Parametes.
	premenna ParamR - Sluzi na urcenie ci bol nastaveny vstupny parameter programu  -R
	premenna ParamW - Sluzi na urcenie ci bol nastaveny vstupny parameter programu  -W
	premenna Parama - Sluzi na urcenie ci bol zadany vstupny parameter programu  -a
	premenna Paramd - Sluzi na urcenie ci bol zadany vstupny parameter programu  -d
	premenna Paramt - Sluzi na urcenie ci bol zadany vstupny parameter programu  -t
	premenna Params - Sluzi na urcenie ci bol zadany vstupny parameter programu  -s
	premenna Paramm - Sluzi na urcenie ci bol zadany vstupny parameter programu  -m
	premenna Paramc - Sluzi na urcenie ci bol zadany vstupny parameter programu : -c
	premenna Paramh - Sluzi na urcenie ci bol zadany vstupny parameter programu  -h

	Fukncia vrati vyplnenu strukturu Parameters.
*/
Parameters Check_arguments(std::vector<std::string> v){
	std::vector<string> HelpVector;
	Parameters HelpParamStruct;
	bool ParamR = false;
	bool ParamW = false; 
	bool Parama = false;
	bool Paramd = false;
	bool Paramt = false;
	bool Params = false;
	bool Paramm = false;
	bool Paramc = false;
	bool Paramh = false;
	for (long unsigned int  i=1; i<v.size();++i)
	{
		boost::trim_right(v[i]);
		std::string::size_type pos;
		
		// Odstranenie prebytocnych bielych znakov	
		while((pos = v[i].find("  ")) != std::string::npos)
		{
  		v[i].replace(pos, 2, " ");
		}
		// Spracovanie vstupnych parametrov
		if (v[i] == "R")
		{
			ParamR = true;
			HelpParamStruct.Rparam =true; 
		}
		else if (v[i] == "W")
		{
			ParamW = true;
			HelpParamStruct.Wparam =true; 
		}
		else if(v[i] == "h")
		{
			Paramh=true;		
		}	
		else
		{
			Split_arguments(v[i],HelpVector," ");
			if(v[i][0]=='a' && v[i][1]==' ' && Parama == false) // Spracovanie a kontrola vstupneho parametra -a 
			{
				if(HelpVector.size() > 4)
				{
					Print_error(Err_ParamForm);
					HelpParamStruct.ParamWrong = true;
					break;
				}
				else if (HelpVector.size() == 4 )
				{
					HelpParamStruct.aparamIP = HelpVector[1];
					HelpParamStruct.aparamPort = std::stoi(HelpVector[3]);
					HelpParamStruct.aparamPortString = HelpVector[3];
					if(HelpVector[2] != ",")
					{
						Print_error(Err_ParamForm);
						HelpParamStruct.ParamWrong = true;
						break;
					}
				}
				else if(HelpVector.size() == 2)
				{
					std::vector<string> HelpVectorIP; // pomocny vektor pre spracovanie IP
					Split_arguments(HelpVector[1],HelpVectorIP,",");
					if(HelpVectorIP.size() <2 )
					{
						Print_error(Err_ParamForm);
						HelpParamStruct.ParamWrong = true;
						break;
					}
					HelpParamStruct.aparamIP = HelpVectorIP[0];
					HelpParamStruct.aparamPort = std::stoi(HelpVectorIP[1]);
					HelpParamStruct.aparamPortString = HelpVectorIP[1];
				}
				else 
				{
					if(HelpVector[1].find(',') ==  string::npos && HelpVector[2].find(',')==  string::npos )	
					{
						Print_error(Err_ParamForm);
						HelpParamStruct.ParamWrong = true;
						break;
					}
					std::string::size_type pos;
					while((pos = HelpVector[1].find(",")) != std::string::npos)
					{
  					HelpVector[1].replace(pos, 2, "");
					}
					while((pos = HelpVector[2].find(",")) != std::string::npos)
					{
  					HelpVector[2].replace(pos, 1, "");
					}

					HelpParamStruct.aparamIP = HelpVector[1];
					HelpParamStruct.aparamPort = std::stoi( HelpVector[2]);
					HelpParamStruct.aparamPortString = HelpVector[2];
				}
				Parama = true;
			}
			else if(v[i][0]=='d' && v[i][1]==' ' && Paramd == false)// Spracovanie a kontrola vstupneho parametra -d
			{
				std::vector<string> HelpVectorAddress; // Pomocny vektor pre spracovanie adresy suboru.
				Split_arguments(HelpVector[1],HelpVectorAddress,"/");

				if(HelpVector.size() > 2)
				{
					Print_error(Err_ParamForm);
					HelpParamStruct.ParamWrong = true;
					break;
				}
				Paramd= true;

				std::string filename = HelpVectorAddress[HelpVectorAddress.size()-1];
				filename.erase(remove(filename.begin(), filename.end(), ' '), filename.end());
				HelpParamStruct.dparamfilename = filename;
				HelpParamStruct.dparamaddress = HelpVector[1];
			}
			else if(v[i][0]=='t' && v[i][1]==' ' && Paramt == false) // Spracovanie a kontrola vstupneho parametra -t
			{
				if(HelpVector.size() > 2)
				{
					Print_error(Err_ParamForm);
					HelpParamStruct.ParamWrong = true;
					break;
				}			
				Paramt = true;
				HelpParamStruct.tparam =true;
				HelpParamStruct.tparamnumber = HelpVector[1];
				if(atoi(HelpParamStruct.tparamnumber.c_str()) < 1 || atoi(HelpParamStruct.tparamnumber.c_str()) > 255)
				{
					cerr<<"Boli zadane zle hodnoty parametra -t , valitne hodnoty su <1,255>";
					exit(75);
				}
			}	
			else if(v[i][0]=='s' && v[i][1]==' ' && Params == false)// Spracovanie a kontrola vstupneho parametra -s
			{
				if(HelpVector.size() > 2)
				{
					Print_error(Err_ParamForm);
					HelpParamStruct.ParamWrong = true;
					break;
				}
				HelpParamStruct.sparamb = true;
				HelpParamStruct.sparam = HelpVector[1];
				if(atoi(HelpParamStruct.sparam.c_str()) < 8 || atoi(HelpParamStruct.sparam.c_str()) > 65464)
				{
					cerr<<"Boli zadane zle hodnoty parametra -s , valitne hodnoty su <8,65464>";
					exit(75);
				}
				Params = true;
			}
			else if(v[i][0]=='m' && Paramm == false) // Spracovanie a kontrola vstupneho parametra -m
			{
				if(HelpVector.size() > 1)
				{
					Print_error(Err_ParamForm);
					HelpParamStruct.ParamWrong = true;
					break;
				}		
				Paramm = true;
				HelpParamStruct.mparam =true;
			}
			else if(v[i][0]=='c' && v[i][1]==' ' && Paramc == false) // Spracovanie a kontrola vstupneho parametra -c
			{
				if(HelpVector.size() > 2)
				{
					Print_error(Err_ParamForm);
					HelpParamStruct.ParamWrong = true;
					break;
				}
				// kontrola hodnoty paramtera -c 
				if(HelpVector[1] == "ascii" || HelpVector[1] == "netascii" || HelpVector[1] == "binary" || HelpVector[1] == "octet" )
				{
					
					if(HelpVector[1] == "ascii")
					{
						HelpParamStruct.cparam = "netascii";
					}
					else if(HelpVector[1] == "netascii")
					{
						HelpParamStruct.cparam = HelpVector[1];
					}
					else if(HelpVector[1]=="binary")
					{
						HelpParamStruct.cparam ="octet";
					}
					else
					{
						HelpParamStruct.cparam = HelpVector[1];
					}
				}
				else
				{
					Print_error(Err_ParamForm);
					HelpParamStruct.ParamWrong = true;
					break;
				}	
				Paramc = true;
			}
			else
			{ 
				Print_error(Err_Param);
				HelpParamStruct.ParamWrong = true;
				break;	
			}
			HelpVector.clear();// vymazanie pomocneho vektora 
		}
	}
	

  if((ParamR && ParamW )||(!ParamR && !ParamW)) // Kontrola ci nebol zadany naraz  aj parameter -R aj -W alebo ani jeden z nich
	{
		if (!Paramh)
		{
			Print_error(Err_Comb);
			HelpParamStruct.ParamWrong = true;
		}
		else
		{
			PrintHelp();
		}		
	}
	else
	{
		if (Paramh)
		{
			Print_error(Err_Comb);
			HelpParamStruct.ParamWrong = true;
		}
		if(Paramd == false)
		{
			Print_error(Err_Comb);
			HelpParamStruct.ParamWrong = true;
		}
	}
	return HelpParamStruct;
}


int main ()
{
  std::string mystr;
  std::vector<string> v; 
  struct addrinfo hints,*ServerInfo;
  while(1)
  {
  	// Spracovanie parametrov
  	long data_calculator = 1;
  	Parameters ClientParameter;
  	cout << ">";
  	getline (cin, mystr);
  	if(mystr == "exit")
  	{
  		break;
  	}	
  	Split_arguments(mystr,v,"-");
  	ClientParameter = Check_arguments(v);
  	v.clear();	
  	if(ClientParameter.ParamWrong == true)
   	{
   		continue;
   	}

  	const char *charAddress = ClientParameter.aparamIP.c_str();
  	memset(&hints, 0, sizeof hints);
  	hints.ai_family = AF_UNSPEC;
  	hints.ai_socktype = SOCK_DGRAM;

  	int q =0;
  	// ziskanie informacii potrebnych k vyrvoreniu socketu
  	if (( q = getaddrinfo(charAddress, ClientParameter.aparamPortString.c_str(), &hints, &ServerInfo)) != 0) 
  	{
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(q));
        return 1;
    }

  	int UdpSocket = socket(ServerInfo->ai_family,ServerInfo->ai_socktype,IPPROTO_UDP);
  	if(UdpSocket < 0 )
  	{
  		Print_information("");
  		Print_error(ErrSock);
  		exit(1);
  	}
 
  	struct ifreq ifr;
  	int ioctlcouner = 0;
  	int lowest_mtu = INT_MAX;
  	while(1) // iterovanie cez vsetky rozhrania a ziskanie minimalneho MTU z nich
  	{
  		ioctlcouner++;
  		ifr.ifr_ifindex = ioctlcouner;
			if(ioctl(UdpSocket, SIOCGIFNAME, &ifr) == -1)
			{
				break;
			}
			if(ioctl(UdpSocket, SIOCGIFMTU, &ifr)== -1)
			{
				break;
			}
			ifr.ifr_mtu;
			
			if(ifr.ifr_mtu < lowest_mtu)
			{
				lowest_mtu = ifr.ifr_mtu;
			}
  	}

  	if(ServerInfo->ai_family == 10)//ipv6
  	{
  		lowest_mtu = lowest_mtu -52;
  	}
  	else
  	{ 
  		lowest_mtu = lowest_mtu -32;
  	}
  	if(ClientParameter.sparamb == true)
  	{
  		if(atoi(ClientParameter.sparam.c_str()) > lowest_mtu)
  		{
  			std::string lowest_mtu_string = std::to_string(lowest_mtu);
  			Print_information("Zadana hodnota parametru -s je vacsia ako MTU, prenos pokracuje s hodnotou "+lowest_mtu_string+"(MTU -(IP header + TFTP header + UDP header))");
  			ClientParameter.sparam = lowest_mtu_string;
  		}
  	}
  	// ----------------------------------------------Vytvorenie RRQ requestu ----------------------------------------
   	if(ClientParameter.Rparam == true)
  	{
  		const char *charFilename = ClientParameter.dparamaddress.c_str();
  		char buffer[516]; 
  		bool request_passed = false;
  		int count;
  		int request_length;
  		uint16_t recived_block_number_minus_one = 0;
  		uint16_t recived_block_number ;
  		FILE *output_file;
 		  
 		  request_length = Create_request(buffer,ClientParameter,charFilename,"0"); // vytvorenie read requestu 

  		if((count = sendto(UdpSocket,buffer,request_length,0,ServerInfo->ai_addr,ServerInfo->ai_addrlen ))== -1)
  		{
  			Print_error(Err_SendRRQ);
  		}

  		int retry_count = 0;
  		OACKreturn OACKvalues;
  		bool tsizeoutput = false;
  		int size_of_recv_buffer = 512;
  		bool read_error = false;
  		bool first_data = true;
  		while(1)
  		{
				char *recvbuffer = new char[size_of_recv_buffer+4];
  			struct sockaddr_storage Incserver;
  			socklen_t adress_size = sizeof(Incserver);
  			bzero(&Incserver, sizeof(Incserver));
  			int recv_count;
  			int recv_timeout = recvtimeout(UdpSocket,3); // timeout 

  			if(recv_timeout == -2) // nastal timeout;
  			{
  				Print_information("Odosielam READ request na server "+ClientParameter.aparamIP+":"+ClientParameter.aparamPortString);
  				if (retry_count == 3)
  				{
  				  read_error = true;
  					break;
  				}
  				else
  				{
  					retry_count ++; // premenna urcuje pocet opakovania timeoutu 
  					if( request_passed == false)
  					{
  						if((count = sendto(UdpSocket,buffer,request_length,0,ServerInfo->ai_addr,ServerInfo->ai_addrlen))== -1)
							{
								Print_error(Err_SendRRQ); // error by mal byt nieco ako 3krat sa nepodarilo obdrzat packet od serveru . koniec
							
							}
  					}
  					else
  					{
  						buffer[0]=0;
							buffer[1]=4;
  						if((count = sendto(UdpSocket,buffer,4,0,(struct sockaddr *)&Incserver,adress_size)) == -1)
							{
								Print_error(Err_SendACK); 
							}
  					}						
  				}
  			}
  			else if(recv_timeout == -1) // nastal error pri timeout
  			{
  				cerr<< "Error pri recvtimeout";
  			}
  			else
  			{
  				recv_count=recvfrom(UdpSocket,recvbuffer,size_of_recv_buffer+4,0,(struct sockaddr *)&Incserver,&adress_size); // prijmanie dat zo serveru 
  				request_passed = true;
  				if(recv_count == -1)
  				{
  					Print_error(Err_Recv);
  				}
  				
  				if(ntohs(*(short *)recvbuffer) == 8) // server zamietol options a ukoncil spojenie 
  				{
  					Print_information("");
  					cerr<<"Server zamietol options a ukoncuje spojenie";
  					read_error = true;
  					break;
  				}
  				
  				if(ntohs(*(short *)recvbuffer) == 6) // Server odoslal OACK
  				{
  					Print_information("Server "+ClientParameter.aparamIP+":"+ClientParameter.aparamPortString+" potvrdil READ request");
  					OACKvalues = Handle_OACK(recvbuffer,ClientParameter,recv_count);
  					if(ClientParameter.sparamb == true && OACKvalues.blocksize!="")
  					{
  						size_of_recv_buffer = atoi(OACKvalues.blocksize.c_str());
  					}
  					recvbuffer[0]=0;
						recvbuffer[1]=4;
						recvbuffer[2]=0;
						recvbuffer[3]=0;
						if((count = sendto(UdpSocket,recvbuffer,4,0,(struct sockaddr *)&Incserver,adress_size)) == -1)
						{
							Print_error(Err_SendACK);
						}		
  				}
  				else if (ntohs(*(short *)recvbuffer) == 3) // server odoslal data packet 
  				{
  					if(OACKvalues.tsize == "" && OACKvalues.tsizebool == false)
  					{
  						Print_information("Server odmietol transfer size option");
  						OACKvalues.tsizebool = true;
  					}
  					
  					if(OACKvalues.tsize =="" && tsizeoutput == false)
  					{
  						Print_information("Server neprial option tsize ale transfer pokracuje v zakladnom nastaveni");
  						tsizeoutput = true; 
  					}
  					recived_block_number = recvbuffer[3] | recvbuffer[2] << 8;
  					if(recived_block_number_minus_one != recived_block_number )
  					{
  						if(first_data == true)
  						{
  							output_file=fopen(ClientParameter.dparamfilename.c_str(),"w"); // vytvorenie noveho suboru s nazvom ktory dostaneme z -d parametra
  							fclose(output_file);
  							first_data = false;
  						}
  						if(ClientParameter.cparam == "octet")
  						{
  							output_file = fopen(ClientParameter.dparamfilename.c_str(),"ab");
								fwrite(&recvbuffer[4], 1, recv_count - 4, output_file);					
  						}
  						else
  						{
  							output_file = fopen(ClientParameter.dparamfilename.c_str(),"a+");
  							char *precv = &recvbuffer[4];
  							char *out_buffer=new char[4+size_of_recv_buffer];
  							int size = Convert_from_netascii(out_buffer,precv,recv_count);
  							fwrite(&out_buffer[0], 1, size, output_file);
  							delete[] out_buffer;		
  						}
  					}			
  					fclose (output_file);
						recvbuffer[0]=0;
						recvbuffer[1]=4;
						recived_block_number_minus_one = recived_block_number;
						if((count = sendto(UdpSocket,recvbuffer,4,0,(struct sockaddr *)&Incserver,adress_size)) == -1)
						{
							Print_error(Err_SendACK);
						}

						if(recv_count < (size_of_recv_buffer+4))
  					{
  						break;
  					}
  				}
  				else if(ntohs(*(short *)recvbuffer) == 5) // server odoslal error packet
  				{
  					Print_information("");
  					cerr<<"Server odoslal error packet : ";
  					int err_number = recvbuffer[3] | recvbuffer[2] << 8;
  					Print_tftp_error(err_number);
  	    		read_error=true;
  					break;
  				}					
  			}		
  			 // spracovanie a vypisu pri prijmani dat 
  			long long number_of_data_help = data_calculator*size_of_recv_buffer;	
				data_calculator++;
				std::string number_of_data = std::to_string(number_of_data_help);					
				Print_transfer_information(recived_block_number,number_of_data,OACKvalues,size_of_recv_buffer," Ziskavanie dat so servera ... ");	
				delete[] recvbuffer;	

  		} // koniec while

  		if(read_error == false)
  		{
  			Print_information("Prenos suboru "+ClientParameter.dparamfilename+" prebehol uspesne.");
  		}
  		else
  		{
  			Print_information("Pri prenose suboru "+ClientParameter.dparamfilename+" nastala chyba.");
  		}
  		
  	}
  	//----------------------------------------- Vytvorenie WRQ requestu	---------------------------------------------
  	else if (ClientParameter.Wparam == true)
  	{
  		std::cout << std::flush;
  		// inicializacia potrebnych premennych 
  		const char *charFilename = ClientParameter.dparamfilename.c_str();
  		char buffer[516];
  		char ack_buffer_request[516];
  		unsigned char ack_buffer[4];
  		bool ack_good = false;
  		uint16_t control_ack_number;
  		int count;
  		int request_length;
  		ssize_t Data_length;
  		uint16_t Data_block_number = 0;
  		FILE *output_file ;
  		OACKreturn OACKvalues;
  		long int file_size = Calculate_size(ClientParameter.dparamaddress.c_str());
  		std::string file_size_string = std::to_string(file_size);
  		int size_of_output_buffer =512;
  		bool err_packet = false;
  		// otvorenie suboru na citanie 
  		if(ClientParameter.cparam=="octet")
  		{
  			if((output_file = fopen(ClientParameter.dparamaddress.c_str(),"rb"))== NULL)
  			{
  				exit(2);
  			}
  		}
  		else
  		{
  			if((output_file = fopen(ClientParameter.dparamaddress.c_str(),"r"))== NULL)
  			{
  				exit(2);
  			}
  		}
  		
  		request_length = Create_request(buffer,ClientParameter,charFilename,file_size_string); // vytvorenie wrq requestu 
  		
  		int TimesRepeat = 0;
  		struct sockaddr_storage Incserver;
  		socklen_t adress_size = sizeof(Incserver);
  		bzero(&Incserver, sizeof(Incserver));
  		int recv_count;
  		int recv_timeout = 0;
  		
  		for (TimesRepeat = 0;TimesRepeat<4;++TimesRepeat) // cyklus ktory realizuje opakovany timeout
  		{
  			recv_timeout = recvtimeout(UdpSocket,2);
  			
  			if(recv_timeout == -2) // nastal timeout 
  			{
  				Print_information("Odosielam WRITE request na server "+ClientParameter.aparamIP+":"+ClientParameter.aparamPortString);
  			
  				if((count= sendto(UdpSocket,buffer,request_length,0,ServerInfo->ai_addr,ServerInfo->ai_addrlen)) == -1 ) // opetovne odosielanie dat 
  				{
  					Print_error(Err_SendRRQ);
  					exit(10);
  				}
  			}
  			else if(recv_timeout == -1) // nastal error
  			{
  				cerr<< "Error pri recvtimeout";
  			}
  			else
  			{
  				recv_count=recvfrom(UdpSocket,ack_buffer_request,516,0,(struct sockaddr *)&Incserver,&adress_size); // prijmanie odpovete od serveru 
  				
  				if(ntohs(*(short *)ack_buffer_request) == 5) // server odoslal error packet
			 		{
			 			err_packet = true;
			 			Print_information("");
			 			cerr<<"Server odoslal error: ";
  	    		int err_number = ack_buffer_request[3] | ack_buffer_request[2] << 8;
  					Print_tftp_error(err_number);
  	    		break;
  	   	 	}
  				else if(ntohs(*(short *)ack_buffer_request) == 8)// server zamietol options a ukoncuje spojenie 
  				{
  					Print_information("");
  					cerr<<"Server zamietol options a ukoncuje spojenie";
  					err_packet = true;
  					break;		
  				}
  				else if (ntohs(*(short *)ack_buffer_request) == 6)// server odoslal OACK
  				{	
  					Print_information("Server "+ClientParameter.aparamIP+":"+ClientParameter.aparamPortString+" potvrdil WRITE request");
  					OACKvalues = Handle_OACK(ack_buffer_request,ClientParameter,recv_count);		
  					ack_good = true;
  					break;
  				}
  				else if(ntohs(*(short *)ack_buffer_request) == 4)// Server odoslal ACK 
  	    	{
  	    		control_ack_number = ack_buffer_request[3] | ack_buffer_request[2] << 8;
  	    		if(control_ack_number == Data_block_number)
  	    		{
  	    			break;
  	    		}
			  	} 
  			}      
  		}
  		
  		if(ack_good == false && err_packet == false)
  		{
  			Print_information("");
  			cerr<<"nepodarilo sa nadviazat spojenie so serverom \n";
  			exit(100);
  		}

  		if(ClientParameter.sparamb == true && OACKvalues.blocksize != "")
  		{
  			 size_of_output_buffer =atoi(OACKvalues.blocksize.c_str());		
  		}

  		if(err_packet == false)
  		{ 
  			char overflow_char =' ';
  	/*---------------------------------------------ODOSIELANIE DAT -----------------------------------------------------------------*/ 
  			while(1)
  			{
  				std::cout << std::flush;
  				char *data_buffer=new char[size_of_output_buffer];              // data buffer pre klasicky data packet
  				char *data_packet_buffer =new char[size_of_output_buffer+4];			 // buffer pre cely data packet
					bool last_data_packet = false;	
					if(ClientParameter.cparam == "octet") // ak sa jedna o octet mod tak nacitam priamo data 
					{
						Data_length = fread(data_buffer,1,size_of_output_buffer,output_file);	
					}
					else // mod prenasanie dat je ascii
					{
						int read_counter = 0;

						if(overflow_char !=' ')
						{
							data_buffer[read_counter] = overflow_char;
							read_counter++;
							overflow_char = ' ';
						}

						while(read_counter<size_of_output_buffer) // nacitanie dat a spracovanie podla normy netascii
						{
							int loaded_character = fgetc(output_file);
							char loaded_char = (char)loaded_character;	
		
							if(loaded_character == EOF)
							{
								break;
							}

							if(loaded_char =='\n')
							{
								if(read_counter != (size_of_output_buffer-1))
								{ 
									data_buffer[read_counter]='\r';
									data_buffer[read_counter+1]='\n';
									read_counter=read_counter+2;
								}
								else
								{
									data_buffer[read_counter]='\r';
									read_counter++;
									overflow_char = '\n';
								}

							}
							else if(loaded_char =='\r')
							{
								if(read_counter != (size_of_output_buffer-1))
								{
									data_buffer[read_counter]='\r';
									data_buffer[read_counter+1]='\0';
									read_counter=read_counter+2;
								}
								else
								{
									data_buffer[read_counter]='\r';
									read_counter++;
									overflow_char = '\0';
								}
							}
							else
							{
								data_buffer[read_counter]= loaded_char;
								read_counter++;
							}
						}
						Data_length = read_counter;
						cout<<Data_length;
						cout<<"\n";
					}	
					char *last_data_buffer = new char[Data_length]; // data buffer pre posledny data packet
  				char *last_data_packet_buffer=new char[Data_length+4]; // buffer pre cely posledny data packet	Data + OPCODE + block number
	  			Data_block_number++;
  				if(Data_length == size_of_output_buffer) // vytvorenie a naplnenie klasickeho data packetu 
  				{
  					data_packet_buffer[0] = 0;
  					data_packet_buffer[1] = 3;
  					uint16_t Data_block_number_converted = htons(Data_block_number);
  					data_packet_buffer[2] = Data_block_number_converted & 0xff;
  					data_packet_buffer[3] = Data_block_number_converted>>8;
  					memcpy(&data_packet_buffer[4],data_buffer,size_of_output_buffer);
 		 			}
  				else // vytvorenie a naplnenie posledneho data packetu
  				{
  				
  					last_data_packet_buffer[0] = 0;
 	 					last_data_packet_buffer[1] = 3;
  					memcpy(last_data_buffer,data_buffer,Data_length);
  					uint16_t Data_block_number_converted = htons(Data_block_number);
  					last_data_packet_buffer[2] = Data_block_number_converted & 0xff;
  					last_data_packet_buffer[3] = Data_block_number_converted>>8;
  					memcpy(&last_data_packet_buffer[4],last_data_buffer,Data_length);
  					last_data_packet = true;
  				} 

  				for (TimesRepeat = 0;TimesRepeat<5;++TimesRepeat) // Cyklus ktory riesi opetovne odosielanie dat v pripade timeoutu
 		 			{		
  					if(last_data_packet == true) // ak som odosielal posledny packet
  					{
  						if((count = sendto(UdpSocket,last_data_packet_buffer,Data_length+4,0,(struct sockaddr *)&Incserver,adress_size))==-1)
 		 					{
  							cerr<< "Nastala chyba pri odosielani dat";
  							exit(10);
  						}
 		 				}
  					else
  					{
  						if((count = sendto(UdpSocket,data_packet_buffer,Data_length+4,0,(struct sockaddr *)&Incserver,adress_size))==-1)
  						{
  							cerr<< "Nastala chyba pri odosielani dat";
  							exit(10);
  						}
  					}

  					recv_timeout = recvtimeout(UdpSocket,2);
  					if(recv_timeout == -2) // nastal timeout 
  					{
  						if(last_data_packet == true)
  						{
  							if((count = sendto(UdpSocket,last_data_packet_buffer,Data_length+4,0,(struct sockaddr *)&Incserver,adress_size))==-1)
  							{
  								cerr<< "Nastala chyba pri odosielani dat";
  								exit(10);
  							}
  						}
  						else
  						{
  							if((count = sendto(UdpSocket,data_packet_buffer,Data_length+4,0,(struct sockaddr *)&Incserver,adress_size))==-1)
  							{
  								cerr<< "Nastala chyba pri odosielani dat";
  								exit(10);
  							}
  						}
  					}
  					else if (recv_timeout == -1)
 		 				{
  						cerr<< "Pocas timeoutu nastala chyba";
  						exit(11);
  					}
  					else
  					{
  						recv_count=recvfrom(UdpSocket,ack_buffer,4,0,(struct sockaddr *)&Incserver,&adress_size); // ziskavanie dat od serveru 
  	    	
  	  	  		// kontrola ack paketu
  		    		if(ntohs(*(short *)ack_buffer) == 5)
  	   		 		{
  	    				err_packet = true;
  	    			}

  	    			if(ntohs(*(short *)ack_buffer) == 4) // kontorla ci sa jedna o ACK packet
  	   				{
  	  	  			control_ack_number = ack_buffer[3] | ack_buffer[2] << 8;   // ziskanie block number z ack packetu

  		    			if(control_ack_number == Data_block_number) // kontrola ci sa hodnota z ack packetu rovna block number odoslaneho packetu 
 		 	    			{
  	    					break;
  	    				}
  	    			}
  	    			if(last_data_packet == true) // ak sa jednalo o posledny packet tak sa for cyklus prerusi 
  	    			{
  	  	  			break;
  		    		}
  		    		if(TimesRepeat == 4 ) 
	  	    		{
	  	    			Print_information("");
  	    				cerr<<"Nastala chyba pri prenose suboru, prenos sa ukoncuje ";
  	    				err_packet =true;
  	    			}
  					}
  				}		
  				if(Data_length < size_of_output_buffer)
  				{
  					break;
  				}

  				if(err_packet == true)
  				{
  					break;
  				}
					// riesenie vypisu pri odosielani dat 
					long long number_of_data_help = data_calculator*Data_length;	
					data_calculator++;
					std::string number_of_data = std::to_string(number_of_data_help);					
					Print_transfer_information(Data_block_number,number_of_data,OACKvalues,Data_length,"Odosielanie dat na server ... ");	
					delete[] data_buffer;
					delete[] data_packet_buffer;
					delete[] last_data_buffer;
					delete[] last_data_packet_buffer;

  			}// koniec while
  		
      }
  		if(err_packet == false)
  		{
  			Print_information("Prenos suboru "+ClientParameter.dparamfilename+" prebehol uspesne.");
  		}
  		else
  		{
  			Print_information("Prenos suboru "+ClientParameter.dparamfilename+" bol neuspesny.");
  		}
  		fclose(output_file);
  	}
  	freeaddrinfo(ServerInfo);
  	close(UdpSocket);
  }

  return 0;
}




