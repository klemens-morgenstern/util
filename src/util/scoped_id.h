/*
 * scoped_id.h
 *
 *  Created on: 23.03.2015
 *      Author: Klemens
 */

#ifndef SCOPED_ID_H_
#define SCOPED_ID_H_

#include <string>
#include <sstream>
#include <vector>
#include <boost/mpl/string.hpp>


namespace util
{

template<int Seperator = '.'>
struct scoped_identifier : public std::vector<std::string>
{
	using _st_type 	= boost::mpl::string<Seperator>;
	using _c_id		= boost::mpl::c_str<_st_type>;
	using _id_size	= boost::mpl::size<_st_type>;
	using father = std::vector<std::string>;
	static father make_splitted(const std::string& str);
public:
	static constexpr size_t sz = _id_size::value;
	static constexpr const char* const sep = _c_id::value;

	scoped_identifier(const char* c_str) : scoped_identifier(std::string(c_str)) {};
	scoped_identifier(const std::string & str) 	: father(make_splitted(str)) {};
	scoped_identifier(const std::vector<std::string> &id) : father(id) {};
	scoped_identifier(std::vector<std::string> &&id) : father(id) {};

	scoped_identifier() = default;

	scoped_identifier(const scoped_identifier&) noexcept = default;
	scoped_identifier( scoped_identifier&&)  	noexcept = default;
	scoped_identifier &operator=(const scoped_identifier&) 	noexcept = default;
	scoped_identifier &operator=( scoped_identifier&&) 		noexcept = default;

	using father::father;

	std::string to_string() const
	{
		std::stringstream str;
		for (auto itr = begin(); itr != end(); itr++)
		{
			if (itr != begin())
				str << sep;
			str << *itr;

		}
		return str.str();
	}

};

template<int Seperator>
inline std::vector<std::string> scoped_identifier<Seperator>::make_splitted(const std::string& str)
{
	size_t cnt = 0;
	std::vector<std::string> ret;

	auto sub = str.begin();
	for (auto itr = str.begin(); itr != str.end(); itr++)
	{

		if ( *(itr) == sep[cnt])
			cnt++;
		else if (*itr == sep[0])
		{
			itr -= (cnt -1);
			cnt = 1;
		}
		else
		{
			itr -= cnt;
			cnt = 0;
		}

		if (cnt == sz)
		{

			ret.emplace_back(sub, itr - cnt + 1);
			cnt = 0;
			sub = itr+1;
		}

	}
	ret.emplace_back(sub, str.end());
	return ret;
}



}


#endif /* SCOPED_ID_H_ */
