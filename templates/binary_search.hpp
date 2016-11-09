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
#ifndef EXPERIMENTAL_TEMPLATES_BINARY_SEARCH_H
#define EXPERIMENTAL_TEMPLATES_BINARY_SEARCH_H

// Template-based binary search algorithm.
// Performs on small compile-time sorted arrays and 
// dynamic sorted arrays with size known at compile time.
// Shouldn't be used on large arrays because of recursion.

namespace experimental
{
	namespace binary_search_ns 
	{
		template<typename ValueType> struct helper	
		{
			template<size_t Lo, size_t Delta> struct impl 
			{
				static bool search( const ValueType *ArrayPointer, ValueType Value )
				{
					const size_t Hi = Lo + Delta;
					const size_t Mi = Lo + Delta / 2;
					if ( ArrayPointer[Mi] == Value )
						return true;
					else if ( ArrayPointer[Mi] > Value )
						return impl<Lo,Mi-Lo>::search( ArrayPointer, Value );
					else
						return impl<Mi+1,Hi-Mi-1>::search( ArrayPointer, Value );
				}
			};
			template<size_t Lo> struct impl<Lo,0> 
			{
				static bool search( const ValueType *ArrayPointer, ValueType Value )
				{
					return ( ArrayPointer[Lo] == Value );
				}
			};
		};
	}
	template<size_t ArraySize, typename ValueType> bool binary_search( const ValueType (&Array)[ArraySize], ValueType Value )
	{
		return binary_search_ns::helper<ValueType>::impl<0,ArraySize-1>::search( Array, Value );
	}
	template<size_t ArraySize, typename ValueType> bool binary_search( const ValueType *ArrayPointer, ValueType Value )
	{
		return binary_search_ns::helper<ValueType>::impl<0,ArraySize-1>::search( ArrayPointer, Value );
	}
}

#endif //!EXPERIMENTAL_TEMPLATES_BINARY_SEARCH_H
