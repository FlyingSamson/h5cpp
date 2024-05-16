//
// (c) Copyright 2017 DESY,ESS
//               2020 Eugen Wintersberger <eugen.wintersberger@gmail.com>
//
// This file is part of h5pp.
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty ofMERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the
// Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor
// Boston, MA  02110-1301 USA
// ===========================================================================
//
// Authors:
//   Eugen Wintersberger <eugen.wintersberger@gmail.com>
//   Martin Shetty <martin.shetty@esss.se>
//   Jan Kotanski <jan.kotanski@desy.de>
// Created on: Aug 21, 2017
//

#ifdef H5CPP_CATCH2_V2
#include <catch2/catch.hpp>
#else
#include <catch2/catch_all.hpp>
#endif
#include <h5cpp/file/memory_driver.hpp>
#include <h5cpp/property/file_access.hpp>
#include <sstream>
#include <string>
#include <tuple>
#include <initializer_list>
#include "../utilities.hpp"
#include "utilities.hpp"

namespace pl = hdf5::property;

// we need to define these test cases outside of any catch2 MACRO as '#if'-directives inside a
// macro are not portable
// tuple contents: (library version enum value, library version string representation)
const std::initializer_list<std::tuple<pl::LibVersion, std::string>> lib_version_to_strings {
#if H5_VERSION_GE(1,10,2)
  {pl::LibVersion::V18, "V18"}, 
  {pl::LibVersion::V110, "V110"},
#endif
#if H5_VERSION_GE(1,12,0)
  {pl::LibVersion::V112, "V112"},
#endif
#if H5_VERSION_GE(1,13,0)
  {pl::LibVersion::V114, "V114"},
#endif
#if H5_VERSION_LE(1,10,1)
  // starting with 1.10.2 "Latest" is reported as the specific latest version
  {pl::LibVersion::Latests, "LATEST"},
#endif
  {pl::LibVersion::Earliest, "EARLIEST"}
};

SCENARIO("Writing file access associated enumerations to a stream") {
  GIVEN("library version values") {
    auto versions = GENERATE(table<pl::LibVersion, std::string>(lib_version_to_strings));
    THEN("writing this to the stream") {
      std::stringstream s;
      s << std::get<0>(versions);
      REQUIRE(s.str() == std::get<1>(versions));
    }
  }
  GIVEN("closing degree values") {
    using r = std::tuple<pl::CloseDegree, std::string>;
    auto degrees = GENERATE(table<pl::CloseDegree, std::string>(
        {r{pl::CloseDegree::Weak, "WEAK"}, r{pl::CloseDegree::Strong, "STRONG"},
         r{pl::CloseDegree::Semi, "SEMI"},
         r{pl::CloseDegree::Default, "DEFAULT"}}));
    AND_WHEN("we write these values to the stream") {
      std::stringstream s;
      s << std::get<0>(degrees);
      REQUIRE(s.str() == std::get<1>(degrees));
    }
  }
}

SCENARIO("FileAccessList creation") {
  GIVEN("a default constructed list") {
    pl::FileAccessList fapl;
    THEN("we expect") {
      REQUIRE(fapl.get_class() == pl::kFileAccess);
      REQUIRE(fapl.library_version_bound_low() == pl::LibVersion::Earliest);
      REQUIRE(fapl.library_version_bound_high() == pl::LibVersion::Latest);
      REQUIRE(fapl.close_degree() == pl::CloseDegree::Default);
    }
    WHEN("closing the object") {
      close(fapl);
      THEN("all methods throw exceptions") {
        REQUIRE_THROWS_AS(fapl.library_version_bound_low(), std::runtime_error);
        REQUIRE_THROWS_AS(fapl.library_version_bound_high(),
                          std::runtime_error);
        REQUIRE_THROWS_AS(fapl.close_degree(), std::runtime_error);
        REQUIRE_THROWS_AS(fapl.close_degree(pl::CloseDegree::Strong),
                          std::runtime_error);
      }
    }
  }
  GIVEN("a handle to a file access list") {
    auto handle = handle_from_class(pl::kFileAccess);
    THEN("we can use this to construct a valid property list") {
      REQUIRE_NOTHROW(pl::FileAccessList{std::move(handle)});
    }
  }
  GIVEN("a handle to a group creation propety list") {
    auto handle = handle_from_class(pl::kGroupCreate);
    THEN("creating a file access list must fail") {
      REQUIRE_THROWS_AS(pl::FileAccessList{std::move(handle)},
                        std::runtime_error);
    }
  }
}

// we need to define these test cases outside of any catch2 MACRO as '#if'-directives inside a
// macro are not portable
// tuple contents: (lower version number, upper version number, valid)
const std::initializer_list<std::tuple<pl::LibVersion, pl::LibVersion, bool>> 
  lib_version_compatibility_list {
      {pl::LibVersion::Earliest, pl::LibVersion::Earliest, false},
#if H5_VERSION_GE(1,10,2)
      {pl::LibVersion::Earliest, pl::LibVersion::V18, true},
      {pl::LibVersion::Earliest, pl::LibVersion::V110, true},
      {pl::LibVersion::V18, pl::LibVersion::Earliest, false},
      {pl::LibVersion::V18, pl::LibVersion::V18, true},
      {pl::LibVersion::V18, pl::LibVersion::V110, true},
      {pl::LibVersion::V110, pl::LibVersion::Earliest, false},
      {pl::LibVersion::V110, pl::LibVersion::V18, false},
      {pl::LibVersion::V110, pl::LibVersion::V110, true},
#endif
#if H5_VERSION_GE(1,12,0)
      {pl::LibVersion::Earliest, pl::LibVersion::V112, true},
      {pl::LibVersion::V18, pl::LibVersion::V112, true},
      {pl::LibVersion::V110, pl::LibVersion::V112, true},
      {pl::LibVersion::V112, pl::LibVersion::Earliest, false},
      {pl::LibVersion::V112, pl::LibVersion::V18, false},
      {pl::LibVersion::V112, pl::LibVersion::V110, false},
      {pl::LibVersion::V112, pl::LibVersion::V112, true},
#endif
#if H5_VERSION_GE(1,13,0)
      {pl::LibVersion::Earliest, pl::LibVersion::V114, true},
      {pl::LibVersion::V18, pl::LibVersion::V114, true},
      {pl::LibVersion::V110, pl::LibVersion::V114, true},
      {pl::LibVersion::V112, pl::LibVersion::V114, true},
      {pl::LibVersion::V114, pl::LibVersion::Earliest, false},
      {pl::LibVersion::V114, pl::LibVersion::V18, false},
      {pl::LibVersion::V114, pl::LibVersion::V110, false},
      {pl::LibVersion::V114, pl::LibVersion::V112, false},
      {pl::LibVersion::V114, pl::LibVersion::V114, true},
#endif
      {pl::LibVersion::Earliest, pl::LibVersion::Latest, true},
      {pl::LibVersion::Latest, pl::LibVersion::Latest, true}
};

SCENARIO("Setting the library version on a file access property list") {
  GIVEN("a default constructed file access property list") {
    pl::FileAccessList fapl;

    auto versions = 
        GENERATE(table<pl::LibVersion, pl::LibVersion, bool>(lib_version_compatibility_list));

    const auto lower_ver = std::get<0>(versions);
    const auto upper_ver = std::get<1>(versions);
    const auto valid_combination = std::get<2>(versions);

    std::stringstream ss;
    ss << "setting " << lower_ver << ":" << upper_ver;

    WHEN(ss.str()) {
      if(valid_combination){
        REQUIRE_NOTHROW(fapl.library_version_bounds(lower_ver, upper_ver));
        REQUIRE(fapl.library_version_bound_low() == lower_ver);
        REQUIRE(fapl.library_version_bound_high() == upper_ver);
      }
      else {
        THEN("the operation must fail") {
        REQUIRE_THROWS_AS(fapl.library_version_bounds(pl::LibVersion::Earliest,
                                                      pl::LibVersion::Earliest),
                          std::runtime_error);
        }
      }
    }
  }
}

SCENARIO("setting close degree on a file access property list") {
  GIVEN("a default constructed file access property list") {
    pl::FileAccessList fapl;
    THEN("setting the close degree to STRONG must work") {
      REQUIRE_NOTHROW(fapl.close_degree(pl::CloseDegree::Strong));
      REQUIRE(fapl.close_degree() == pl::CloseDegree::Strong);
    }
    THEN("setting the close degree to WEAK must work") {
      REQUIRE_NOTHROW(fapl.close_degree(pl::CloseDegree::Weak));
      REQUIRE(fapl.close_degree() == pl::CloseDegree::Weak);
    }
    THEN("setting the close degree to SEMI") {
      REQUIRE_NOTHROW(fapl.close_degree(pl::CloseDegree::Semi));
      REQUIRE(fapl.close_degree() == pl::CloseDegree::Semi);
    }
    THEN("setting the close degree to default") {
      REQUIRE_NOTHROW(fapl.close_degree(pl::CloseDegree::Default));
      REQUIRE(fapl.close_degree() == pl::CloseDegree::Default);
    }
  }
}

SCENARIO("setting the file driver on a file access property list") {
  GIVEN("a default constructed list") {
    pl::FileAccessList fapl;
    AND_GIVEN("the memorty driver") {
      hdf5::file::MemoryDriver driver;
      THEN("set can set the memory driver on the property list") {
        REQUIRE_NOTHROW(fapl.driver(driver));
      }
    }
  }
}
