#include "catch.hpp"
#include "internal/catch_xmlwriter.h"

#include <sstream>

inline std::string encode( std::string const& str, Catch::XmlEncode::ForWhat forWhat = Catch::XmlEncode::ForTextNodes ) {
    std::ostringstream oss;
    oss << Catch::XmlEncode( str, forWhat );
    return oss.str();
}

TEST_CASE( "XmlEncode", "[XML]" ) {
    SECTION( "normal string" ) {
        REQUIRE( encode( "normal string" ) == "normal string" );
    }
    SECTION( "empty string" ) {
        REQUIRE( encode( "" ) == "" );
    }
    SECTION( "string with ampersand" ) {
        REQUIRE( encode( "smith & jones" ) == "smith &amp; jones" );
    }
    SECTION( "string with less-than" ) {
        REQUIRE( encode( "smith < jones" ) == "smith &lt; jones" );
    }
    SECTION( "string with greater-than" ) {
        REQUIRE( encode( "smith > jones" ) == "smith > jones" );
        REQUIRE( encode( "smith ]]> jones" ) == "smith ]]&gt; jones" );
    }
    SECTION( "string with quotes" ) {
        std::string stringWithQuotes = "don't \"quote\" me on that";
        REQUIRE( encode( stringWithQuotes ) == stringWithQuotes );
        REQUIRE( encode( stringWithQuotes, Catch::XmlEncode::ForAttributes ) == "don't &quot;quote&quot; me on that" );
    }
    SECTION( "string with control char (1)" ) {
        REQUIRE( encode( "[\x01]" ) == "[\\x01]" );
    }
    SECTION( "string with control char (x7F)" ) {
        REQUIRE( encode( "[\x7F]" ) == "[\\x7F]" );
    }
}

TEST_CASE("XmlEncode: UTF-8", "[xml][utf-8]") {
    std::vector<char> bytes;
    SECTION("Valid utf-8 strings") {
        CHECK(encode(u8"Here be 👾") == u8"Here be 👾");
        CHECK(encode(u8"šš") == u8"šš");
    }
    SECTION("Invalid utf-8 strings") {
        CHECK(encode(u8"Here \xFF be 👾") == u8"Here \\xFF be 👾");
        CHECK(encode("\xFF") == "\\xFF");
        CHECK(encode("\xF4\x90\x80\x80") == u8"\\xF4\\x90\\x80\\x80");

        bytes = { '\xC5', '\xC5', '\xA0', '\0' };
        CHECK(encode(bytes.data()) == u8"\\xC5Š");
    }
}
