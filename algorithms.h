/*
 * algorithms.h
 *
 *  Created on: 02.07.2014
 *      Author: Klemens
 */

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#include <vector>
#include <string>
#include <algorithm>

namespace util
{

std::vector<std::string> splice_string(const std::string & str, const std::string & sep);
template<typename T>
bool contains(const std::vector<T*> &v, T* t)
{
	return std::find(v.begin(), v.end(), t) != v.end();
}

} /* namespace qlog */

#endif /* ALGORITHMS_H_ */
