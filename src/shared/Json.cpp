/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <locale>
#include <iomanip>
#include <json-glib/json-glib.h>
#include <Json.h>

#define PATH_DELIM	'.'
#define MAX_NUM_DIGITS	17
#define GVAR_VALUES	"values"
#define GVAR_TYPES	"types"

using namespace ctx;

static int __count_fraction_digits(double in)
{
	int cnt;
	for (cnt = 0; in != rint(in) && cnt < MAX_NUM_DIGITS; in += in, ++cnt);
	return cnt;
}

static std::string __double_to_string(double in)
{
	/* Locale-independent double-to-string conversion */
	std::ostringstream ostr;
	ostr.imbue(std::locale("C"));
	ostr << std::fixed << std::setprecision(__count_fraction_digits(in)) << in;
	return ostr.str();
}

static double __string_to_double(const char* in)
{
	IF_FAIL_RETURN_TAG(in, 0, _E, "Parameter NULL");

	double out;

	/* Locale-independent string-to-double conversion */
	std::istringstream istr(in);
	istr.imbue(std::locale("C"));
	istr >> out;

	return out;
}

SO_EXPORT Json::Json() :
	__jsonNode(NULL)
{
	JsonObject *obj = json_object_new();
	IF_FAIL_VOID_TAG(obj, _E, "Json object construction failed");

	__jsonNode = json_node_new(JSON_NODE_OBJECT);
	if (!__jsonNode) {
		json_object_unref(obj);
		_E("Json object construction failed");
	}

	json_node_set_object(__jsonNode, obj);
	json_object_unref(obj);
}

SO_EXPORT Json::Json(const Json &j)
	: __jsonNode(NULL)
{
	__jsonNode = json_node_copy(j.__jsonNode);
	IF_FAIL_VOID_TAG(__jsonNode, _E, "Json object construction failed");
}

SO_EXPORT Json::Json(const char *s)
	: __jsonNode(NULL)
{
	if (s) {
		__parse(s);
	} else {
		__parse(EMPTY_JSON_OBJECT);
	}
}

SO_EXPORT Json::Json(const std::string &s)
	: __jsonNode(NULL)
{
	if (s.empty()) {
		__parse(EMPTY_JSON_OBJECT);
	} else {
		__parse(s.c_str());
	}
}

SO_EXPORT Json::~Json()
{
	__release();
}

void Json::__parse(const char *s)
{
	gboolean result;
	JsonParser *parser = NULL;
	JsonNode *root = NULL;

	parser = json_parser_new();
	IF_FAIL_VOID_TAG(parser, _E, "Memory allocation failed");

	result = json_parser_load_from_data(parser, s, -1, NULL);
	IF_FAIL_CATCH_TAG(result, _E, "Parsing failed");

	root = json_parser_get_root(parser);
	IF_FAIL_CATCH_TAG(root, _E, "Getting root failed");

	__jsonNode = json_node_copy(root);
	IF_FAIL_CATCH_TAG(__jsonNode, _E, "Copying failed");

CATCH:
	if (parser)
		g_object_unref(parser);
}

void Json::__release()
{
	if (__jsonNode) {
		json_node_free(__jsonNode);
		__jsonNode = NULL;
	}
}

SO_EXPORT bool Json::valid()
{
	return (__jsonNode != NULL);
}

SO_EXPORT Json& Json::operator=(const Json &j)
{
	__release();
	__jsonNode = json_node_copy(j.__jsonNode);
	if (!__jsonNode) {
		_E("Json object copy failed");
	}
	return *this;
}

SO_EXPORT Json& Json::operator=(const char *s)
{
	__release();
	if (s) {
		__parse(s);
	} else {
		__parse(EMPTY_JSON_OBJECT);
	}
	return *this;
}

SO_EXPORT Json& Json::operator=(const std::string &s)
{
	__release();
	if (s.empty()) {
		__parse(EMPTY_JSON_OBJECT);
	} else {
		__parse(s.c_str());
	}
	return *this;
}

SO_EXPORT bool Json::operator==(const Json &rhs)
{
	return __nodeEq(__jsonNode, rhs.__jsonNode);
}

SO_EXPORT bool Json::operator!=(const Json &rhs)
{
	return !operator==(rhs);
}

char* Json::__strDup()
{
	IF_FAIL_RETURN_TAG(__jsonNode, NULL, _E, "Json object not initialized");

	JsonGenerator *jgen = NULL;
	char *output = NULL;

	jgen = json_generator_new();
	IF_FAIL_CATCH(jgen);

	json_generator_set_root(jgen, __jsonNode);
	output = json_generator_to_data(jgen, NULL);
	IF_FAIL_CATCH(output);

	g_object_unref(jgen);
	return output;

CATCH:
	if (jgen) {
		g_object_unref(jgen);
	}

	_E("Memory allocation failed");
	return NULL;
}

SO_EXPORT std::string Json::str()
{
	std::string output;
	char *_s = __strDup();
	IF_FAIL_RETURN(_s, output = EMPTY_JSON_OBJECT);

	output = _s;
	g_free(_s);

	return output;
}

static std::vector<std::string> __tokenize_path(std::string path)
{
	std::vector<std::string> tokens;
	std::size_t begin = 0;
	std::size_t end = path.find(PATH_DELIM, 0);

	while (end != std::string::npos) {
		tokens.push_back(path.substr(begin, end - begin));
		begin = end + 1;
		end = path.find(PATH_DELIM, begin);
	}

	tokens.push_back(path.substr(begin));
	return tokens;
}

static JsonObject* __traverse(JsonNode *jnode, const char *path, bool force)
{
	IF_FAIL_RETURN_TAG(jnode, NULL, _E, "Invalid parameter");

	unsigned int depth = 0;
	std::vector<std::string> pathToken;
	JsonObject *jobj = NULL;
	JsonObject *childObj = NULL;
	JsonNode *childNode = NULL;

	jobj = json_node_get_object(jnode);
	IF_FAIL_RETURN(jobj, NULL);

	if (path)
		pathToken = __tokenize_path(path);

	for (depth = 0; depth < pathToken.size(); depth++) {
		if (!json_object_has_member(jobj, pathToken[depth].c_str())) {
			if (!force) {
				return NULL;
			}
			childObj = json_object_new();
			IF_FAIL_RETURN_TAG(childObj, NULL, _E, "Memory allocation failed");
			json_object_set_object_member(jobj, pathToken[depth].c_str(), childObj);
		}
		childNode = json_object_get_member(jobj, pathToken[depth].c_str());
		IF_FAIL_RETURN(childNode && json_node_get_node_type(childNode) == JSON_NODE_OBJECT, NULL);

		jobj = json_node_get_object(childNode);
		IF_FAIL_RETURN(jobj, NULL);
	}

	return jobj;
}

SO_EXPORT bool Json::set(const char *path, const char *key, Json &val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val.__jsonNode, false, _E, "Invalid parameter");

	JsonObject *jobj = __traverse(__jsonNode, path, true);
	IF_FAIL_RETURN(jobj, false);

	if (json_object_has_member(jobj, key))
		json_object_remove_member(jobj, key);

	json_object_set_member(jobj, key, val.__jsonNode);
	val.__jsonNode = NULL;
	val = Json();

	return true;
}

SO_EXPORT bool Json::set(const char *path, const char *key, int val)
{
	return set(path, key, static_cast<int64_t>(val));
}

SO_EXPORT bool Json::set(const char *path, const char *key, int64_t val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key, false, _E, "Invalid parameter");

	JsonObject *jobj = __traverse(__jsonNode, path, true);
	IF_FAIL_RETURN(jobj, false);

	if (json_object_has_member(jobj, key))
		json_object_remove_member(jobj, key);

	json_object_set_int_member(jobj, key, val);
	return true;
}

SO_EXPORT bool Json::set(const char *path, const char *key, double val)
{
	return set(path, key, __double_to_string(val));
}

SO_EXPORT bool Json::set(const char *path, const char *key, std::string val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key, false, _E, "Invalid parameter");

	JsonObject *jobj = __traverse(__jsonNode, path, true);
	IF_FAIL_RETURN(jobj, false);

	if (json_object_has_member(jobj, key)) {
		json_object_remove_member(jobj, key);
	}

	json_object_set_string_member(jobj, key, val.c_str());
	return true;
}

SO_EXPORT bool Json::set(const char *path, const char *key, GVariant *val)
{
#if JSON_CHECK_VERSION(0, 14, 0)
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val, false, _E, "Invalid parameter");

	const gchar *typeStr = g_variant_get_type_string(val);
	IF_FAIL_RETURN_TAG(typeStr, false, _E, "GVariant manipulation failed");

	json_node_t *node = json_gvariant_serialize(val);
	IF_FAIL_RETURN_TAG(node, false, _E, "GVariant manipulation failed");

	Json gvarJson;
	gvarJson.set(NULL, GVAR_TYPES, std::string(typeStr));
	json_object_set_member(json_node_get_object(gvarJson.__jsonNode), GVAR_VALUES, node);

	return set(path, key, gvarJson);
#else
	_E("Insufficient version of json-glib(" JSON_VERSION_S ")");
	return false;
#endif
}

SO_EXPORT bool Json::get(const char *path, const char *key, Json *val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val, false, _E, "Invalid parameter");

	JsonObject *jobj = NULL;
	JsonNode *node = NULL;

	jobj = __traverse(__jsonNode, path, false);
	IF_FAIL_RETURN(jobj && json_object_has_member(jobj, key), false);

	node = json_object_dup_member(jobj, key);
	IF_FAIL_RETURN_TAG(node, false, _E, "Memory allocation failed");

	if (val->__jsonNode) {
		json_node_free(val->__jsonNode);
	}
	val->__jsonNode = node;

	return true;
}

static JsonNode* __get_value_node(JsonNode *jnode, const char *path, const char *key)
{
	JsonNode *node = NULL;
	JsonObject *jobj = NULL;
	JsonNodeType ntype;

	jobj = __traverse(jnode, path, false);
	IF_FAIL_RETURN(jobj && json_object_has_member(jobj, key), NULL);

	node = json_object_get_member(jobj, key);
	ntype = json_node_get_node_type(node);
	IF_FAIL_RETURN(ntype == JSON_NODE_VALUE, NULL);

	return node;
}

SO_EXPORT bool Json::get(const char *path, const char *key, int *val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val, false, _E, "Invalid parameter");

	int64_t v;

	if (get(path, key, &v)) {
		*val = v;
		return true;
	}

	return false;
}

SO_EXPORT bool Json::get(const char *path, const char *key, int64_t *val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val, false, _E, "Invalid parameter");

	JsonNode *node = __get_value_node(__jsonNode, path, key);
	IF_FAIL_RETURN(node, false);

	GType vtype = json_node_get_value_type(node);
	if (vtype == G_TYPE_INT64) {
		*val = json_node_get_int(node);
	} else if (vtype == G_TYPE_STRING) {
		//TODO: if the string is not a number?
		*val = static_cast<int64_t>(__string_to_double(json_node_get_string(node)));
	} else {
		return false;
	}

	return true;
}

SO_EXPORT bool Json::get(const char *path, const char *key, double *val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val, false, _E, "Invalid parameter");

	JsonNode *node = __get_value_node(__jsonNode, path, key);
	IF_FAIL_RETURN(node, false);

	GType vtype = json_node_get_value_type(node);
	if (vtype == G_TYPE_DOUBLE) {
		*val = json_node_get_double(node);
	} else if (vtype == G_TYPE_INT64) {
		*val = json_node_get_int(node);
	} else if (vtype == G_TYPE_STRING) {
		*val = __string_to_double(json_node_get_string(node));
	} else {
		return false;
	}

	return true;
}

SO_EXPORT bool Json::get(const char *path, const char *key, std::string *val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val, false, _E, "Invalid parameter");

	JsonNode *node = __get_value_node(__jsonNode, path, key);
	IF_FAIL_RETURN(node, false);

	GType vtype = json_node_get_value_type(node);
	IF_FAIL_RETURN(vtype == G_TYPE_STRING, false);

	const char *str_val = json_node_get_string(node);
	IF_FAIL_RETURN_TAG(str_val, false, _E, "Getting string failed");

	*val = str_val;
	return true;
}

SO_EXPORT bool Json::get(const char *path, const char *key, GVariant **val)
{
#if JSON_CHECK_VERSION(0, 14, 0)
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val, false, _E, "Invalid parameter");

	bool ret;
	Json gvarJson;
	ret = get(path, key, &gvarJson);
	IF_FAIL_RETURN(ret, false);

	std::string gvarTypes;
	ret = gvarJson.get(NULL, GVAR_TYPES, &gvarTypes);
	IF_FAIL_RETURN(ret, false);

	Json gvarValues;
	ret = gvarJson.get(NULL, GVAR_VALUES, &gvarValues);
	IF_FAIL_RETURN(ret, false);

	GError *gerr = NULL;
	*val = json_gvariant_deserialize(gvarValues.__jsonNode, gvarTypes.c_str(), &gerr);
	HANDLE_GERROR(gerr);
	IF_FAIL_RETURN(*val, false);

	return true;
#else
	_E("Insufficient version of json-glib(" JSON_VERSION_S ")");
	*val = NULL;
	return false;
#endif
}

static JsonArray* __get_array(JsonNode *jnode, const char *path, const char *key, bool force)
{
	JsonNode *node = NULL;
	JsonArray *arr = NULL;
	JsonObject *jobj = NULL;

	jobj = __traverse(jnode, path, force);
	IF_FAIL_RETURN(jobj, NULL);

	if (!json_object_has_member(jobj, key)) {
		if (force) {
			arr = json_array_new();
			IF_FAIL_RETURN_TAG(arr, NULL, _E, "Memory allocation failed");
			json_object_set_array_member(jobj, key, arr);
		} else {
			return NULL;
		}
	}
	node = json_object_get_member(jobj, key);
	IF_FAIL_RETURN_TAG(node && json_node_get_node_type(node) == JSON_NODE_ARRAY,
			NULL, _W, "Type mismatched: %s", key);

	return json_node_get_array(node);
}

SO_EXPORT int Json::getSize(const char *path, const char *key)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, -1, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key, -1, _E, "Invalid parameter");

	JsonArray *jarr = __get_array(__jsonNode, path, key, false);
	IF_FAIL_RETURN_TAG(jarr, -1, _D, "Mismatched data type");

	return json_array_get_length(jarr);
}

SO_EXPORT bool Json::append(const char *path, const char *key, Json &val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val.__jsonNode, false, _E, "Invalid parameter");

	JsonArray *arr = __get_array(__jsonNode, path, key, true);
	IF_FAIL_RETURN(arr, false);

	json_array_add_element(arr, val.__jsonNode);
	val.__jsonNode = NULL;
	val = Json();

	return true;
}

SO_EXPORT bool Json::append(const char *path, const char *key, int val)
{
	return append(path, key, static_cast<int64_t>(val));
}

SO_EXPORT bool Json::append(const char *path, const char *key, int64_t val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key, false, _E, "Invalid parameter");

	JsonArray *arr = __get_array(__jsonNode, path, key, true);
	IF_FAIL_RETURN(arr, false);

	json_array_add_int_element(arr, val);
	return true;
}

SO_EXPORT bool Json::append(const char *path, const char *key, double val)
{
	return append(path, key, __double_to_string(val));
}

SO_EXPORT bool Json::append(const char *path, const char *key, std::string val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key, false, _E, "Invalid parameter");

	JsonArray *arr = __get_array(__jsonNode, path, key, true);
	IF_FAIL_RETURN(arr, false);

	json_array_add_string_element(arr, val.c_str());
	return true;
}

static JsonNode* __get_array_elem(JsonNode *jnode, const char *path, const char *key, int index)
{
	JsonArray *jarr = __get_array(jnode, path, key, false);
	IF_FAIL_RETURN_TAG(jarr, NULL, _W, "Mismatched data type");

	int size = json_array_get_length(jarr);
	IF_FAIL_RETURN(size > index, NULL);

	JsonNode *node = json_array_get_element(jarr, index);
	IF_FAIL_RETURN_TAG(node, NULL, _E, "Failed to get an array element");

	return node;
}

SO_EXPORT bool Json::setAt(const char *path, const char *key, int index, Json &val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(val.__jsonNode && key && index >= 0, false, _E, "Invalid parameter");

	JsonNode *node = __get_array_elem(__jsonNode, path, key, index);
	IF_FAIL_RETURN_TAG(node, false, _W, "Out of range");

	JsonObject *obj = json_node_get_object(val.__jsonNode);
	IF_FAIL_RETURN_TAG(obj, false, _E, "Getting object failed");

	json_node_set_object(node, obj);
	json_node_free(val.__jsonNode);
	val.__jsonNode = NULL;
	val = Json();

	return true;
}

SO_EXPORT bool Json::setAt(const char *path, const char *key, int index, int val)
{
	return setAt(path, key, index, static_cast<int64_t>(val));
}

SO_EXPORT bool Json::setAt(const char *path, const char *key, int index, int64_t val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && index >= 0, false, _E, "Invalid parameter");

	JsonNode *node = __get_array_elem(__jsonNode, path, key, index);
	IF_FAIL_RETURN_TAG(node, false, _W, "Out of range");
	IF_FAIL_RETURN_TAG(json_node_get_node_type(node) == JSON_NODE_VALUE, false, _E, "Type mismatched: %s[%d]", key, index);

	json_node_set_int(node, val);
	return true;
}

SO_EXPORT bool Json::setAt(const char *path, const char *key, int index, double val)
{
	return setAt(path, key, index, __double_to_string(val));
}

SO_EXPORT bool Json::setAt(const char *path, const char *key, int index, std::string val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && index >= 0, false, _E, "Invalid parameter");

	JsonNode *node = __get_array_elem(__jsonNode, path, key, index);
	IF_FAIL_RETURN_TAG(node, false, _W, "Out of range");
	IF_FAIL_RETURN_TAG(json_node_get_node_type(node) == JSON_NODE_VALUE, false, _E, "Type mismatched: %s[%d]", key, index);

	json_node_set_string(node, val.c_str());
	return true;
}

SO_EXPORT bool Json::getAt(const char *path, const char *key, int index, Json *val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val && index >= 0, false, _E, "Invalid parameter");

	JsonNode *node = __get_array_elem(__jsonNode, path, key, index);
	IF_FAIL_RETURN(node, false);

	JsonNode *nodeCopy = json_node_copy(node);
	IF_FAIL_RETURN_TAG(nodeCopy, false, _E, "Memory allocation failed");

	if (val->__jsonNode) {
		json_node_free(val->__jsonNode);
	}
	val->__jsonNode = nodeCopy;

	return true;
}

SO_EXPORT bool Json::getAt(const char *path, const char *key, int index, int *val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val && index >= 0, false, _E, "Invalid parameter");

	int64_t v;
	if (getAt(path, key, index, &v)) {
		*val = v;
		return true;
	}

	return false;
}

SO_EXPORT bool Json::getAt(const char *path, const char *key, int index, int64_t *val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val && index >= 0, false, _E, "Invalid parameter");

	JsonNode *node = __get_array_elem(__jsonNode, path, key, index);
	IF_FAIL_RETURN(node, false);

	JsonNodeType ntype = json_node_get_node_type(node);
	IF_FAIL_RETURN_TAG(ntype == JSON_NODE_VALUE, false, _E, "Type mismatched: %s", key);

	GType vtype = json_node_get_value_type(node);
	if (vtype == G_TYPE_INT64) {
		*val = json_node_get_int(node);
	} else if (vtype == G_TYPE_STRING) {
		*val = static_cast<int64_t>(__string_to_double(json_node_get_string(node)));
	} else {
		_E("Type mismatched: %s", key);
		return false;
	}

	return true;
}

SO_EXPORT bool Json::getAt(const char *path, const char *key, int index, double *val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val && index >= 0, false, _E, "Invalid parameter");

	JsonNode *node = __get_array_elem(__jsonNode, path, key, index);
	IF_FAIL_RETURN(node, false);

	JsonNodeType ntype = json_node_get_node_type(node);
	IF_FAIL_RETURN_TAG(ntype == JSON_NODE_VALUE, false, _E, "Type mismatched: %s", key);

	GType vtype = json_node_get_value_type(node);
	if (vtype == G_TYPE_DOUBLE) {
		*val = json_node_get_double(node);
	} else if (vtype == G_TYPE_INT64) {
		*val = json_node_get_int(node);
	} else if (vtype == G_TYPE_STRING) {
		*val = __string_to_double(json_node_get_string(node));
	} else {
		_E("Type mismatched: %s", key);
		return false;
	}

	return true;
}

SO_EXPORT bool Json::getAt(const char *path, const char *key, int index, std::string *val)
{
	IF_FAIL_RETURN_TAG(this->__jsonNode, false, _E, "Json object not initialized");
	IF_FAIL_RETURN_TAG(key && val && index >= 0, false, _E, "Invalid parameter");

	JsonNode *node = __get_array_elem(__jsonNode, path, key, index);
	IF_FAIL_RETURN(node, false);

	JsonNodeType ntype = json_node_get_node_type(node);
	IF_FAIL_RETURN_TAG(ntype == JSON_NODE_VALUE, false, _E, "Type mismatched: %s", key);

	GType vtype = json_node_get_value_type(node);
	IF_FAIL_RETURN_TAG(vtype == G_TYPE_STRING, false, _E, "Type mismatched: %s", key);

	const char *str_val = json_node_get_string(node);
	IF_FAIL_RETURN_TAG(str_val, false, _E, "Getting string failed");

	*val = str_val;
	return true;
}

bool Json::__getMembers(json_node_t *node, std::list<std::string> &list)
{
	IF_FAIL_RETURN(node, false);
	list.clear();

	JsonObject *jobj = json_node_get_object(node);
	IF_FAIL_RETURN_TAG(jobj, false, _E, "Getting Json object failed");

	GList *members = json_object_get_members(jobj);
	IF_FAIL_RETURN(members, true);

	for (GList *it = g_list_first(members); it; it = g_list_next(it)) {
		const char *key = static_cast<const char*>(it->data);
		if (!key) {
			list.clear();
			g_list_free(members);
			_E("Member list extraction failed");
			return false;
		}

		list.push_back(key);
	}

	g_list_free(members);
	return true;
}

SO_EXPORT bool Json::getKeys(std::list<std::string>* list)
{
	IF_FAIL_RETURN_TAG(list, false, _E, "Invalid parameter");
	return __getMembers(__jsonNode, *list);
}

bool Json::__nodeEq(json_node_t *lhs, json_node_t *rhs)
{
	IF_FAIL_RETURN(lhs && rhs, false);

	JsonNodeType ltype = json_node_get_node_type(lhs);
	JsonNodeType rtype = json_node_get_node_type(rhs);
	IF_FAIL_RETURN(ltype == rtype, false);

	switch (ltype) {
	case JSON_NODE_VALUE:
		IF_FAIL_RETURN(__valueEq(lhs, rhs), false);
		break;
	case JSON_NODE_OBJECT:
		IF_FAIL_RETURN(__objectEq(lhs, rhs), false);
		break;
	case JSON_NODE_ARRAY:
		IF_FAIL_RETURN(__arrayEq(lhs, rhs), false);
		break;
	default:
		_W("Unsupported type");
		return false;
	}

	return true;
}

bool Json::__valueEq(json_node_t *lhs, json_node_t *rhs)
{
	GType ltype = json_node_get_value_type(lhs);
	GType rtype = json_node_get_value_type(rhs);
	IF_FAIL_RETURN(ltype == rtype, false);

	switch (ltype) {
	case G_TYPE_INT64:
		return json_node_get_int(lhs) == json_node_get_int(rhs);
	case G_TYPE_DOUBLE:
		return json_node_get_double(lhs) == json_node_get_double(rhs);
	case G_TYPE_STRING:
		return STR_EQ(json_node_get_string(lhs), json_node_get_string(rhs));
	default:
		_W("Unsupported type");
		return false;
	}
}

bool Json::__objectEq(json_node_t *lhs, json_node_t *rhs)
{
	std::list<std::string> lm, rm;
	IF_FAIL_RETURN(__getMembers(lhs, lm), false);
	IF_FAIL_RETURN(__getMembers(rhs, rm), false);
	IF_FAIL_RETURN(lm.size() == rm.size(), false);

	lm.sort();
	rm.sort();

	std::list<std::string>::iterator lit, rit;
	lit = lm.begin();
	rit = rm.begin();

	while (lit != lm.end()) {
		IF_FAIL_RETURN(*lit == *rit, false);

		json_node_t *lhsChild = json_object_get_member(json_node_get_object(lhs), (*lit).c_str());
		json_node_t *rhsChild = json_object_get_member(json_node_get_object(rhs), (*rit).c_str());
		IF_FAIL_RETURN(__nodeEq(lhsChild, rhsChild), false);

		++lit;
		++rit;
	}

	return true;
}

bool Json::__arrayEq(json_node_t *lhs, json_node_t *rhs)
{
	JsonArray *larr = json_node_get_array(lhs);
	JsonArray *rarr = json_node_get_array(rhs);

	int size = json_array_get_length(larr);
	IF_FAIL_RETURN(size == static_cast<int>(json_array_get_length(rarr)), false);

	for (int i=0; i<size; ++i) {
		json_node_t *lhsChild = json_array_get_element(larr, i);
		json_node_t *rhsChild = json_array_get_element(rarr, i);
		IF_FAIL_RETURN(__nodeEq(lhsChild, rhsChild), false);
	}

	return true;
}
