/*
 * algorithms.cpp
 *
 *  Created on: 02.07.2014
 *      Author: Klemens
 */

#include "algorithms.h"

using namespace std;

namespace util
{
vector<string> splice_string(const string & str, const string & sep)
{
	if (str.find(sep) == std::string::npos) return {str};
	vector<string> v;


	auto itr = str.begin();
	auto itr_sep = sep.begin();

	auto candidate = str.begin();
	auto sub_str_beg = str.begin();

	while (itr != str.end())
	{
		if (*itr == *itr_sep) //found first element
		{
			if (itr_sep == sep.begin())
			{
				candidate = itr;
			}

			itr_sep++;
			if (itr_sep == sep.end())
			{
				v.emplace_back(sub_str_beg, candidate);
				sub_str_beg = itr;
				advance(sub_str_beg, sep.size()-1);
				itr_sep = sep.begin();
 			}
		}
		else
			itr_sep = sep.begin();

		itr++;
	}
	advance(candidate,sep.size());
	v.emplace_back(candidate, str.end());
	return v;
}
} /* namespace qlog */
