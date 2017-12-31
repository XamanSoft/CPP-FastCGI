#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

Record::Record() {
}

Record::Record(Record::RequestType type, int requestId, int paddingLength) {
	data.reserve(HEADER_LEN);
	data[0] = VERSION;
	data[1] = type;
	data[2] = requestId >> 8 & 0xFF;
	data[3] = requestId & 0xFF;
	data[6] = paddingLength;
	data[7] = 0; // reserved
}

Record::Record(CppSystemRT::File& file) {
	read(file);
}

Record::~Record() {
}

int Record::read(CppSystemRT::File& file) {
	data.clear();
	data.reserve(HEADER_LEN);
	int rLen = file.read((char*)data.data(), HEADER_LEN);
	if (rLen == HEADER_LEN) {
		int cLength = data[4] << 8 | data[5], pLength = data[6];
		data.resize(HEADER_LEN+cLength+pLength);
		rLen += file.read((char*)(data.data()+HEADER_LEN), cLength+pLength);
	}
	return rLen;
}

int Record::write(CppSystemRT::File& file) {
	if (data.size() < (HEADER_LEN+contentLength()+data[6])) // write padding
		data.resize(data.size() + data[6], 0);
	return file.write((char*)data.data(), data.size());
}

int Record::requestId(int id) {
	if (id > -1) {
		data[2] = id >> 8 & 0xFF;
		data[3] = id & 0xFF;
	} else {
		id = data[2] << 8 | data[3];
	}

	return id;
}

int Record::requestType(int type) {
	if (type > RT_NONE)
		data[1] = type;
	else
		type = data[1];
	
	return type;
}

int Record::contentLength(int length) {
	if (length > -1) {
		data[4] = (length >> 8 & 0xFF);
		data[5] = (length & 0xFF);
	} else {
		length = (int)data[4] << 8 | data[5];
	}
	
	return length;
}

void Record::readNameValuePair(std::map<std::string, std::string>& params) {
	std::string name;
	std::string value;
	
	for (int index = HEADER_LEN, size = 0, nameLength = 0, valueLength = 0; index < data.size(); index += size) {
		name.clear();
		value.clear();

		if (data[index] & 0x80) {
			if (data[index+4] & 0x80) {
				size = 8;
				nameLength = ((data[index] & 0x7f) << 24) + (data[index+1] << 16) + (data[index+2]  << 8) + data[index+3];
				valueLength = ((data[index+4] & 0x7f) << 24) + (data[index+5] << 16) + (data[index+6]  << 8) + data[index+7];
			} else {
				size = 5;
				nameLength = ((data[index] & 0x7f) << 24) + (data[index+1] << 16) + (data[index+2]  << 8) + data[index+3];
				valueLength = data[index+4];
			}
		} else {
			if (data[index+1] & 0x80) {
				size = 5;
				nameLength = data[index];
				valueLength = ((data[index+1] & 0x7f) << 24) + (data[index+2] << 16) + (data[index+3]  << 8) + data[index+4];
			} else {
				size = 2;
				nameLength = data[index];
				valueLength = data[index+1];
			}
		}
		for (int j = size; j < (size+nameLength); j++) {
			name += data[j];
		}
		size += nameLength;
		for (int j = size; j < (size+valueLength); j++) {
			value += data[j];
		}
		size += valueLength;
		params[name] = value;
	}
}

void Record::writeNameValuePair(std::map<std::string, std::string> const& params) {
	for (auto& nm : params) {
		if (nm.first.size() < 128) {
			data.push_back(nm.first.size());
		} else {
			int sz = nm.first.size();
			data.push_back(((sz >> 24) & 0x7F)|0x80);
			data.push_back((sz >> 16) & 0xFF);
			data.push_back((sz >> 8) & 0xFF);
			data.push_back(sz & 0xFF);
		}
		
		if (nm.second.size() < 128) {
			data.push_back(nm.second.size());
		} else {
			int sz = nm.second.size();
			data.push_back(((sz >> 24) & 0x7F)|0x80);
			data.push_back((sz >> 16) & 0xFF);
			data.push_back((sz >> 8) & 0xFF);
			data.push_back(sz & 0xFF);
		}
		
		for (int j = 0; j < nm.first.size(); j++) {
			data.push_back(nm.first[j]);
		}
		
		for (int j = 0; j < nm.second.size(); j++) {
			data.push_back(nm.second[j]);
		}
	}
	
	int cLength = data.size() - HEADER_LEN;
	data[4] = (cLength >> 8 & 0xFF);
	data[5] = (cLength & 0xFF);
}

void Record::writeUnknownTypeBody(int type) {
	data.resize(HEADER_LEN+8);
	std::memset(data.data()+HEADER_LEN, 0, 8);
	data[HEADER_LEN] = type;
}

int Record::readUnknownTypeBody() {
	if (contentLength() != 8) return -1;

	return data[HEADER_LEN];
}

void Record::readBeginRequestBody(int& role, int& flags) {
	role = data[HEADER_LEN] << 8 | data[HEADER_LEN+1];
	flags = data[HEADER_LEN+2];
}

void Record::writeBeginRequestBody(int const& role, int const& flags) {
	data.resize(HEADER_LEN+8);
	std::memset(data.data()+HEADER_LEN, 0, 8);

	data[HEADER_LEN] = (role >> 8) & 0xFF;
	data[HEADER_LEN+1] = role & 0xFF;
	data[HEADER_LEN+2] = flags;
}

void Record::readEndRequestBody(int& appStatus, int& protocolStatus) {
	appStatus = ((int)data[HEADER_LEN] << 24) | ((int)data[HEADER_LEN+1]  << 16) | ((int)data[HEADER_LEN+2] << 8) | (int)data[HEADER_LEN+3];
	protocolStatus = data[HEADER_LEN+4];
}

void Record::writeEndRequestBody(int const& appStatus, int const& protocolStatus) {
	data.resize(HEADER_LEN+8);
	std::memset(data.data()+HEADER_LEN, 0, 8);
	data[HEADER_LEN] = (appStatus >> 24) & 0xFF;
	data[HEADER_LEN+1] = (appStatus >> 16) & 0xFF;
	data[HEADER_LEN+2] = (appStatus >> 8) & 0xFF;
	data[HEADER_LEN+3] = appStatus & 0xFF;
	data[HEADER_LEN+4] = protocolStatus;
}

void Record::readData(std::stringstream& dat) {
	dat.write((const char*)data.data()+HEADER_LEN, contentLength());
}

void Record::writeData(std::stringstream& dat) {
	data.resize(HEADER_LEN+dat.str().size());
	dat.read((char*)data.data()+HEADER_LEN, data.size());
}
