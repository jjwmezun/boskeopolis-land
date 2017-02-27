

// Name
//===================================
//
// CorruptSaveException
//

#ifndef CORRUPT_SAVE_EXCEPTION_H
#define CORRUPT_SAVE_EXCEPTION_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

	#include <stdexcept>	


// CLASS
//===================================
	namespace BoskCorruptSave
	{
		class InvalidSaveSizeException : public std::runtime_error
		{
			public:
				InvalidSaveSizeException() : std::runtime_error( "Save file is corrupt:\nfile size doesn't match expected size.\nPlease try making a new save." ) {};
		};
		
		inline void testSaveSize( int save_size, int size_needed )
		{
			if ( save_size < size_needed ) throw InvalidSaveSizeException();
		};
	}

#endif // CORRUPT_SAVE_EXCEPTION_H
