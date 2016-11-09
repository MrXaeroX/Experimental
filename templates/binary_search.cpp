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
#include "binary_search.hpp"
#include <cassert>
#include <memory>
#include <vector>
#include <algorithm>

int main( int argc, char *argv[] )
{
	int c_sorted_array[] = { 1, 2, 3, 5, 6, 7, 9, 10 };

	assert( false == experimental::binary_search( c_sorted_array, 0 ) );
    assert(  true == experimental::binary_search( c_sorted_array, 1 ) );
    assert( false == experimental::binary_search( c_sorted_array, 4 ) );
    assert(  true == experimental::binary_search( c_sorted_array, 5 ) );
    assert( false == experimental::binary_search( c_sorted_array, 8 ) );
    assert(  true == experimental::binary_search( c_sorted_array, 10 ) );
    assert( false == experimental::binary_search( c_sorted_array, 100 ) );
 
	std::vector<int> c_vector;
	const int c_vectorSize = 8;
	c_vector.reserve( c_vectorSize );
    c_vector.push_back( 1 );
    c_vector.push_back( 3 );
    c_vector.push_back( 2 );
    c_vector.push_back( 9 );
    c_vector.push_back( 6 );
    c_vector.push_back( 7 );
    c_vector.push_back( 5 );
    c_vector.push_back( 10 );
	assert( c_vector.size() == c_vectorSize );
	std::sort( c_vector.begin(), c_vector.end() );

	assert( false == experimental::binary_search<c_vectorSize>( &c_vector[0], 0 ) );
    assert(  true == experimental::binary_search<c_vectorSize>( &c_vector[0], 1 ) );
    assert( false == experimental::binary_search<c_vectorSize>( &c_vector[0], 4 ) );
    assert(  true == experimental::binary_search<c_vectorSize>( &c_vector[0], 5 ) );
    assert( false == experimental::binary_search<c_vectorSize>( &c_vector[0], 8 ) );
    assert(  true == experimental::binary_search<c_vectorSize>( &c_vector[0], 10 ) );
    assert( false == experimental::binary_search<c_vectorSize>( &c_vector[0], 100 ) );

	return 0;
}
