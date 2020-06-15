#include "unittests.h"
#include "point2d.h"
#include "gds_helper.h"
#include "polygon2d.h"
#include "gds_reader.h"
#include "gds_structureinfo.h"
#include "gds_stuctureinfocreator.h"
#include "gds_structure.h"
#include "gds_graphfactory.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
/*
#define _CRT_SECURE_NO_WARNINGS

namespace {
std::string TestModelsDir()
{
	const char* aModelsDir = std::getenv("TEST_MODELS_DIR");
	if (aModelsDir) {
		return aModelsDir;
	}
	else {
		std::cout << "Please set TEST_MODELS_DIR environment variable" << std::endl;
		assert(aModelsDir);
	}
}
}*/

void UnitTests::IntersectionTest()
{
	TEST("IntersectionTest");

	Point2D<double> a1(985.0, 1439.0);
	Point2D<double> a2(985.0, 1423.0);
	Point2D<double> a3(1003.0, 1423.0);
	Point2D<double> a4(1003.0, 1439.0);
	Point2D<double> a5(997.0, 1442.0);
	Point2D<double> a6(997.0, 1427.0);
	Point2D<double> a7(1009.0, 1427.0);
	Point2D<double> a8(1009.0, 1442.0);

	std::vector<polygon2d::Point> aCoordinates1;
	std::vector<polygon2d::Point> aCoordinates2;

	aCoordinates1.push_back(a1);
	aCoordinates1.push_back(a2);
	aCoordinates1.push_back(a3);
	aCoordinates1.push_back(a4);

	aCoordinates2.push_back(a5);
	aCoordinates2.push_back(a6);
	aCoordinates2.push_back(a7);
	aCoordinates2.push_back(a8);

	polygon2d aPolygon1(aCoordinates1);
	polygon2d aPolygon2(aCoordinates2);

	GDS_Helper helper;
	CHECK (helper.IntersectionPolygon(aPolygon1, aPolygon2));
	CHECK (!helper.PointInPolygon(a5, aPolygon1));
	CHECK (helper.PointInPolygon(a6, aPolygon1));
	CHECK (!helper.PointInPolygon(a7, aPolygon1));
	CHECK(!helper.PointInPolygon(a8, aPolygon1));
}

void UnitTests::ReaderTest()
{
	TEST("ReaderTest");

	GDS_Reader aReader;
	CHECK (aReader.ReadFile ("E:\\project\\TestModels\\Test0.GDS"));
	auto aFile = aReader.File();
	CHECK(!!aFile.MainStructure());

	GDS_StuctureInfoCreator anInfoCreator;
	aFile.MainStructure()->Accept(anInfoCreator);
	auto anInfo = anInfoCreator.Result();

	CHECK(anInfo.myActiveLayers.size() == 1);
	CHECK(anInfo.myBondaryCount == 0);
	CHECK(anInfo.myBoxesCount == 3);
	CHECK(anInfo.myPathCount == 3);
	CHECK(anInfo.myReferencesCountByMe == 1);
}

void UnitTests::GraphTest()
{
	TEST("GraphTest");

	GDS_Reader aReader;
	CHECK(aReader.ReadFile("E:\\project\\TestModels\\Test0.GDS"));

	// Check main graph
	auto aGraph = GDS_GraphFactory::Instance()->Graph(aReader.File().MainStructure());
	CHECK(!!aGraph);
	CHECK(aGraph->SubGraphs().size() == 1);
	CHECK(aGraph->Connections().size() == 3);
	CHECK(aGraph->mySearchContainer.size() == 6);

	auto aOneAncestorsCount = 0;
	auto aThreeAncestorsCount = 0;
	for (const auto& aConnection : aGraph->Connections()) {
		auto anAncestorsSize = aConnection.first->myAncestors.size();

		if (anAncestorsSize == 1) {
			aOneAncestorsCount++;
		}
		else if (anAncestorsSize == 3) {
			aThreeAncestorsCount++;
		}
	}
	CHECK(aOneAncestorsCount == 1);
	CHECK(aThreeAncestorsCount == 2);

	// Check sub graph
	auto aSubGraph = aGraph->SubGraphs().front();
	CHECK(!!aSubGraph);
	CHECK(aSubGraph->SubGraphs().size() == 0);
	CHECK(aSubGraph->Connections().size() == 2);
	CHECK(aSubGraph->mySearchContainer.size() == 4);

	for (const auto& aConnection : aSubGraph->Connections()) {
		CHECK(aConnection.first->myAncestors.size() == 1);
	}
}

int UnitTests::Run()
{
	IntersectionTest();
	ReaderTest();
	GraphTest();

	return PRINT();
}

void UnitTests::TEST(const std::string& theTestname)
{
	myCurrentTest = theTestname;
}

void UnitTests::CHECK(bool theValue)
{
	try {
		myResults.at(myCurrentTest);
	}
	catch (...) {
		auto& aTests = myResults[myCurrentTest];
		aTests.first = 0;
		aTests.second = 0;
	}

	auto& aTests = myResults[myCurrentTest];
	aTests.first += 1;
	aTests.second += theValue ? 0 : 1;
}

int UnitTests::PRINT()
{
	auto aFailedCount = 0;
	for (const auto& aResult : myResults) {
		const auto& aTestsResult = aResult.second;
		aFailedCount += aTestsResult.second;
		std::cout << "Test name: " << aResult.first << ". All: " << aTestsResult.first << " Failed: " << aTestsResult.second << " " << ((aTestsResult.second == 0) ? "OK" : "FAIL") << std::endl;
	}

	std::cout << ((aFailedCount == 0) ? "OK" : "FAIL") << std::endl;

	return aFailedCount;
}
