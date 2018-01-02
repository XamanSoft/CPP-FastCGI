#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

Record::Record() {
}

Record::Record(Record::RequestType type, int requestId, int paddingLength) {
	std::memset(header, 0, HEADER_LEN);
	header[0] = VERSION;
	header[1] = type;
	header[2] = requestId >> 8 & 0xFF;
	header[3] = requestId & 0xFF;
	header[6] = paddingLength;
	//header[7] = 0; // reserved
}

Record::Record(CppSystemRT::File& file) {
	read(file);
}

Record::~Record() {
}

int Record::read(CppSystemRT::File& file) {
	std::memset(header, 0, HEADER_LEN);
	data.clear();
	int rLen = file.read((char*)header, HEADER_LEN);
	if (rLen == HEADER_LEN) {
		int cLength = header[4] << 8 | header[5], padLength = header[6];

		if (cLength) {
			data.resize(cLength);
			rLen += file.read((char*)(data.data()), cLength);
		}
		
		if (padLength) {
			std::unique_ptr<char> padding(new char[padLength]);
			std::memset(padding.get(), 0, padLength);
			rLen += file.read(padding.get(), padLength);
		}
	}
	return rLen;
}

int Record::write(CppSystemRT::File& file) {
	int totalsz = 0, sz = 0;
	int cLength = data.size();
	header[4] = (cLength >> 8 & 0xFF);
	header[5] = (cLength & 0xFF);
	if ((sz = file.write((char*)header, HEADER_LEN)) != HEADER_LEN)
		return sz;
	totalsz = sz;
	if ((sz = file.write((char*)data.data(), data.size())) != data.size())
		return sz;
	totalsz += sz;
	if (header[6]) { // write padding
		std::unique_ptr<char> padding(new char[header[6]]);
		std::memset(padding.get(), 0, header[6]);
		if ((sz = file.write(padding.get(), header[6])) != header[6])
			return sz;
	}
	totalsz += sz;
	return totalsz;
}

int Record::requestId(int id) {
	if (id > -1) {
		header[2] = id >> 8 & 0xFF;
		header[3] = id & 0xFF;
	} else {
		id = (int)header[2] << 8 | header[3];
	}

	return id;
}

int Record::requestType(int type) {
	if (type > RT_NONE)
		header[1] = type;
	else
		type = header[1];
	
	return type;
}

int Record::contentLength(int length) {
	if (length > -1) {
		header[4] = (length >> 8 & 0xFF);
		header[5] = (length & 0xFF);
	} else {
		length = (int)header[4] << 8 | header[5];
	}
	
	return length;
}

void Record::readNameValuePair(std::map<std::string, std::string>& params) {
	std::string name;
	std::string value;
	int index = 0, nameLength = 0, valueLength = 0;
	
	while (index < data.size()) {
		name.clear();
		value.clear();
		nameLength = 0;
		valueLength = 0;

		if (data[index] & 0x80) {
			nameLength = (((int)data[index] & 0x7f) << 24) | ((int)data[index+1] << 16) | ((int)data[index+2]  << 8) | (int)data[index+3];
			index += 4;
		} else {
			nameLength = data[index];
			index++;
		}
		
		if (data[index] & 0x80) {
			valueLength = (((int)data[index] & 0x7f) << 24) | ((int)data[index+1] << 16) | ((int)data[index+2]  << 8) | (int)data[index+3];
			index += 4;
		} else {
			valueLength = data[index];
			index++;
		}

		for (int j = index; j < (index+nameLength); j++) {
			name += data[j];
		}
		index += nameLength;

		if (valueLength) {
			for (int j = index; j < (index+valueLength); j++) {
				value += data[j];
			}
			index += valueLength;
		}

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
}

void Record::writeUnknownTypeBody(int type) {
	data.resize(8);
	std::memset(data.data(), 0, 8);
	data[0] = type;
}

int Record::readUnknownTypeBody() {
	if (contentLength() != 8) return -1;

	return data[0];
}

void Record::readBeginRequestBody(int& role, int& flags) {
	role = data[0] << 8 | data[1];
	flags = data[2];
}

void Record::writeBeginRequestBody(int const& role, int const& flags) {
	data.resize(8);
	std::memset(data.data(), 0, 8);

	data[0] = (role >> 8) & 0xFF;
	data[1] = role & 0xFF;
	data[2] = flags;
}

void Record::readEndRequestBody(int& appStatus, int& protocolStatus) {
	appStatus = ((int)data[0] << 24) | ((int)data[1]  << 16) | ((int)data[2] << 8) | (int)data[3];
	protocolStatus = data[4];
}

void Record::writeEndRequestBody(int const& appStatus, int const& protocolStatus) {
	data.resize(8);
	std::memset(data.data(), 0, 8);
	data[0] = (appStatus >> 24) & 0xFF;
	data[1] = (appStatus >> 16) & 0xFF;
	data[2] = (appStatus >> 8) & 0xFF;
	data[3] = appStatus & 0xFF;
	data[4] = protocolStatus;
}

void Record::readData(std::stringstream& dat) {
	dat.write((const char*)data.data(), contentLength());
}

void Record::writeData(std::stringstream& dat) {
	data.resize(dat.str().size());
	dat.read((char*)data.data(), dat.str().size());
}
