
#include "StdAfx.h"
#include "SkJson.h"
#include <io.h>
#include <stdio.h>

using namespace SkJson;

bool JsonSerializer::loadFile(int fd) {
	size_t fileLen = _lseek(fd, 0, SEEK_END);
	_lseek(fd, 0, SEEK_SET);

	body.resize(fileLen);
	int bytesRead = _read(fd, &*body.begin(), fileLen);
	if (bytesRead != fileLen) {
		return false;
	}

	pos = 0;

	nextByte();

	if (node != NULL) {
		delete node;
	}
	node = readToken();

	pos = 0;
	body.clear();

	return node != NULL;
}

JNode *JsonSerializer::readToken() {
	skipSpaces();
	if (curByte == '{') {
		JObject *node = new JObject();
		nextByte();
		while (true) {
			skipSpaces();
			if (curByte == '}' || curByte == -1) {
				nextByte();
				break;
			}
			JNode *first = readToken();
			if (first->nodeKind != 1) {
				delete first;
				break;
			}
			JValue *firstValue = static_cast<JValue *>(first);
			if (firstValue->kind != 1) {
				delete firstValue;
				break;
			}
			skipSpaces();
			if (curByte != ':') {
				// invalid
				break;
			}
			nextByte();
			JNode *second = readToken();
			node->nodes[firstValue->strValue] = second;
			skipSpaces();
			if (curByte == ',') {
				nextByte();
			}
		}
		return node;
	}
	else if (curByte == '[') {
		JArray *node = new JArray();
		nextByte();
		while (true) {
			skipSpaces();
			if (curByte == ']' || curByte == -1) {
				nextByte();
				break;
			}
			JNode *value = readToken();
			node->nodes.push_back(value);
			if (curByte == ',') {
				nextByte();
			}
		}
		return node;
	}
	else if (curByte == '\'' || curByte == '"') {
		char term = curByte;
		nextByte();
		std::string strValue;
		while (true) {
			if (curByte == term || curByte == -1) {
				nextByte();
				break;
			}
			if (curByte == '\\') {
				if (!nextByte()) {
					break;
				}
				if (curByte == 'n') {
					strValue += "\n";
					continue;
				}
				if (curByte == 'r') {
					strValue += "\r";
					continue;
				}
				if (curByte == 't') {
					strValue += "\t";
					continue;
				}
				if (curByte == 'x') {
					if (!nextByte()) {
						// invalid
						break;
					}

					char hex2[3] = {0};

					hex2[0] = curByte;

					if (!nextByte()) {
						// invalid
						break;
					}

					hex2[1] = curByte;

					int hexValue = 0;
					if (1 != sscanf(hex2, "%02X", &hexValue)) {
						// invalid
						break;
					}

					strValue += (char)hexValue;
					continue;
				}
				// invalud
				break;
			}
			strValue += curByte;
			nextByte();
		}
		JValue *node = new JValue();
		node->kind = 1;
		node->strValue = strValue;
		return node;
	}
	else if (curByte == '-' || (curByte >= '0' && curByte <= '9')) {
		bool neg = (curByte == '-');
		if (neg) {
			nextByte();
		}
		std::string strValue;
		while (curByte >= '0' && curByte <= '9') {
			strValue += (char)curByte;
			nextByte();
		}
		JValue *node = new JValue();
		node->kind = 2;
		node->intValue = atoi(strValue.c_str()) * (neg ? -1 : 1);
		return node;
	}
	else {
		std::string strValue;
		while (!isspace(curByte) && curByte != ',' && curByte != ':' && curByte != -1) {
			strValue += (char)curByte;
			nextByte();
		}

		if (strValue == "true") {
			JValue *node = new JValue();
			node->kind = 2;
			node->intValue = 1;
			return node;
		}
		else if (strValue == "false") {
			JValue *node = new JValue();
			node->kind = 2;
			node->intValue = 0;
			return node;
		}
		// as null
		JValue *node = new JValue();
		return node;
	}
}

bool JsonSerializer::nextByte() {
	if (pos < body.size()) {
		curByte = body.at(pos);
		pos++;
		return true;
	}
	else {
		curByte = -1;
		return false;
	}
}
