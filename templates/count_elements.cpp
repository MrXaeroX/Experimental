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
#include "count_elements.hpp"
#include <cassert>
#include <memory>

int main( int argc, char *argv[] )
{
	const char c_string[] = "Hello, world!";
    const int c_integers[] = { 0, 1, 2, 3, 2, 7, 123, 2 };

	assert( 2 == experimental::count_elements( c_string, 'o' ) );
    assert( 1 == experimental::count_elements( c_string, 'H' ) );
    assert( 0 == experimental::count_elements( c_string, 'z' ) );

	assert( 3 == experimental::count_elements( c_integers, 2 ) );
	assert( 1 == experimental::count_elements( c_integers, 123 ) );
	assert( 0 == experimental::count_elements( c_integers, 999 ) );

	return 0;
}
