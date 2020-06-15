#include "applicationcli.h"
#include "typedefs.h"
#include "gds_graphfactory.h"

#include "gds_structureinfo.h"
#include "gds_structure.h"
#include "gds_path.h"
#include "gds_bondary.h"
#include "gds_box.h"
#include "gds_arrayReference.h"
#include "gds_reference.h"
#include "Point2D.h"
#include "Transformation2D.h"
#include "printhelper.h"

#include <string>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cassert>

using namespace std;

ApplicationCLI::ApplicationCLI (int argc, char* argv[])
{
    if (argc > 1) {
        myFilename = argv[1];
    }   
}
namespace {
template<typename T>
T GetValue (const std::string& theText)
{
    T aValue;
    std::cout << theText << ":"; //"Enter the value (or -1 to stop): ";
    std::cin >> aValue;
    return aValue;
}

template<typename T, typename Container>
Container GetValues (const std::string& theText)
{
    Container aValues;

    int aLastValue = -1;

    while (true)
    {
        aLastValue = GetValue<int> (theText);
        if (aLastValue == -1) {
            break;
        }
        aValues.insert (aLastValue);
    }

    return aValues;
}

std::ofstream& OutputFile()
{
	static std::ofstream aStream("output.txt", std::ofstream::out);
	return aStream;
}

void ElementInfoMenu (const GDS_GraphPtr& theGraph)
{
    while (true) {
        cout << "-----------------------------" << endl;
        cout << "0. Exit" << endl;
        cout << "1. Print Element info" << endl;
		cout << "2. Print Element info to file" << endl;
        cout << "-----------------------------" << endl;

        auto aNumber = GetValue<int> ("Enter number");

        if (aNumber == 0) {
            break;
        }

        if (aNumber > 2) {
            continue;
        }

        if (!theGraph) {
            cout << "The current structure is null." << endl;
            continue;
        }

        auto anElementNumber = GetValue<int> ("Enter element number from structure " + theGraph->Structure()->Name());
        auto anElement = theGraph->SearchElement (anElementNumber);

        if (anElement) {

			auto& aStream = aNumber == 1 ? std::cout : OutputFile();

			PrintHelper aHelper(aStream);
			aHelper.PrintElementFullInfomation(anElement);
			auto aConnections = theGraph->SearchConnectedWith(anElement);
			aHelper.PrintConnections(aConnections);
        } else {
            cout << "The element doesn't exist" << endl;
            continue;
        }
    }
}

}

int ApplicationCLI::exec()
{
    if (myFilename.empty()) {
        cout << "Please enter source file: ";
		std::cin >> myFilename;
    }

    auto anIsImported = myApplication.Import (myFilename);

    if (!anIsImported) {
        cout << "Unable to load the file: " << myFilename << endl;
        return 1;
    }

    cout << "Enter contact layers:" << endl;
    auto aContactLayers = GetValues<LayerType, LayersSet> ("Enter the value (or -1 to stop)");
    myApplication.SetContactLayers (aContactLayers);
    cout << aContactLayers.size() << endl;
    cout << endl;

    cout << "Enter isolator layers:" << endl;
    auto anIsolatorLayers = GetValues<LayerType, LayersSet> ("Enter the value (or -1 to stop)");

    myApplication.SetIsolatorLayers (anIsolatorLayers);
    cout << anIsolatorLayers.size() << endl;
    cout << endl;


    while (true) {
        cout << "-----------------------------" << endl;
        cout << "0. Exit" << endl;
        cout << "1. Print structure info" << endl;
        cout << "2. Print structure connections" << endl;
		cout << "3. Print structure connections to file" << endl;
        cout << "-----------------------------" << endl;

        auto aNumber = GetValue<int> ("Enter number");

        if (aNumber == 0) {
            break;
        }

        if (aNumber > 3) {
            continue;
        }

        if (aNumber == 1 || aNumber == 2 || aNumber == 3) {

            auto aStructureName = GetValue<string> ("Enter structure name");

            myCurrentStructure = myApplication.Structure (aStructureName);

            if (!myCurrentStructure) {
                cout << "The structure " << aStructureName << " doesn't exist" << endl;
                continue;
            }

            if (aNumber == 1) {
                auto anInfo = myApplication.GetStructureInfo (myCurrentStructure);
				PrintHelper aHelper (cout);
				aHelper.PrintStructureInfo (anInfo);
            } else if (aNumber == 2 || aNumber == 3) {
                auto aCurrentGraph = myApplication.GetGraph (myCurrentStructure);
				
				auto& aStream = aNumber == 2 ? std::cout : OutputFile();

				PrintHelper aHelper(aStream);
				aHelper.PrintGraph(aCurrentGraph);

                ElementInfoMenu (aCurrentGraph);
            }
        }
    }

    return 0;
}
