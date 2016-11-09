/***************************************************************************
* Copyright (C) 2016 Alexander V. Popov.
* 
* This source code is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of 
* the License, or (at your option) any later version.
* 
* This source code is distributed in the hope that it will be 
* useful, but WITHOUT ANY WARRANTY; without even the implied 
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
* See the GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
***************************************************************************/
#ifndef EXPERIMENTAL_TEMPLATES_COUNT_INDIRECTIONS_H
#define EXPERIMENTAL_TEMPLATES_COUNT_INDIRECTIONS_H

// Compile-time indirections counter (aka "counter of stars in the pointer")

namespace experimental
{
	namespace count_indirections_ns 
	{
		template<typename T> size_t count_indirections() 
		{
			return std::is_pointer<T>::value ? ( 1 + count_indirections<std::remove_pointer<T>::type>() ) : 0; 
		}
	}
	template<typename T> size_t count_indirections( const T& ) 
	{
		return count_indirections_ns::count_indirections<T>();
	}
	template<template<typename> class T, typename U> size_t count_indirections( const T<U>& ) 
	{ 
		return count_indirections_ns::count_indirections<U*>();
	}
	template<template<typename, typename> class T, typename U, typename V> size_t count_indirections( const T<U,V>& ) 
	{ 
		return count_indirections_ns::count_indirections<U*>();
	}
}

#endif //!EXPERIMENTAL_TEMPLATES_COUNT_INDIRECTIONS_H
