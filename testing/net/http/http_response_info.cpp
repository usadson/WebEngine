/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <sstream>

#include <climits>

#include <gtest/gtest.h>

#include "logger.hpp"

#include "net/http/http_response_info.hpp"

namespace Net::HTTP {

	class HTTPResponseInfoTest : public ::testing::Test {
	public:
		HTTPResponseInfo info;
	};

	TEST_F(HTTPResponseInfoTest, GetHeaderTest) {
		info.headers.clear();

		// Empty headerlist
		ASSERT_FALSE(info.GetHeader("").has_value());
		ASSERT_FALSE(info.GetHeader("unset").has_value());

		// 1-header headerlist
		info.headers.push_back({ "test", "okokokok" });
		ASSERT_TRUE(info.GetHeader("test").has_value());
		ASSERT_TRUE(info.GetHeader("Test").has_value());
		ASSERT_TRUE(info.GetHeader("TEST").has_value());
		ASSERT_TRUE(info.GetHeader("TeSt").has_value());
		ASSERT_EQ(strcmp(info.GetHeader("test").value(), "okokokok"), 0);
	}

	TEST_F(HTTPResponseInfoTest, GetHeaderUnsignedTest) {
		info.headers.clear();

		// Empty headerlist
		ASSERT_FALSE(info.GetHeaderUnsigned("").has_value());
		ASSERT_FALSE(info.GetHeaderUnsigned("unset").has_value());

		// 1-header headerlist
		info.headers.push_back({ "test", "123" });
		ASSERT_TRUE(info.GetHeaderUnsigned("test").has_value());
		ASSERT_TRUE(info.GetHeaderUnsigned("Test").has_value());
		ASSERT_TRUE(info.GetHeaderUnsigned("TEST").has_value());
		ASSERT_TRUE(info.GetHeaderUnsigned("TeSt").has_value());

		// Correct value?
		ASSERT_EQ(info.GetHeaderUnsigned("test").value(), 123);
		ASSERT_EQ(info.GetHeaderUnsigned("Test").value(), 123);
		ASSERT_EQ(info.GetHeaderUnsigned("TEST").value(), 123);
		ASSERT_EQ(info.GetHeaderUnsigned("TeSt").value(), 123);

		// Invalid integer (the function returns an unset optional when the
		// header does not exists, or the value couldn't be converted to an
		// unsigned integer).
		info.headers.push_back({ "invalid-integer", "this is not an integer" });
		ASSERT_FALSE(info.GetHeaderUnsigned("invalid-integer").has_value());
		ASSERT_FALSE(info.GetHeaderUnsigned("INVALID-INTEGER").has_value());
	}

} // namespace Net::HTTP

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
