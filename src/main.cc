#if defined(WIN32) or defined(_WIN32) or defined(__WIN32__) or defined(__NT__)
#	define WINDOWS
#	error Windows is currently not supported
#elif defined(__linux__) or defined(linux)
#	define LINUX
#else
#	error This platform is not supported
#endif

#include "app.hh"

int main (int argc, char* argv[]) {
	App app(argc, argv);

	app.Start();

	return 0;
};
