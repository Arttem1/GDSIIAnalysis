#include "applicationcli.h"
#include "unittests.h"

using namespace std;


int main(int argc, char* argv[])
{
	int aRes = 0;

	if (argc > 1 && std::strcmp (argv[1], "-test") == 0) {
		UnitTests aTests;
		aRes = aTests.Run();
	} else {
		ApplicationCLI anApp(argc, argv);
		aRes = anApp.exec();
	}

#if _WIN32 || _WIN64
	system("pause");
#endif
	return aRes;
}
