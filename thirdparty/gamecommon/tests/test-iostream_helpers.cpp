/**
 * @file   test-iostream_helpers.cpp
 * @brief  Test code for the iostream helper functions.
 *
 * Copyright (C) 2010-2011 Adam Nielsen <malvineous@shikadi.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/test/unit_test.hpp>
#include <camoto/stream_string.hpp>
#include <camoto/iostream_helpers.hpp>

using namespace camoto;

BOOST_AUTO_TEST_SUITE(iostream_helpers)

BOOST_AUTO_TEST_CASE(null_padded_write)
{
	BOOST_TEST_MESSAGE("Write null-padded string");
	{
		stream::string_sptr data(new stream::string());
		data << nullPadded("AB", 4);
		BOOST_REQUIRE_EQUAL(data->str().length(), 4);
		BOOST_CHECK_EQUAL((uint8_t)data->str().at(0), 0x41);
		BOOST_CHECK_EQUAL((uint8_t)data->str().at(1), 0x42);
		BOOST_CHECK_EQUAL((uint8_t)data->str().at(2), 0x00);
		BOOST_CHECK_EQUAL((uint8_t)data->str().at(3), 0x00);
	}
}

BOOST_AUTO_TEST_CASE(null_padded_read)
{
	BOOST_TEST_MESSAGE("Read null-padded string");
	{
		stream::string_sptr data(new stream::string());
		data << std::string("ABC\0EFGHIJKL", 12);
		data->seekg(0, stream::start);
		std::string v;
		data >> nullPadded(v, 8);
		BOOST_CHECK(v.compare("ABC") == 0);
		// Make sure all eight characters were read, even though the ones after the
		// null were discarded.
		BOOST_REQUIRE_EQUAL(data->tellg(), 8);
	}
}

BOOST_AUTO_TEST_CASE(fixed_length_read)
{
	BOOST_TEST_MESSAGE("Fixed-length read with embedded nulls");
	{
		stream::string_sptr data(new stream::string());
		data << std::string("ABC\0EFGHIJKL", 12);
		data->seekg(0, stream::start);
		std::string v;
		data >> fixedLength(v, 8);
		BOOST_REQUIRE_EQUAL(v.length(), 8);
		BOOST_CHECK_EQUAL((uint8_t)v.c_str()[0], 0x41);
		BOOST_CHECK_EQUAL((uint8_t)v.c_str()[1], 0x42);
		BOOST_CHECK_EQUAL((uint8_t)v.c_str()[2], 0x43);
		BOOST_CHECK_EQUAL((uint8_t)v.c_str()[3], 0x00);
		BOOST_CHECK_EQUAL((uint8_t)v.c_str()[4], 0x45);
		BOOST_CHECK_EQUAL((uint8_t)v.c_str()[5], 0x46);
		BOOST_CHECK_EQUAL((uint8_t)v.c_str()[6], 0x47);
		BOOST_CHECK_EQUAL((uint8_t)v.c_str()[7], 0x48);
		BOOST_CHECK_EQUAL((uint8_t)v.c_str()[8], 0x00);
	}
}

BOOST_AUTO_TEST_CASE(null_terminated_write)
{
	BOOST_TEST_MESSAGE("Write null-terminated string");
	{
		stream::string_sptr data(new stream::string());
		data << nullTerminated("AB", 4);
		BOOST_REQUIRE_EQUAL(data->str().length(), 3);
		BOOST_CHECK_EQUAL((uint8_t)data->str().at(0), 0x41);
		BOOST_CHECK_EQUAL((uint8_t)data->str().at(1), 0x42);
		BOOST_CHECK_EQUAL((uint8_t)data->str().at(2), 0x00);
	}
}

BOOST_AUTO_TEST_CASE(null_terminated_read)
{
	BOOST_TEST_MESSAGE("Read null-terminated string");
	{
		stream::string_sptr data(new stream::string());
		data << std::string("ABC\0EFGHIJKL", 12);
		data->seekg(0, stream::start);
		std::string v;
		data >> nullTerminated(v, 8);
		BOOST_CHECK(v.compare("ABC") == 0);
		// Make sure only the characters up to the null were read, even though there
		// was room to read more.
		BOOST_REQUIRE_EQUAL(data->tellg(), 4);
	}
}

BOOST_AUTO_TEST_SUITE_END()
