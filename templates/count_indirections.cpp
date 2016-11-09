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
#include "count_indirections.hpp"
#include <cassert>
#include <memory>

int main( int argc, char *argv[] )
{
	const int i = 0;
	const int *pi = &i;
	const int **ppi = &pi;
	const int *********pppi = nullptr;
	const auto sp = std::make_shared<int**>();
	const auto up = std::unique_ptr<char****>( new char**** );
	const auto wp = std::weak_ptr<int**>( sp );

	assert( 0 == experimental::count_indirections( i ) );
	assert( 1 == experimental::count_indirections( pi ) );
	assert( 2 == experimental::count_indirections( ppi ) );
	assert( 9 == experimental::count_indirections( pppi ) );
	assert( 3 == experimental::count_indirections( sp ) );
	assert( 5 == experimental::count_indirections( up ) );
	assert( 3 == experimental::count_indirections( wp ) );

	return 0;
}
