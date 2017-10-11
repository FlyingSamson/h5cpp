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
// Author: Martin Shetty <martin.shetty@esss.se>
// Created on: Sep 9, 2017
//
#include <sstream>
#include <h5cpp/node/functions.hpp>

namespace hdf5 {
namespace node {

void copy(const Node &source, const Group& base, const Path &rel_path,
          const property::ObjectCopyList &ocpl,
          const property::LinkCreationList &lcpl)
{
  //what if rel_path is actually absolute?
  if (base.links.exists(static_cast<std::string>(rel_path)))
  {
    std::stringstream ss;
    ss << "node::copy failed. "
       << base.link() << " / " << rel_path << " already exists!";
    throw std::runtime_error(ss.str());
  }

  if (0 > H5Ocopy(static_cast<hid_t>(source.link().parent()), //parent
                  source.link().path().back().c_str(),        //object name
                  static_cast<hid_t>(base),                   //destination parent
                  static_cast<std::string>(rel_path).c_str(), //destination name
                  static_cast<hid_t>(ocpl),                   //object copy property list
                  static_cast<hid_t>(lcpl)))                  //link creation property list
  {
    std::stringstream ss;
    ss << "node::copy failed. Could not copy "
       << source.link() << " to "
       << base.link() << " / " << rel_path;
    throw std::runtime_error(ss.str());
  }
}

void copy(const Node &source, const Group& destination,
          const property::ObjectCopyList &ocpl,
          const property::LinkCreationList &lcpl)
{
  //what if rel_path is actually absolute?
  auto name = source.link().path().back(); //this feels awkward
  if (destination.links.exists(name))
  {
    std::stringstream ss;
    ss << "node::copy failed. "
       << destination.link() << " / " << name << " already exists!";
    throw std::runtime_error(ss.str());
  }

  if (0 > H5Ocopy(static_cast<hid_t>(source.link().parent()), //parent
                  name.c_str(),                               //object name
                  static_cast<hid_t>(destination),            //destination parent
                  name.c_str(),                               //...same name
                  static_cast<hid_t>(ocpl),                   //object copy property list
                  static_cast<hid_t>(lcpl)))                  //link creation property list
  {
    std::stringstream ss;
    ss << "node::copy failed. Could not copy "
       << source.link() << " to "
       << destination.link() << " / " << name;
    throw std::runtime_error(ss.str());
  }
}

void remove(const Node &object,
            const property::LinkAccessList &lapl)
{
  remove(object.link().parent(), Path(object.link().path().back()), lapl);
}

void remove(const Group &base, const Path &rel_path,
            const property::LinkAccessList &lapl)
{
  if (!base.links.exists(static_cast<std::string>(rel_path)))
  {
    std::stringstream ss;
    ss << "node::remove failed. "
       << base.link() << " / " << rel_path << " does not exist.";
    throw std::runtime_error(ss.str());
  }
  if (0 > H5Ldelete(static_cast<hid_t>(base),
                    static_cast<std::string>(rel_path).c_str(),
                    static_cast<hid_t>(lapl)))
  {
    std::stringstream ss;
    ss << "node::remove failed. Could not remove"
       << base.link() << " / " << rel_path;
    throw std::runtime_error(ss.str());
  }
}

void move(const Node &source,
          const Group &destination_base,
          const property::LinkCreationList &lcpl,
          const property::LinkAccessList &lapl)
{
  move(source,
       destination_base,
       Path(source.link().path().back()),
       lcpl, lapl);
}

void move(const Node &source,
          const Group &destination_base,
          const Path &destination_path,
          const property::LinkCreationList &lcpl,
          const property::LinkAccessList &lapl)
{
  auto name = static_cast<std::string>(destination_path);
  if (destination_base.links.exists(name))
  {
    std::stringstream ss;
    ss << "node::move failed. "
       << destination_base.link() << " / " << name << " already exists!";
    throw std::runtime_error(ss.str());
  }

  if (0 > H5Lmove(static_cast<hid_t>(source.link().parent()),
                  source.link().path().back().c_str(),
                  static_cast<hid_t>(destination_base),
                  name.c_str(),
                  static_cast<hid_t>(lcpl),
                  static_cast<hid_t>(lapl)))
  {
    std::stringstream ss;
    ss << "node::move failed. Could not move "
       << source.link() << " to "
       << destination_base.link() << " / " << destination_path;
    throw std::runtime_error(ss.str());
  }
}

} // namespace node
} // namespace hdf5




