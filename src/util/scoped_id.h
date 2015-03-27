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

template<typename Itr1, typename Itr2>
bool diff_equal(Itr1 beg1, Itr1 end1, Itr2 beg2, Itr2 end2)
{
	for (auto itr1 = beg1, itr2 = beg2; (itr1 != end1) && (itr2 != end2); itr1++, itr2++)
	{
		if (*itr1 != *itr2)
			return false;
	}

	return true;
}


template<int Seperator = '.', bool Inverted = true> //Inverted is right to left.
struct scoped_identifier : public std::vector<std::string>
{
	using _st_type 	= boost::mpl::string<Seperator>;
	using _c_id		= boost::mpl::c_str<_st_type>;
	using _id_size	= boost::mpl::size<_st_type>;
	using father = std::vector<std::string>;
	static father make_splitted(const std::string& str);

	bool _equal_impl(const scoped_identifier& scp, std::true_type ) const {return diff_equal(scp.rbegin(), scp.rend(), rbegin(), rend());}
	bool _equal_impl(const scoped_identifier& scp, std::false_type) const {return diff_equal(scp.begin(), scp.end(), begin(), end());}
	int _compare_impl(const scoped_identifier& scp, std::true_type ) const;
	int _compare_impl(const scoped_identifier& scp, std::false_type) const;

//	void _add_impl(const std::string& str, std::true_type) {push_front(str);}
//	void _add_impl(const std::string& str, std::false_type) {push_back(str);}

	void _add_impl(const scoped_identifier& scp, std::true_type) {insert(begin(), scp.begin(), scp.end());}
	void _add_impl(const scoped_identifier& scp, std::false_type){insert(end(), scp.begin(), scp.end());}


public:
	static constexpr size_t sz = _id_size::value;
	static constexpr const char* const sep = _c_id::value;

	scoped_identifier(const char* c_str) : scoped_identifier(std::string(c_str)) {};
	scoped_identifier(const std::string & str) 	: father(make_splitted(str)) {};
	scoped_identifier(const std::vector<std::string> &id) : father(id) {};
	scoped_identifier(std::vector<std::string> &&id) : father(id) {};

	scoped_identifier() = default;

	scoped_identifier(const scoped_identifier&) = default;
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

	bool equal(const scoped_identifier& scp) const {return _equal_impl(scp, std::integral_constant<bool, Inverted>());}
	int compare(const scoped_identifier& scp) const {return _compare_impl(scp, std::integral_constant<bool, Inverted>());}
	void push_front	(const std::string& str) {insert(begin(), str);}
	void pop_front	() {erase (begin());}


	void add(const std::string		& str) {_add_impl(make_splitted(str), std::integral_constant<bool, Inverted>());}
	void add(const scoped_identifier& scp) {_add_impl(scp, std::integral_constant<bool, Inverted>());}


	scoped_identifier operator+(const scoped_identifier& scp) const
	{
		auto ret = *this;
		ret.add(scp);
		return ret;
	}
	scoped_identifier operator+(const std::string& scp) const
	{
		auto ret = *this;
		ret.add(scp);
		return ret;
	}
	scoped_identifier &operator+=(const scoped_identifier& scp)
	{
		add(scp);
		return *this;
	}
	scoped_identifier &operator+=(const std::string& scp)
	{
		add(scp);
		return *this;
	}

	bool operator==(const scoped_identifier& scp) const {return  equal(scp);}
	bool operator!=(const scoped_identifier& scp) const {return !equal(scp);}
	bool operator< (const scoped_identifier& scp) const {return compare(scp) <  0;}
	bool operator> (const scoped_identifier& scp) const {return compare(scp) >  0;}
	bool operator<=(const scoped_identifier& scp) const {return compare(scp) <= 0;}
	bool operator>=(const scoped_identifier& scp) const {return compare(scp) >= 0;}

	bool operator==(const std::string& scp) const {return   equal(make_splitted(scp));}
	bool operator!=(const std::string& scp) const {return  !equal(make_splitted(scp));}
	bool operator< (const std::string& scp) const {return compare(make_splitted(scp)) <  0;}
	bool operator> (const std::string& scp) const {return compare(make_splitted(scp)) >  0;}
	bool operator<=(const std::string& scp) const {return compare(make_splitted(scp)) <= 0;}
	bool operator>=(const std::string& scp) const {return compare(make_splitted(scp)) >= 0;}


	scoped_identifier& operator--() 	{pop_back();  return *this;}
	scoped_identifier& operator--(int) 	{pop_front(); return *this;}

};

template<int Seperator, bool Inverted>
inline std::vector<std::string> scoped_identifier<Seperator, Inverted>::make_splitted(const std::string& str)
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


template<int Seperator, bool Inverted>
inline int scoped_identifier<Seperator, Inverted>::_compare_impl(const scoped_identifier<Seperator, Inverted>& scp, std::true_type integral_constant) const
{
	int ret = 0;
	for (auto it1 = rbegin(), it2 = scp.rbegin();
		 (it1 != rend()) && (it2 != scp.rend()) && (ret == 0);
			it1++, it2++)
	{
		ret = it1->compare(*it2);
	}

	return ret;
}

template<int Seperator, bool Inverted>
inline int scoped_identifier<Seperator, Inverted>::_compare_impl(const scoped_identifier<Seperator, Inverted>& scp, std::false_type integral_constant) const
{
	int ret = 0;
	for (auto it1 = begin(), it2 = scp.begin();
		 (it1 != end()) && (it2 != scp.end()) && (ret == 0);
			it1++, it2++)
	{
		ret = it1->compare(*it2);
	}

	return ret;
}

}
namespace std
{

   template<int Seperator, bool Inverted>
   struct hash<util::scoped_identifier<Seperator, Inverted>>
   {
       typedef util::scoped_identifier<Seperator, Inverted> argument_type;
       typedef std::size_t result_type;

       result_type operator()(argument_type const& s) const
       {
           result_type val;

           std::hash<std::string> hsh;

           for (auto & v : s)
           {
        	   val = hsh(v) ^ (val << 1);
           }

           return val;
       }
   };
}



#endif /* SCOPED_ID_H_ */
