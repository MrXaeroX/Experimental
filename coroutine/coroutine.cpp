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
#include "coroutine.hpp"
#include <iostream>

class SimpleCoroutineTest : public experimental::coroutine<void>
{
protected:
	virtual void run()
	{
		int x = 10;
		int y = 20;

		std::cout << "x = " << x;
		yield();

		std::cout << "y = " << y;
		yield();

		std::cout << "x + y = " << (x + y);
		yield();

		std::cout << "x * y = " << (x * y);
	}
};

class ReturnValueCoroutineTest : public experimental::coroutine<int>
{
protected:
	virtual ReturnValueType run()
	{
		ReturnValueType counter = 0;

		yield( ++counter );
		yield( ++counter );
		yield( ++counter );
		yield( ++counter );
		yield( ++counter );

		return -1;
	}
};

class LocalLoopCoroutineTest : public experimental::coroutine<short>
{
protected:
	virtual ReturnValueType run()
	{
		for ( ReturnValueType counter = 1; counter <= 5; ++counter )
			yield( counter );

		return -1;
	}
};

class RecursiveCoroutineTest : public experimental::coroutine<unsigned long long>
{
protected:
	virtual ReturnValueType run()
	{
		for ( ReturnValueType i = 1; i < 15; ++i )
			yield( factorial( i ) );
		return -1;
	}
private:
	ReturnValueType factorial( ReturnValueType i ) {
		if ( i <= 1 )
			return 1;
		else
			return i * factorial( i - 1 );
	}
};

int main( int argc, char *argv[] )
{
	SimpleCoroutineTest c1;
	
	for ( auto i = 0; i < 8; ++i ) {
		std::cout << "[" << i << "] SimpleCoroutineTest: ";
		try {
			c1(); std::cout << std::endl;
		} catch ( std::exception &e ) {
			std::cout << "Coroutine error: " << e.what() << std::endl;
		}
	}
	std::cout << std::endl;

	ReturnValueCoroutineTest c2;
	
	for ( auto i = 0; i < 8; ++i ) {
		std::cout << "[" << i << "] ReturnValueCoroutineTest: ";
		try {
			std::cout << c2() << std::endl;
		} catch ( std::exception &e ) {
			std::cout << "Coroutine error: " << e.what() << std::endl;
		}
	}
	std::cout << std::endl;

	LocalLoopCoroutineTest c3;

	for ( auto i = 0; i < 8; ++i ) {
		std::cout << "[" << i << "] LocalLoopCoroutineTest: ";
		try {
			std::cout << c3() << std::endl;
		} catch ( std::exception &e ) {
			std::cout << "Coroutine error: " << e.what() << std::endl;
		}
	}
	std::cout << std::endl;

	RecursiveCoroutineTest c4;

	for ( auto i = 0; i < 15; ++i ) {
		std::cout << "[" << i << "] RecursiveCoroutineTest: ";
		try {
			std::cout << c4() << std::endl;
		} catch ( std::exception &e ) {
			std::cout << "Coroutine error: " << e.what() << std::endl;
		}
	}
	std::cout << std::endl;

	return 0;
}