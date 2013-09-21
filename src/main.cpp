#include "common.h"
#include "errorcodes.h"
#include "EngineCore.hpp"

#include <exception>

int main()
{
	try
	{
		EngineCore();


	} catch (std::exception& ex) {
		LogFatal(ex.what());
	}
	return 0;
}
