#ifndef _CPPFASTCGI_RECORD_HPP
#define _CPPFASTCGI_RECORD_HPP

namespace CppFastCGI { // CppFastCGI Begin

class Record {
public:
	// Listening socket file number
	//static constexpr const int LISTENSOCK_FILENO	= 0;

	// Number of bytes in a FCGI_Header.  Future versions of the protocol will not reduce this number.
	static constexpr const int HEADER_LEN		= 8;

	// Value for version component of FCGI_Header
	static constexpr const int VERSION			= 1;

	// Values for type component of FCGI_Header
	enum RequestType {
		RT_NONE					= 0,
		BEGIN_REQUEST			= 1,
		ABORT_REQUEST			= 2,
		END_REQUEST				= 3,
		PARAMS					= 4,
		STDIN					= 5,
		STDOUT					= 6,
		STDERR					= 7,
		DATA					= 8,
		GET_VALUES				= 9,
		GET_VALUES_RESULT		= 10,
		UNKNOWN_TYPE			= 11,
		MAXTYPE					= UNKNOWN_TYPE
	};

	// Value for requestId component of FCGI_Header
	static constexpr const int NULL_REQUEST_ID	= 0;

	// Mask for flags component of FCGI_BeginRequestBody
	static constexpr const int KEEP_CONN		= 1;

	// Values for role component of FCGI_BeginRequestBody
	enum RequestRole {
		RESPONDER				= 1,
		AUTHORIZER				= 2,
		FILTER					= 3
	};

	// Values for protocolStatus component of FCGI_EndRequestBody
	enum ProtocolStatus {
		REQUEST_COMPLETE		= 0,
		CANT_MPX_CONN			= 1,
		OVERLOADED				= 2,
		UNKNOWN_ROLE			= 3
	};

	// Variable names for FCGI_GET_VALUES / FCGI_GET_VALUES_RESULT records
	static constexpr const char* MAX_CONNS	= "FCGI_MAX_CONNS";
	static constexpr const char* MAX_REQS	= "FCGI_MAX_REQS";
	static constexpr const char* MPXS_CONNS	= "FCGI_MPXS_CONNS";

	Record();
	Record(RequestType type, int requestId = NULL_REQUEST_ID, int paddingLength = 0);
	Record(CppSystemRT::File& file);
	virtual ~Record();
	
	int requestId(int id = -1);
	int requestType(int type = RT_NONE);
	int contentLength(int length = -1);
	
	void writeUnknownTypeBody(int type);
	int readUnknownTypeBody();
	void readBeginRequestBody(int& role, int& flags);
	void writeBeginRequestBody(int const& role, int const& flags);
	void readEndRequestBody(int& appStatus, int& protocolStatus);
	void writeEndRequestBody(int const& appStatus, int const& protocolStatus);
	void readNameValuePair(std::map<std::string, std::string>& params);
	void writeNameValuePair(std::map<std::string, std::string> const& params);

	void readData(std::stringstream& dat);
	void writeData(std::stringstream& dat);

	int read(CppSystemRT::File& file);
	int write(CppSystemRT::File& file);
	
private:
	unsigned char header[HEADER_LEN];
	std::vector<unsigned char> data;
};

} // CppFastCGI End

#endif
