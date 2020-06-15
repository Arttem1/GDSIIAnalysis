#ifndef APPLICATIONCLI_H
#define APPLICATIONCLI_H

#include "application.h"

class ApplicationCLI
{
public:
    ApplicationCLI (int argc, char* argv[]);

    int exec();

private:
    Application myApplication;
    std::string myFilename;
    GDS_StructurePtr myCurrentStructure;
};

#endif // APPLICATIONCLI_H
