/*
 * ref_vector.h
 *
 *  Created on: 03.10.2014
 *      Author: Klemens
 */

#ifndef REF_VECTOR_H_
#define REF_VECTOR_H_

#include <vector>
#include <boost/iterator/transform_iterator.hpp>

namespace util {

template<typename T, class Allocator = std::allocator<T*>>
class ref_vector
{
	std::vector<T*, Allocator> _store;
	constexpr static T& 	  _to_ref(T* in) {return *in;};
	constexpr static const T& _to_const_ref(const T* in) {return *in;};

	using _to_ref_t 		 = T&(*)(T*);
	using _to_const_ref_t = const T&(*)(const T*);

	constexpr static T* 	  _to_ptr(T& in) 			{return &in;};
	constexpr static const T* _to_const_ptr(const T& in) {return &in;};

	using _to_ptr_t 		 = T*(*)(T&);
	using _to_const_ptr_t = const T*(*)(const T&);
public:

	using value_type = T;
	using allocator_type = Allocator;
	using size_type	= typename std::vector<T*, Allocator>::size_type;
	using difference_type	= typename std::vector<T*, Allocator>::size_type;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator				 = 	boost::transform_iterator<_to_ref_t, 	   typename std::vector<T*, Allocator>::iterator>;
	using const_iterator		 = 	boost::transform_iterator<_to_const_ref_t, typename std::vector<T*, Allocator>::const_iterator>;
	using reverse_iterator		 = 	boost::transform_iterator<_to_ref_t, 	   typename std::vector<T*, Allocator>::reverse_iterator>;
	using const_reverse_iterator = 	boost::transform_iterator<_to_const_ref_t, typename std::vector<T*, Allocator>::const_reverse_iterator>;

	ref_vector() = default;

	explicit ref_vector( size_type count, value_type& value, const Allocator& alloc = Allocator()) : _store(count, &value, alloc) {};
	template<class InputIt>
	ref_vector(InputIt first, InputIt last, const Allocator& alloc = Allocator() ) :
			_store(boost::make_transform_iterator(first, &ref_vector::_to_ptr),
				   boost::make_transform_iterator(last,  &ref_vector::_to_ptr), alloc) {}

	ref_vector( const ref_vector& other ) = default;
	ref_vector( const ref_vector& other, const Allocator& alloc ) : _store(other._store, alloc) {};
	ref_vector( ref_vector&& other ) = default;
	ref_vector( ref_vector&& other, const Allocator& alloc ) : _store(std::move(other._store), alloc) {};


	ref_vector( std::initializer_list<T&> init, const Allocator& alloc = Allocator() ) :
		_store(init.begin(), init.end(), alloc) {}

	~ref_vector() = default;


	ref_vector& operator=( const ref_vector& other ) = default;
	ref_vector& operator=( ref_vector&& other ) = default;

	ref_vector& operator=( std::initializer_list<T&> ilist )
	{
		_store = std::vector<T*, Allocator>(boost::make_transform_iterator(ilist.begin(), &_to_ptr),
											boost::make_transform_iterator(ilist.end(),  &_to_ptr));
		return *this;
	}

	void assign( size_type count,  T& value ) {_store.assign(count, &value);}

	template< class InputIt >
	void assign( InputIt first, InputIt last )
	{
		_store.assign(boost::make_transform_iterator(first, &_to_ptr),
					  boost::make_transform_iterator(last,  &_to_ptr));
	}
	void assign( std::initializer_list<T> ilist )
	{
		_store.assign(boost::make_transform_iterator(ilist.begin(), &_to_ptr),
					  boost::make_transform_iterator(ilist.end(),   &_to_ptr));
	}

	allocator_type get_allocator() const {return _store.get_allocator();}

	reference       at( size_type pos ) 	  {return *_store.at(pos);}
	const_reference at( size_type pos ) const {return *_store.at(pos);}

	reference       		  operator[]( size_type pos ) 		{return *_store[pos];}
	constexpr const_reference operator[]( size_type pos ) const {return *_store[pos];}

		  reference front()		  {return *_store.front(); }
	const_reference front() const {return *_store.front(); }

		  reference back()		 {return *_store.back();}
	const_reference back() const {return *_store.back();}

		  T** data()	   noexcept {return _store.data();}
	const T** data() const noexcept {return _store.data();}

	iterator 		begin() 	  noexcept {return boost::make_transform_iterator(_store.begin(), &_to_ref);}
	const_iterator  begin() const noexcept {return boost::make_transform_iterator(_store.begin(), &_to_const_ref);}
	const_iterator cbegin() const noexcept {return boost::make_transform_iterator(_store.begin(), &_to_const_ref);}

	iterator 		end() 	  	noexcept {return boost::make_transform_iterator(_store.end(), &_to_ref);}
	const_iterator  end() const noexcept {return boost::make_transform_iterator(_store.end(), &_to_const_ref);}
	const_iterator cend() const noexcept {return boost::make_transform_iterator(_store.end(), &_to_const_ref);}

	iterator 		rbegin() 	   noexcept  {return boost::make_transform_iterator(_store.rbegin(), &_to_ref);}
	const_iterator  rbegin() const noexcept {return boost::make_transform_iterator(_store.rbegin(), &_to_const_ref);}
	const_iterator crbegin() const noexcept {return boost::make_transform_iterator(_store.rbegin(), &_to_const_ref);}

	iterator 		rend() 	     noexcept {return boost::make_transform_iterator(_store.rend(), &_to_ref);}
	const_iterator  rend() const noexcept {return boost::make_transform_iterator(_store.rend(), &_to_const_ref);}
	const_iterator crend() const noexcept {return boost::make_transform_iterator(_store.rend(), &_to_const_ref);}

	bool 	  empty() 					 const noexcept {return _store.empty();}
	size_type size() 					 const noexcept {return _store.size();}
	size_type max_size() 				 const noexcept {return _store.max_size();}
	void 	  reserve(size_type new_cap) 				{_store.reserve(new_cap);}
	size_type capacity() 				 const noexcept {return _store.capacity();}
	void 	  shrink_to_fit()							{_store.shrink_to_fit();}

	void clear() noexcept {_store.clear();}

	iterator insert( const_iterator pos, T& value ) 				 {return boost::make_transform_iterator(_store.insert(pos.base(), value), &_to_ptr);}
	iterator insert( const_iterator pos, size_type count, T& value ) {return boost::make_transform_iterator(_store.insert(pos.base(), count, value), &_to_ptr);}
	template< class InputIt >
	iterator insert( const_iterator pos, InputIt first, InputIt last )
	{
		return boost::make_transform_iterator(
				_store.insert(pos.base(),
							boost::make_transform_iterator(first, &_to_ptr ),
							boost::make_transform_iterator(last, &_to_ptr )
						),&_to_ref);
	}
	iterator insert( const_iterator pos, std::initializer_list<T> ilist )
	{
		return boost::make_transform_iterator(
				_store.insert(pos.base(),
							boost::make_transform_iterator(ilist.first(), &_to_ptr ),
							boost::make_transform_iterator(ilist.end(), &_to_ptr )
						),&_to_ref);
	}

	iterator emplace( const_iterator pos, T& value ) {return _store.emplace(pos.base(), value);}

	iterator erase( const_iterator pos ) {return boost::make_transform_iterator(_store.erase(pos.base()), &_to_ref);}
	iterator erase( const_iterator first, const_iterator last )
	{
		return boost::make_transform_iterator(_store.erase(first.base(), last.base()), &_to_ref);
	}
	void push_back(value_type& value) {_store.push_back(&value);}
	void pop_back() 		 		  {_store.pop_back();}
	void resize( size_type count, value_type& value ) {_store.resize(count, &value);}

	void swap( ref_vector& other ) {_store.swap(other._store);}

	template<typename T2, class Alloc> friend bool operator==	(const ref_vector<T2, Alloc> & lhs, const ref_vector<T2, Alloc> &rhs);
	template<typename T2, class Alloc> friend bool operator!=	(const ref_vector<T2, Alloc> & lhs, const ref_vector<T2, Alloc> &rhs);
	template<typename T2, class Alloc> friend bool operator<	(const ref_vector<T2, Alloc> & lhs, const ref_vector<T2, Alloc> &rhs);
	template<typename T2, class Alloc> friend bool operator<=	(const ref_vector<T2, Alloc> & lhs, const ref_vector<T2, Alloc> &rhs);
	template<typename T2, class Alloc> friend bool operator>	(const ref_vector<T2, Alloc> & lhs, const ref_vector<T2, Alloc> &rhs);
	template<typename T2, class Alloc> friend bool operator>=	(const ref_vector<T2, Alloc> & lhs, const ref_vector<T2, Alloc> &rhs);
};

template<typename T, class Allocator>
inline bool operator==	(const ref_vector<T, Allocator> & lhs, const ref_vector<T, Allocator> &rhs) {return lhs._store == rhs._store;}
template<typename T, class Allocator>
inline bool operator!=	(const ref_vector<T, Allocator> & lhs, const ref_vector<T, Allocator> &rhs) {return lhs._store != rhs._store;}
template<typename T, class Allocator>
inline bool operator<	(const ref_vector<T, Allocator> & lhs, const ref_vector<T, Allocator> &rhs) {return lhs._store < rhs._store; }
template<typename T, class Allocator>
inline bool operator<=	(const ref_vector<T, Allocator> & lhs, const ref_vector<T, Allocator> &rhs) {return lhs._store <= rhs._store;}
template<typename T, class Allocator>
inline bool operator>	(const ref_vector<T, Allocator> & lhs, const ref_vector<T, Allocator> &rhs) {return lhs._store > rhs._store; }
template<typename T, class Allocator>
inline bool operator>=	(const ref_vector<T, Allocator> & lhs, const ref_vector<T, Allocator> &rhs) {return lhs._store >= rhs._store;}


} /* namespace util */

#endif /* REF_VECTOR_H_ */
