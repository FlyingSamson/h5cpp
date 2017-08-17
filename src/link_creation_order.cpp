//
// (c) Copyright 2017 DESY,ESS
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
// Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
// Created on: Aug 17, 2017
//

#include "property_list.hpp"

namespace hdf5 {
namespace property_list {

LinkCreationOrder::LinkCreationOrder():
  tracked_(0),
  indexed_(0),
  reserved_(0)
{

}

LinkCreationOrder::LinkCreationOrder(unsigned value):
  tracked_(0),
  indexed_(0),
  reserved_(0)
{
  tracked_ = value & H5P_CRT_ORDER_TRACKED;
  indexed_ = (value & H5P_CRT_ORDER_INDEXED) >> 1;
}

LinkCreationOrder &LinkCreationOrder::enable_tracked()
{
  tracked_=1;
  return *this;
}

LinkCreationOrder &LinkCreationOrder::disable_tracked()
{
  tracked_=0;
  return *this;
}

LinkCreationOrder &LinkCreationOrder::enable_indexed()
{
  tracked_=1;
  indexed_=1;
  return *this;
}

LinkCreationOrder &LinkCreationOrder::disable_indexed()
{
  indexed_=0;
  return *this;
}

bool LinkCreationOrder::tracked() const
{
  return tracked_;
}

bool LinkCreationOrder::indexed() const
{
  return indexed_;
}


} // namespace property_list
} // namespace hdf5
