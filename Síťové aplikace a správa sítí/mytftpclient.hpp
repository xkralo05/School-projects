int main();
#include <string>
#include <vector>
struct Parameters{
	bool Rparam = false;
	bool Wparam = false;
	std::string dparamaddress = "";
	std::string dparamfilename = "";
	bool tparam = false;
	std::string tparamnumber = ""; 
	std::string sparam = "";
	bool sparamb = false;
	bool mparam = false;
	std::string cparam = "octet";
	std::string aparamIP = "127.0.0.1";
	std::string aparamPortString = "69";
	int aparamPort = 69;
	bool ParamWrong= false;
	

};

struct OACKreturn{
	std::string tsize = "";
	std::string timeout = "";
	std::string blocksize = "";
	bool tsizebool = false;
};

enum ErrorEnum
{
	Err_Comb,
	Err_Param,
	Err_ParamForm,
	ErrSock,
	Err_SendRRQ,
	Err_Recv,
	Err_SendACK
};

enum OpcodeEnum
{
	RRQ,
	WRQ,
	DATA,
	ACK,
	ERROR
};

void Split_arguments(std::string mystr, std::vector<std::string> &v,std::string delimeter);
Parameters Check_arguments(std::vector<std::string>v);
void Print_error(ErrorEnum Err);
long long int Calculate_size(const char* filename[100]);
int recvtimeout(int socket ,int timeout_sec);
void Print_information(std::string information);
void Print_transfer_information(int Data_block_number,std::string number_of_data,OACKreturn OACKvalues, int Data_length,std::string transfer_type);
OACKreturn Handle_OACK(char* buffer, Parameters ClientParameter, int recv_count) ;
int  Convert_from_netascii(char* out_buffer, char*precv  ,int recv_count);
void PrintHelp();
int Create_request(char* buffer, Parameters ClientParameter, const char* charFilename, std::string file_size);
void Print_tftp_error(int err_number);
