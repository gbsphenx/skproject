
#pragma once

#include <string>
#include <vector>
#include <map>

namespace SkJson {
	class JNode {
	public:
		int nodeKind; // 1=JValue, 2=JArray, 3=JObject

		JNode(int nodeKind): nodeKind(nodeKind) {

		}

		virtual ~JNode() {

		}

		virtual std::string toString() const = 0;

		static std::string encodeString(std::string inStr) {
			std::string outStr;
			size_t x = 0, cx = inStr.size();
			for (; x < cx; x++) {
				int c = inStr[x];
				if (c <= 31 || c >= 127 || c == '\'' || c == '"' || c == '%' || c == '\\') {
					char buff[10];
					_snprintf(buff, sizeof(buff), "\\x%02X", c);
					outStr += buff;
				}
				else {
					outStr += (char)c;
				}
			}
			return outStr;
		}
	};

	class JValue : public JNode {
	public:
		JValue()
			: kind(0), JNode(1) {
		}

		std::string strValue;
		int intValue;
		int kind; // 0=null, 1=str, 2=int

		virtual std::string toString() const {
			switch (kind) {
				case 0:
					return "null";
				case 1:
					return "'" + encodeString(strValue) + "'";
				case 2:
				default:
					char buff[20];
					_snprintf(buff, sizeof(buff), "%d", intValue);
					return buff;
			}
		}
	};

	class JArray : public JNode {
	public:
		typedef std::vector<JNode *> NodesType;
		NodesType nodes;

		JArray(): JNode(2) {

		}

		virtual ~JArray() {
			NodesType::const_iterator iter = nodes.begin();
			for (; iter != nodes.end(); iter++) {
				delete *iter;
			}
		}

		virtual std::string toString() const {
			std::string body;
			body += "[";
			NodesType::const_iterator iter = nodes.begin();
			bool first = true;
			for (; iter != nodes.end(); iter++) {
				if (first) {
					first = false;
				}
				else {
					body += ", ";
				}
				body += (*iter)->toString();
			}
			body += "]";
			return body;
		}
	};

	class JObject : public JNode {
	public:
		typedef std::map<std::string, JNode *> NodesType;
		NodesType nodes;

		JObject(): JNode(3) {

		}

		virtual ~JObject() {
			NodesType::const_iterator iter = nodes.begin();
			for (; iter != nodes.end(); iter++) {
				delete iter->second;
			}
		}

		virtual std::string toString() const {
			std::string body;
			body += "{";
			NodesType::const_iterator iter = nodes.begin();
			bool first = true;
			for (; iter != nodes.end(); iter++) {
				if (first) {
					first = false;
				}
				else {
					body += ", ";
				}
				body += "'" + encodeString(iter->first) + "'";
				body += ": ";
				body += iter->second->toString();
			}
			body += "}";
			return body;
		}
	};

	class JsonSerializer {
	public:
		JsonSerializer()
			: node(NULL) {
		}
		~JsonSerializer() {
			delete node;
		}

		bool loadFile(int fd);
		bool save();

		JNode *node;

	protected:
		int curByte;
		std::string body;
		int pos;

		JNode *readToken();
		bool nextByte();
		void skipSpaces() {
			while (isspace(curByte)) {
				nextByte();
			}
		}
	};

	class JsonPathService {
	public:
		JNode *node;

		JsonPathService(JNode *node)
			: node(node) {
		}

		JsonPathService(JNode *node, const char *keyName)
			: node(node) {
			node = findNode(keyName);
		}

		JNode *findNode(const char *keyName, int nodeKindAs = -1) {
			if (node == NULL || node->nodeKind != 3) {
				return NULL;
			}
			JObject *objNode = static_cast<JObject *>(node);
			JObject::NodesType::iterator iter = objNode->nodes.find(keyName);
			if (iter == objNode->nodes.end()) {
				return NULL;
			}
			JNode *node = iter->second;
			if (nodeKindAs != -1 && nodeKindAs != node->nodeKind) {
				return NULL;
			}
			return iter->second;
		}
	};

	class JsonValueService {
	public:
		bool has;
		bool hasInt;
		JNode *node;
		JValue *valueNode;
		int intValue;

		JsonValueService(JsonPathService &path, const char *keyName)
			: has(false), hasInt(false), node(NULL), valueNode(NULL), intValue(0) {

			if (path.node == NULL || path.node->nodeKind != 3) {
				return;
			}
			JObject *objNode = static_cast<JObject *>(path.node);
			JObject::NodesType::iterator iter = objNode->nodes.find(keyName);
			if (iter != objNode->nodes.end()) {
				node = iter->second;
				has = true;

				if (node->nodeKind == 1) {
					valueNode = static_cast<JValue *>(node);

					if (hasInt = valueNode->kind == 2) {
						intValue = valueNode->intValue;
					}
				}
			}
		}
	};

	class JsonValueLookupService {
	public:
		JObject *objNode;

		JsonValueLookupService(JNode *node)
			: objNode(NULL) {

			if (node == NULL || node->nodeKind != 3) {
				return;
			}
			objNode = static_cast<JObject *>(node);
		}

		int getInt(const char *keyName, int defaultValue) {
			if (objNode != NULL) {
				JObject::NodesType::const_iterator iter = objNode->nodes.find(keyName);
				if (iter != objNode->nodes.end()) {
					JNode *node = iter->second;

					if (node->nodeKind == 1) {
						JValue *valueNode = static_cast<JValue *>(node);

						if (valueNode->kind == 2) {
							return valueNode->intValue;
						}
					}
				}
			}

			return defaultValue;
		}
	};

	class JsonArrayLookupService {
	public:
		JArray *arrayNode;

		JsonArrayLookupService(JsonPathService &path)
			: arrayNode(NULL) {

			if (path.node->nodeKind != 2) {
				return;
			}
			arrayNode = static_cast<JArray *>(path.node);
		}

		int getLen() {
			if (arrayNode != NULL) {
				return arrayNode->nodes.size();
			}
			return 0;
		}

		JNode *at(int index) {
			if (arrayNode != NULL) {
				return arrayNode->nodes.at(index);
			}
			return NULL;
		}
	};
}
