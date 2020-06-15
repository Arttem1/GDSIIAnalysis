#pragma once
#include <unordered_map>

class UnitTests
{
public:
	UnitTests() {}

	int Run();

private:
	void IntersectionTest();
	void ReaderTest();
	void GraphTest();

private:
	void TEST(const std::string& theTestname);
	void CHECK (bool theValue);
	int PRINT();

	std::string myCurrentTest;
	std::unordered_map<std::string, std::pair<int, int>> myResults;
};

