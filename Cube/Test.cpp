 // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_MAIN 
#include "Catch\Catch.h"
#include "stdafx.h"
#include "Reader.h"
#include "Exceptions.h"


TEST_CASE("loading configuration file") {
	
	SECTION("cheacking loading cofiguration files") {
		unique_ptr<Reader> rd;
		cin.putback(3);
		REQUIRE_THROWS_AS(Reader::open("nie_istniejacy_plik.xml", rd),ConfigurationFileException);
		REQUIRE_NOTHROW(Reader::open("testConfigFiles/configuration1.txt", rd));
	}
	SECTION("cheacking error modes of images txt") {
		cin.putback(3);
		unique_ptr<Reader> rd;
		vector <Mat> images;
		REQUIRE_NOTHROW(Reader::open("testConfigFiles/configuration1.txt", rd));
		REQUIRE_THROWS_AS(rd->load(images), ImageModeException);
	}
	SECTION("cheacking error modes of images xml") {
		cin.putback(3);
		unique_ptr<Reader> rd;
		vector <Mat> images;
		REQUIRE_NOTHROW(Reader::open("testConfigFiles/configuration1.xml", rd));
		REQUIRE_THROWS_AS(rd->load(images), ImageModeException);
	}
	SECTION("cheacking error path of images txt") {
		cin.putback(3);
		unique_ptr<Reader> rd;
		vector <Mat> images;
		REQUIRE_NOTHROW(Reader::open("testConfigFiles/configuration2.txt", rd));
		REQUIRE_THROWS_AS(rd->load(images), ImageFileException);
	}
	SECTION("cheacking error path of images xml") {
		cin.putback(3);
		unique_ptr<Reader> rd;
		vector <Mat> images;
		REQUIRE_NOTHROW(Reader::open("testConfigFiles/configuration2.xml", rd));
		REQUIRE_THROWS_AS(rd->load(images), ImageFileException);
	}
	SECTION("cheacking too short list of images txt") {
		cin.putback(3);
		unique_ptr<Reader> rd;
		vector <Mat> images;
		REQUIRE_NOTHROW(Reader::open("testConfigFiles/configuration3.txt", rd));
		REQUIRE_THROWS_AS(rd->load(images), TooShortConfigException);
	}
	SECTION("cheacking too short list of images xml") {
		cin.putback(3);
		unique_ptr<Reader> rd;
		vector <Mat> images;
		REQUIRE_NOTHROW(Reader::open("testConfigFiles/configuration3.xml", rd));
		REQUIRE_THROWS_AS(rd->load(images), TooShortConfigException);
	}
	
}

