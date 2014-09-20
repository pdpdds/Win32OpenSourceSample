#pragma once
#include <string>

#define BOOST_TEST_MODULE atoi
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

namespace impl {

	int atoi(const std::string &val) {
		int result = 0;
		int multiplier = 1;
		bool negative = false;

		for (std::string::const_reverse_iterator i = val.rbegin();
			i < val.rend();  ++i) {
			if (*i <= '9' && *i >= '0') {
				result += (*i - '0') * multiplier;
				multiplier *= 10;
			}
			else {
				if (*i == '-' && i == val.rend() - 1) {
					negative = true;
				}
				else {
					return 0;
				}
			}
		}
		return negative ? -result : result;
	}

} // namespace impl

BOOST_AUTO_TEST_CASE(unit_position) {
	BOOST_CHECK_EQUAL(impl::atoi("6"), 6);
}

BOOST_AUTO_TEST_CASE(tenth_position) {
	BOOST_CHECK_EQUAL(impl::atoi("45"), 45);
}

BOOST_AUTO_TEST_CASE(large_number) {
	BOOST_CHECK_EQUAL(impl::atoi("123456789"), 123456789);
}

BOOST_AUTO_TEST_CASE(negative_number) {
	BOOST_CHECK_EQUAL(impl::atoi("-876"), -876);
}

BOOST_AUTO_TEST_CASE(sign_in_wrong_position) {
	BOOST_CHECK_EQUAL(impl::atoi("72-56"), 0);
}

BOOST_AUTO_TEST_CASE(invalid) {
	BOOST_CHECK_EQUAL(impl::atoi("abcd"), 1);
}