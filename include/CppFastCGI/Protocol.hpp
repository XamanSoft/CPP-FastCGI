#ifndef _CPPFASTCGI_DATATYPE_HPP
#define _CPPFASTCGI_DATATYPE_HPP

namespace CppFastCGI { // CppFastCGI Begin

namespace Protocol {  // Protocol Begin

namespace Value { // Value Begin

// Listening socket file number
const int LISTENSOCK_FILENO	= 0;

// Number of bytes in a FCGI_Header.  Future versions of the protocol will not reduce this number.
const int HEADER_LEN		= 8;

// Value for version component of FCGI_Header
const int VERSION			= 1;

// Values for type component of FCGI_Header
const int BEGIN_REQUEST		= 1;
const int ABORT_REQUEST		= 2;
const int END_REQUEST		= 3;
const int PARAMS			= 4;
const int STDIN				= 5;
const int STDOUT			= 6;
const int STDERR			= 7;
const int DATA				= 8;
const int GET_VALUES		= 9;
const int GET_VALUES_RESULT	= 10;
const int UNKNOWN_TYPE		= 11;
const int MAXTYPE			= UNKNOWN_TYPE;

// Value for requestId component of FCGI_Header
const int NULL_REQUEST_ID	= 0;

// Mask for flags component of FCGI_BeginRequestBody
const int KEEP_CONN			= 1;

// Values for role component of FCGI_BeginRequestBody
const int RESPONDER			= 1;
const int AUTHORIZER		= 2;
const int FILTER			= 3;

// Values for protocolStatus component of FCGI_EndRequestBody
const int REQUEST_COMPLETE	= 0;
const int CANT_MPX_CONN		= 1;
const int OVERLOADED		= 2;
const int UNKNOWN_ROLE		= 3;

// Variable names for FCGI_GET_VALUES / FCGI_GET_VALUES_RESULT records
const char MAX_CONNS[]		= "FCGI_MAX_CONNS";
const char MAX_REQS[]		= "FCGI_MAX_REQS";
const char MPXS_CONNS[]		= "FCGI_MPXS_CONNS";

} // Value End

struct Record {
	unsigned char version;
	unsigned char type;
	unsigned char requestIdB1;
	unsigned char requestIdB0;
	unsigned char contentLengthB1;
	unsigned char contentLengthB0;
	unsigned char paddingLength;
	unsigned char reserved;
	unsigned char *contentData;		// [contentLength]
	unsigned char *paddingData;		// [paddingLength]
};

struct NameValuePair11 {
	unsigned char nameLengthB0;		// nameLengthB0  >> 7 == 0
	unsigned char valueLengthB0;	// valueLengthB0 >> 7 == 0
	unsigned char *nameData;		// [nameLength]
	unsigned char *valueData;		// [valueLength]
};

struct NameValuePair14 {
	unsigned char nameLengthB0;		// nameLengthB0  >> 7 == 0
	unsigned char valueLengthB3;	// valueLengthB3 >> 7 == 1
	unsigned char valueLengthB2;
	unsigned char valueLengthB1;
	unsigned char valueLengthB0;
	unsigned char *nameData;		// [nameLength]
	unsigned char *valueData;		//[valueLength	((B3 & 0x7f) << 24) + (B2 << 16) + (B1 << 8) + B0];
};

struct NameValuePair41 {
	unsigned char nameLengthB3;		// nameLengthB3  >> 7 == 1
	unsigned char nameLengthB2;
	unsigned char nameLengthB1;
	unsigned char nameLengthB0;
	unsigned char valueLengthB0;	// valueLengthB0 >> 7 == 0
	unsigned char *nameData;		// [nameLength ((B3 & 0x7f) << 24) + (B2 << 16) + (B1 << 8) + B0]
	unsigned char *valueData;		// [valueLength]
};

struct NameValuePair44 {
	unsigned char nameLengthB3;		// nameLengthB3  >> 7 == 1
	unsigned char nameLengthB2;
	unsigned char nameLengthB1;
	unsigned char nameLengthB0;
	unsigned char valueLengthB3;	// valueLengthB3 >> 7 == 1
	unsigned char valueLengthB2;
	unsigned char valueLengthB1;
	unsigned char valueLengthB0;
	unsigned char *nameData;		// [nameLength ((B3 & 0x7f) << 24) + (B2 << 16) + (B1 << 8) + B0]
	unsigned char *valueData;		// [valueLength ((B3 & 0x7f) << 24) + (B2 << 16) + (B1 << 8) + B0];
};

struct Header {
    unsigned char version;
    unsigned char type;
    unsigned char requestIdB1;
    unsigned char requestIdB0;
    unsigned char contentLengthB1;
    unsigned char contentLengthB0;
    unsigned char paddingLength;
    unsigned char reserved;
};

struct BeginRequestBody {
    unsigned char roleB1;
    unsigned char roleB0;
    unsigned char flags;
    unsigned char reserved[5];
};

struct BeginRequestRecord {
    Header header;
    BeginRequestBody body;
};

struct EndRequestBody {
    unsigned char appStatusB3;
    unsigned char appStatusB2;
    unsigned char appStatusB1;
    unsigned char appStatusB0;
    unsigned char protocolStatus;
    unsigned char reserved[3];
};

struct EndRequestRecord {
    Header header;
    EndRequestBody body;
};

struct UnknownTypeBody {
    unsigned char type;    
    unsigned char reserved[7];
};

struct UnknownTypeRecord {
    Header header;
    UnknownTypeBody body;
};

} // Protocol End

} // CppFastCGI End

#endif
