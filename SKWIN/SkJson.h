//! STL based tiny JSON serializer/deserializer support

#pragma once

#include <string>
#include <vector>
#include <map>

namespace SkJson {
	//! Root object
	class JNode {
	public:
		//! Subclass declares node kind
		int nodeKind; //!< 1=JValue, 2=JArray, 3=JObject

		JNode(int nodeKind): nodeKind(nodeKind) {

		}

		virtual ~JNode() {

		}

		//! Get JSON expression in string.
		virtual std::string toString() const = 0;

		//! helper to encode string as JSON without both ends quotes;
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

	//! A primitive value like an integer or a string
	class JValue : public JNode {
	public:
		JValue()
			: kind(0), JNode(1) {
		}

		//! String value for string value node
		std::string strValue;
		//! Int value for integer value node
		int intValue;
		//! Kind of value
		int kind; //!< 0=null, 1=str, 2=int

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

	//! An array
	class JArray : public JNode {
	public:
		typedef std::vector<JNode *> NodesType;
		//! Sub nodes: having ownership!
		NodesType nodes;

		JArray(): JNode(2) {

		}

		//! dtor. delete sub nodes
		virtual ~JArray() {
			NodesType::iterator iter = nodes.begin();
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

	//! An object like dictionary
	class JObject : public JNode {
	public:
		typedef std::map<std::string, JNode *> NodesType;
		//! Sub nodes: having ownership!
		NodesType nodes;

		JObject(): JNode(3) {

		}

		//! dtor. delete sub nodes
		virtual ~JObject() {
			NodesType::iterator iter = nodes.begin();
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

	//! Convert between JNode tree and JSON string
	class JsonSerializer {
	public:
		JsonSerializer()
			: node(NULL) {
		}
		//! dtor. delete the node
		~JsonSerializer() {
			delete node;
		}

		//! load JSON via I/O file handle.
		/*! node variable will be overwritten. */
		bool loadFile(int fd);
		//! NOTIMPL
		bool save();

		//! Sub node: having ownership!
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

	//! JNode lookup service
	class JsonPathService {
	public:
		//! Found node. may be NULL
		JNode *node;

		JsonPathService(JNode *node)
			: node(node) {
		}

		//! Search a node having keyName from JObject node
		JsonPathService(JNode *node, const char *keyName)
			: node(node) {
			this->node = findNode(keyName);
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

	//! JValue dedication service
	class JsonValueService {
	public:
		bool has;
		bool hasInt;
		JNode *node;
		JValue *valueNode;
		int intValue;

		//! Search a JValue by keyName
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

	//! JObject dedication service for sub nodes lookup
	class JsonValueLookupService {
	public:
		JObject *objNode;

		//! ctor with JObject node
		JsonValueLookupService(JNode *node)
			: objNode(NULL) {

			if (node == NULL || node->nodeKind != 3) {
				return;
			}
			objNode = static_cast<JObject *>(node);
		}

		//! Search a int JValue node with keyName
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

		//! Search a string JValue node with keyName
		std::string getStr(const char *keyName, std::string defaultValue) {
			if (objNode != NULL) {
				JObject::NodesType::const_iterator iter = objNode->nodes.find(keyName);
				if (iter != objNode->nodes.end()) {
					JNode *node = iter->second;

					if (node->nodeKind == 1) {
						JValue *valueNode = static_cast<JValue *>(node);

						if (valueNode->kind == 1) {
							return valueNode->strValue;
						}
					}
				}
			}

			return defaultValue;
		}
	};

	//! Extract a JNode from the JArray
	class JsonArrayLookupService {
	public:
		JArray *arrayNode;

		JsonArrayLookupService(JsonPathService &path)
			: arrayNode(NULL) {

			if (path.node == NULL || path.node->nodeKind != 2) {
				return;
			}
			arrayNode = static_cast<JArray *>(path.node);
		}

		//! Get length. 0 if not array
		int getLen() {
			if (arrayNode != NULL) {
				return arrayNode->nodes.size();
			}
			return 0;
		}

		//! Get a JNode at index. NULL if not array. Assertion on invalid index.
		JNode *at(int index) {
			if (arrayNode != NULL) {
				return arrayNode->nodes.at(index);
			}
			return NULL;
		}

		//! Get a int from JValue node.
		int asInt(int index, int defaultValue) {
			if (arrayNode != NULL) {
				JNode *node = arrayNode->nodes.at(index);
				if (node->nodeKind == 1) {
					JValue *valueNode = static_cast<JValue *>(node);
					if (valueNode->kind == 2) {
						return valueNode->intValue;
					}
				}
			}
			return defaultValue;
		}
	};
}
