#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "Catch\Catch.h"
#include "stdafx.h"
#include "Reader.h"
#include "Exceptions.h"

unsigned int Factorial(unsigned int number) {
	return number <= 1 ? number : Factorial(number - 1)*number;
}

TEST_CASE("loading configuration file") {
	
	SECTION("cheacking loading cofiguration files") {
		REQUIRE_THROWS_AS(Reader::open("nie_istniejacy_plik.xml"),ConfigurationFileException);
		REQUIRE_THROWS(Reader::open("testConfigFiles/configuration1.xml"));
	}
	vector <Mat> images;
	SECTION("loading conf. files with error mode for 2 extension") {
		unique_ptr <Reader> reader(Reader::open("testConfigFiles/configuration1.txt"));
		REQUIRE_THROWS_AS(reader.get()->load(images), ImageModeException);
	}
}
