#ifndef UTIL_TO_STRING_H_
#define UTIL_TO_STRING_H_

#include <vector>
#include <string>
#include <logic/lang_code.h>

namespace std {

template<typename T> std::string to_string(const T& t) {return t.to_string();}
template<typename T> std::string to_string(const std::vector<T> & vec)
{
	std::string ret;
	for (auto &v : vec)
		ret += to_string(v);
	return ret;
}

template<typename T> std::string to_string(const T& t, qlog::lang_code l) {return t.to_string(l);}
template<typename T> std::string to_string(const std::vector<T> & vec, qlog::lang_code l)
{
	std::string ret;
	for (auto &v : vec)
		ret += to_string(v, l);
	return ret;
}

}

#endif
