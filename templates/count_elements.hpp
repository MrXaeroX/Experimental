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
#ifndef EXPERIMENTAL_TEMPLATES_COUNT_ELEMENTS_H
#define EXPERIMENTAL_TEMPLATES_COUNT_ELEMENTS_H

// Template-based counter of values in small constant compile-time arrays
// Shouldn't be used on large arrays because of recursion.

namespace experimental
{
	namespace count_elements_ns 
	{
		template<typename T> struct helper 
		{
			template<size_t ArraySize> static const size_t impl( const T (&Array)[ArraySize], T Value ) 
			{
				return ( ( Array[ArraySize-1] == Value ) ? 1 : 0 ) + impl<ArraySize-1>( reinterpret_cast<const T(&)[ArraySize-1]>( Array ), Value );
			}
			template<> static const size_t impl<1>( const T (&Array)[1], T Value ) 
			{
				return ( Array[0] == Value ) ? 1 : 0;
			}
		};
	}
	template<typename T, size_t ArraySize> size_t count_elements( const T (&Array)[ArraySize], T Value ) 
	{
		return count_elements_ns::helper<T>::impl( Array, Value );
	}
}

#endif //!EXPERIMENTAL_TEMPLATES_COUNT_ELEMENTS_H
