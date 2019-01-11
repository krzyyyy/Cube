 // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_MAIN 
#include "Catch\Catch.h"
#include "stdafx.h"
#include "Reader.h"
#include "Exceptions.h"

unsigned int Factorial(unsigned int number) {
	return number <= 1 ? number : Factorial(number - 1)*number;
}

TEST_CASE("loading configuration file") {
	
	SECTION("cheacking loading cofiguration files") {
		unique_ptr<Reader> rd;
		REQUIRE_THROWS_AS(Reader::open("nie_istniejacy_plik.xml", rd),ConfigurationFileException);
		REQUIRE_THROWS(Reader::open("testConfigFiles/configuration1.txt", rd));
	}
	
}
TEST_CASE("loading conf. files with error mode for 2 extension") {
	vector <Mat> images;
	unique_ptr<Reader> rd2;
	Reader::open("testConfigFiles/configuration1.xml", rd2);
	REQUIRE_THROWS_AS(rd2.get()->load(images), ImageModeException);
}
