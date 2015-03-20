/*
 * lambda_visitor.h
 *
 *  Created on: 03.07.2014
 *      Author: Klemens
 */

#ifndef LAMBDA_VISITOR_H_
#define LAMBDA_VISITOR_H_

#include <boost/variant/static_visitor.hpp>

#include <type_traits>

namespace util
{

template<typename lambda>
struct parameter_deduction
{
};

template<typename lambda, typename Arg, typename Ret>
struct parameter_deduction<Ret(lambda::*)(Arg) const>
{
	typedef Arg type;
	typedef Ret return_type;
};

template<typename Lambda>
struct deduct_parameter
{
	using type 		  = typename parameter_deduction<decltype(&Lambda::operator())>::type;
	using return_type = typename parameter_deduction<decltype(&Lambda::operator())>::return_type;
};

template<typename Return_Type, typename Lambda>
class void_helper
{
	Lambda lambda;
public:
	void_helper(Lambda && lambda) : lambda(std::forward<Lambda>(lambda)) {};
	using type 		= typename deduct_parameter<Lambda>::type;
	using return_type	= typename deduct_parameter<Lambda>::return_type;

	return_type operator()(type & val) const
	{
		return lambda(val);
	}
};

template<typename Lambda>
class void_helper<void, Lambda>
{
	Lambda lambda;
public:
	void_helper(Lambda && lambda) : lambda(std::forward<Lambda>(lambda)) {};
	using type 		= typename deduct_parameter<Lambda>::type;
	using return_type	= typename deduct_parameter<Lambda>::return_type;

	void operator()(type & val) const
	{
		lambda(val);
	}
};

template<typename ...Args>
class lambda_visitor_helper
{
};

template<typename Lambda>
class lambda_visitor_helper<Lambda> : public void_helper<typename deduct_parameter<Lambda>::return_type, Lambda>
{
	using father = void_helper<typename deduct_parameter<Lambda>::return_type, Lambda>;
public:
	using return_type = typename father::return_type;
	using father::operator();

	lambda_visitor_helper(Lambda && lambda) : father(std::forward<Lambda>(lambda)) {};
};

template<typename Lambda, typename ... Args>
class lambda_visitor_helper<Lambda, Args...> : public lambda_visitor_helper<Args...>, public void_helper<typename deduct_parameter<Lambda>::return_type, Lambda>
{
	using father = void_helper<typename deduct_parameter<Lambda>::return_type, Lambda>;
	static_assert(std::is_same<typename father::return_type, typename lambda_visitor_helper<Args...>::return_type>::value, "return types must have the same type");
public:
	using lambda_visitor_helper<Args...>::operator();
	using return_type = typename father::return_type;
	using father::operator();

	template<typename ... l_Args>
	lambda_visitor_helper(Lambda && lambda, l_Args && ...ls) :
			lambda_visitor_helper<Args...>(std::forward<l_Args>(ls)...), father(std::forward<Lambda>(lambda)) {};
};

template<typename ...Args>
struct lambda_visitor : public lambda_visitor_helper<Args...>, public boost::static_visitor<typename lambda_visitor_helper<Args...>::return_type>
{
	using lambda_visitor_helper<Args...>::operator();
	lambda_visitor(Args&&...args) : lambda_visitor_helper<Args...>(std::forward<Args>(args)...) {};
};

template<typename ...Args>
lambda_visitor<Args...> make_lambda_visitor(Args && ...args) {return lambda_visitor<Args...>(std::forward<Args>(args)...);}

}



#endif /* LAMBDA_VISITOR_H_ */
