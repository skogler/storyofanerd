#include "common.h"
#include "errorcodes.h"
#include "EngineCore.hpp"

#include <exception>

int main()
{
	try
	{
		EngineCore engine;
        engine.start();

	} catch (std::exception& ex) {
		LogFatal(ex.what());
	}
	return 0;
}
