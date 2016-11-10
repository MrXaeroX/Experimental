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
#ifndef EXPERIMENTAL_COROUTINE_H
#define EXPERIMENTAL_COROUTINE_H

// Stackful coroutine implementation for 32-bit code
// This implementation also requires MASM compiler
// and frame pointer optimization disabled (in MSVC,
// it uses a MS-specific pragma to disable it locally)

// Coroutine is a template class, and can return different
// types of values. The return value should be either passed
// to yield(), or returned normally at the end of the coroutine
// function. If coroutine is finished, subsequent run calls
// return a default-constructed value. If enableRestart() is
// set to true, coroutine will restart from the very beginning
// instead (but this call doesn't affect a finished coroutine).

// always generate frame pointers on the program stack
#if defined(_MSC_VER)
#pragma optimize( "y", off )
#endif

#include <stdexcept>

extern "C" size_t experimental_coroutine_get_stack_pointer();
extern "C" size_t experimental_coroutine_save_stack( size_t stackBase, size_t stackMaxSize, void *stackData );
extern "C" size_t experimental_coroutine_restore_stack( size_t stackBase, const void *stackData );

namespace experimental
{
	// coroutine base class, common for all 
	// specialized coroutine templates
	template<typename T>
	class coroutine_base
	{
	public:
		typedef T ReturnValueType;

		// constructor
		explicit coroutine_base( const size_t stackSize ) 
								: restart_( false ), resume_( false ), 
								  stackBase_( 0 ), stackSize_( stackSize + sizeof(STACKHDR) ), 
								  stackData_( new decltype(stackSize_)[stackSize_/sizeof(*stackData_)]() ) {}

		// destructor
		virtual ~coroutine_base() { delete [] stackData_; }

		// function call operator, simply calls exec()
		inline ReturnValueType operator()() { return exec(); }

		// set restart flag (false by default)
		inline void enableRestart( bool value = true ) { restart_ = value; }

	protected:
		// must match the structure in the coroutine.asm
		struct STACKHDR {
			size_t lsize;
			size_t laddr;
			size_t lrebp;
			size_t r_eax;
			size_t r_ebx;
			size_t r_ecx;
			size_t r_edx;
			size_t r_esi;
			size_t r_edi;
		};

		static const size_t Coroutine_Awaiting = size_t( 0 );
		static const size_t Coroutine_Complete = size_t( ~0 );

		// coroutine body (abstract)
		virtual ReturnValueType run() = 0;

		// coroutine call helper (abstract)
		virtual ReturnValueType exec() = 0;

	protected:
		bool	restart_;
		bool	resume_;
		size_t	stackBase_;			// saved frame pointer
		size_t	stackSize_;			// allocated stack size
		size_t	*stackData_;		// allocated stack data
	};

	// template coroutine with a return value
	template<typename T>
	class coroutine : public coroutine_base<T>
	{
		struct yield_call { 
			explicit yield_call( ReturnValueType value ) : returnValue( value ) {}
			ReturnValueType returnValue; 
		};

	public:
		// constructor
		explicit coroutine( const size_t stackSize = 0x1000 ) : coroutine_base( stackSize ) {}

		// coroutine call helper
		virtual ReturnValueType exec()
		{
			ReturnValueType result = ReturnValueType();

			// remember frame pointer
			// this is bottom of the stack of the coroutine
			stackBase_ = experimental_coroutine_get_stack_pointer();

			STACKHDR *hdr = reinterpret_cast<STACKHDR*>( stackData_ );

			if ( hdr->lsize == Coroutine_Complete )
				return result;

			try {
				if ( hdr->lsize != Coroutine_Awaiting ) {
					resume_ = true;
					experimental_coroutine_restore_stack( stackBase_, stackData_ );
				}
				result = run();
				hdr->lsize = restart_ ? Coroutine_Awaiting : Coroutine_Complete;
			} catch ( const yield_call &e ) {
				result = e.returnValue;
			}

			return result;
		}

		// coroutine yield procedure
		void yield( ReturnValueType returnValue = ReturnValueType() )
		{
			resume_ = false;

			const STACKHDR *hdr = reinterpret_cast<const STACKHDR*>( stackData_ );
			experimental_coroutine_save_stack( stackBase_, stackSize_, stackData_ );

			if ( !hdr->lsize )
				throw std::runtime_error( "Coroutine stack overflow" );

			if ( !resume_ )
				throw yield_call( returnValue );
		}
	};

	// specialized coroutine without a return value
	template<>
	class coroutine<void> : public coroutine_base<void>
	{
		struct yield_call {};

	public:
		// constructor
		explicit coroutine( const size_t stackSize = 0x1000 ) : coroutine_base( stackSize ) {}

	protected:
		// coroutine call helper
		virtual void exec()
		{
			// remember frame pointer
			// this is bottom of the stack of the coroutine
			stackBase_ = experimental_coroutine_get_stack_pointer();

			STACKHDR *hdr = reinterpret_cast<STACKHDR*>( stackData_ );

			if ( hdr->lsize == Coroutine_Complete )
				return;

			try {
				if ( hdr->lsize != Coroutine_Awaiting ) {
					resume_ = true;
					experimental_coroutine_restore_stack( stackBase_, stackData_ );
				}
				run();
				hdr->lsize = restart_ ? Coroutine_Awaiting : Coroutine_Complete;
			} catch ( const yield_call & ) {}
		}

		// coroutine yield procedure
		void yield()
		{
			resume_ = false;

			const STACKHDR *hdr = reinterpret_cast<const STACKHDR*>( stackData_ );
			experimental_coroutine_save_stack( stackBase_, stackSize_, stackData_ );

			if ( !hdr->lsize )
				throw std::runtime_error( "Coroutine stack overflow" );

			if ( !resume_ )
				throw yield_call();
		}
	};
}

// restore frame pointer optimizations
#if defined(_MSC_VER)
#pragma optimize( "", on )
#endif

#endif //!EXPERIMENTAL_COROUTINE_H
