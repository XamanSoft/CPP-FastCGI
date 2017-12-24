#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

Protocol::Protocol(CppSystemRT::Socket* sock): paddingLength(0), socket(sock) {
}

Protocol::~Protocol() {
}

void Protocol::readNameValuePair() {
	std::string name;
	std::string value;
	
	for (int index = 0, size = 0, nameLength = 0, valueLength = 0; index < data.size(); index += size) {
		name.clear();
		value.clear();

		if (data[index] & 0x80) {
			if (data[index+4] & 0x80) {
				size = 8;
				nameLength = (data[index] & 0x7f) << 24) + (data[index+1] << 16) + (data[index+2]  << 8) + data[index+3];
				valueLength = (data[index+4] & 0x7f) << 24) + (data[index+5] << 16) + (data[index+6]  << 8) + data[index+7];
			} else {
				size = 5;
				nameLength = (data[index] & 0x7f) << 24) + (data[index+1] << 16) + (data[index+2]  << 8) + data[index+3];
				valueLength = data[index+4];
			}
		} else {
			if (data[index+1] & 0x80) {
				size = 5;
				nameLength = data[index];
				valueLength = (data[index+1] & 0x7f) << 24) + (data[index+2] << 16) + (data[index+3]  << 8) + data[index+4];
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

void Protocol::writeNameValuePair() {
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

void Protocol::writeUnknownTypeBody(int type) {
	data.resize(8);
	std::memset(data.data(), 0, 8);
	data[0] = type;
}

int Protocol::readUnknownTypeBody() {
	if (data.size() != 8) return -1;

	return data[0];
}

void Protocol::readBeginRequestBody(int& role, int& flags) {
	role = data[0] << 8 | data[1];
	flags = data[2];
}

void Protocol::writeBeginRequestBody(int const& role, int const& flags) {
	data.resize(8, 0);
	std::memset(data.data(), 0, 8);

	data[0] = (role >> 8) & 0xFF;
	data[1] = role & 0xFF;
	data[2] = flags;
}

void Protocol::readEndRequestBody(int& appStatus, int& protocolStatus) {
	appStatus = ((int)data[0] << 24) | ((int)data[1]  << 16) | ((int)data[2] << 8) | (int)data[3];
	protocolStatus = data[4];
}

void Protocol::writeEndRequestBody(int const& appStatus, int const& protocolStatus) {
	data.resize(8, 0);
	std::memset(data.data(), 0, 8);
	data[0] = (appStatus >> 24) & 0xFF;
	data[1] = (appStatus >> 16) & 0xFF;
	data[2] = (appStatus >> 8) & 0xFF;
	data[3] = appStatus & 0xFF;
	data[4] = protocolStatus;
}

bool Protocol::read() {
	if (getByte() != VERSION)
		return false;

	requestType	= getByte();
	requestId = getByte() << 8 | getByte();
	contentLength = getByte() << 8 | getByte();
    paddingLength = getByte();
    getByte(); // reserved
	data.resize(contentLength, 0);
	std::memset(data.data(), 0, contentLength);
	socket->read(data->data(), contentLength);

	if (paddingLength) {
		unsigned char padding[paddingLength]{0};
		socket->read(padding, paddingLength);
	}

	return true;
}

bool Protocol::send() {
	if (!writeByte(VERSION))
		return false;

	writeByte(requestType);
	writeByte(requestId >> 8 & 0xFF);
	writeByte(requestId & 0xFF);
	writeByte(contentLength >> 8 & 0xFF);
	writeByte(contentLength & 0xFF);
	writeByte(paddingLength);
    writeByte(0); // reserved
	socket->write(data->data(), data->size());

	if (paddingLength) {
		unsigned char padding[paddingLength]{0};
		socket->write(padding, paddingLength);
	}

	return true;
}

unsigned int Protocol::getByte() {
	unsigned char byte;

	if (socket->read(&byte, 1) != 1)
		return -1;

	return byte;
}

bool Protocol::writeByte(unsigned int byte) {	
	if (socket->write(&byte, 1) != 1)
		return false;

	return true;
}
