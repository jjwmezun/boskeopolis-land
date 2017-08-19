#include "mezun_exceptions.hpp"

namespace mezun
{
	void error( const std::string& message )
	{
		std::cout<<message<<std::endl;
		exit( -1 );
	};
}