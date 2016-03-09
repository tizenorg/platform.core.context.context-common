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

#ifndef _CONTEXT_JSON_H_
#define _CONTEXT_JSON_H_

#include <sys/types.h>
#include <glib.h>
#include <string>
#include <list>
#include <types_internal.h>

#define _J(cmt, jobj) \
do { \
	_SD("%s: %s", (cmt), jobj.str().c_str()); \
} while(0)

#define EMPTY_JSON_OBJECT	"{}"

namespace ctx {

	class Json {
	public:
		Json();
		Json(const char *s);
		Json(const std::string &s);

		/* This Json(const Json &j) only copies the reference to the underlying Json node.
		 * Therefore, changes applied to a Json object affect the other.
		 * If you need to create a 'real' copy of a Json, which can be manipulated separately,
		 * utilize the str() function, e.g., ctx::Json copy(original.str());
		 */
		Json(const Json &j);

		~Json();

		Json& operator=(const char *s);
		Json& operator=(const std::string &s);

		/* This operator=(const Json &j) only copies the reference to the underlying Json node.
		 * Therefore, changes applied to a Json object affect the other.
		 * If you need to create a 'real' copy of a Json, which can be manipulated separately,
		 * utilize the str() function, e.g., ctx::Json copy = original.str();
		 */
		Json& operator=(const Json &j);

		bool operator==(const Json &rhs);
		bool operator!=(const Json &rhs);

		std::string str();

		bool getKeys(std::list<std::string> *list);
		bool valid();

		bool set(const char *path, const char *key, Json &val);
		bool set(const char *path, const char *key, int val);
		bool set(const char *path, const char *key, int64_t val);
		bool set(const char *path, const char *key, double val);
		bool set(const char *path, const char *key, std::string val);
		bool set(const char *path, const char *key, GVariant *val);

		bool get(const char *path, const char *key, Json *val);
		bool get(const char *path, const char *key, int *val);
		bool get(const char *path, const char *key, int64_t *val);
		bool get(const char *path, const char *key, double *val);
		bool get(const char *path, const char *key, std::string *val);
		bool get(const char *path, const char *key, GVariant **val);

		/* Array operations */
		int getSize(const char *path, const char *key);

		bool append(const char *path, const char *key, Json &val);
		bool append(const char *path, const char *key, int val);
		bool append(const char *path, const char *key, int64_t val);
		bool append(const char *path, const char *key, double val);
		bool append(const char *path, const char *key, std::string val);

		bool setAt(const char *path, const char *key, int index, Json &val);
		bool setAt(const char *path, const char *key, int index, int val);
		bool setAt(const char *path, const char *key, int index, int64_t val);
		bool setAt(const char *path, const char *key, int index, double val);
		bool setAt(const char *path, const char *key, int index, std::string val);

		bool getAt(const char *path, const char *key, int index, Json *val);
		bool getAt(const char *path, const char *key, int index, int *val);
		bool getAt(const char *path, const char *key, int index, int64_t *val);
		bool getAt(const char *path, const char *key, int index, double *val);
		bool getAt(const char *path, const char *key, int index, std::string *val);

	private:
		typedef struct _JsonNode json_node_t;
		json_node_t *__jsonNode;

		void __parse(const char *s);
		void __release();
		char* __strDup();

		/* For json vs json comparison */
		bool __getMembers(json_node_t *node, std::list<std::string> &list);
		bool __nodeEq(json_node_t *lhs, json_node_t *rhs);
		bool __valueEq(json_node_t *lhs, json_node_t *rhs);
		bool __objectEq(json_node_t *lhs, json_node_t *rhs);
		bool __arrayEq(json_node_t *lhs, json_node_t *rhs);
	};

}	/* namespace ctx */

#endif // _CONTEXT_JSON_H_
