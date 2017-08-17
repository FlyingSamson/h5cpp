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
#include "property_list_class.hpp"

namespace hdf5 {
namespace property_list {

//=============================================================================
// private member function implementation
//=============================================================================

void GroupCreation::set_estimated_link_info_(unsigned nlinks,
                                             unsigned name_length,
                                             const std::string &error_message) const
{
  if(H5Pset_est_link_info(static_cast<hid_t>(*this),nlinks,name_length)<0)
  {
    throw std::runtime_error(error_message);
  }
}

//============================================================================
void GroupCreation::get_estimated_link_info_(unsigned &nlinks,
                                             unsigned &name_length,
                                             const std::string &error_message) const
{
  if(H5Pget_est_link_info(static_cast<hid_t>(*this),&nlinks,&name_length)<0)
  {
    throw std::runtime_error(error_message);
  }
}

//============================================================================
void GroupCreation::set_link_phase_change_(unsigned max_links,
                                           unsigned min_links,
                                           const std::string &error_message) const
{
  if(H5Pset_link_phase_change(static_cast<hid_t>(*this),max_links,min_links)<0)
  {
    throw std::runtime_error(error_message);
  }
}

//============================================================================
void GroupCreation::get_link_phase_change_(unsigned &max_links,
                                           unsigned &min_links,
                                           const std::string &error_message) const
{
  if(H5Pget_link_phase_change(static_cast<hid_t>(*this),&max_links,&min_links)<0)
  {
    throw std::runtime_error(error_message);
  }
}

//=============================================================================
// public member function implementation
//=============================================================================

GroupCreation::GroupCreation():
        List(kGroupCreate)
{}

//============================================================================
size_t GroupCreation::local_heap_size_hint() const
{
  size_t buffer = 0;
  if(H5Pget_local_heap_size_hint(static_cast<hid_t>(*this),&buffer)<0)
  {
    throw std::runtime_error("Failure reading local heap size hint from "
                             "group creation property list!");
  }
  return buffer;
}

//============================================================================
void GroupCreation::local_heap_size_hint(size_t size) const
{
  if(H5Pset_local_heap_size_hint(static_cast<hid_t>(*this),size)<0)
  {
    throw std::runtime_error("Failure setting local heap size hint in "
                             "group creation property list!");
  }
}

//============================================================================
unsigned GroupCreation::estimated_number_of_links () const
{
  unsigned nlinks = 0,
           name_length = 0;
  std::string error_message = "Failure to retrieve the estimated number of "
                              "links from a group creation property list!";
  get_estimated_link_info_(nlinks,name_length,error_message);
  return nlinks;
}

//============================================================================
void GroupCreation::estimated_number_of_links (unsigned nlinks) const
{
  std::string error_message = "Failure to set the estimated number of links "
                              " for group creation property list!";
  set_estimated_link_info_(nlinks,estimated_link_name_length(),error_message);
}

//============================================================================
unsigned GroupCreation::estimated_link_name_length() const
{
  unsigned nlinks=0,
           name_length=0;
  std::string error_message = "Failure to retrieve the estimated number of "
                              "links from a group creation property list!";
  get_estimated_link_info_(nlinks,name_length,error_message);
  return name_length;
}

//============================================================================
void GroupCreation::estimated_link_name_length (unsigned name_length) const
{
  std::string error_message = "Failure setting the estimated linke name length"
                              " for group creation property list!";
  set_estimated_link_info_(estimated_number_of_links(),name_length,
                           error_message);
}

//============================================================================
void GroupCreation::link_creation_order(LinkCreationOrder order) const
{
  if(H5Pset_link_creation_order(static_cast<hid_t>(*this),order)<0)
  {
    throw std::runtime_error("Failure to set link creation order for group "
                             "creation property list!");
  }

}

//============================================================================
LinkCreationOrder GroupCreation::link_creation_order() const
{
  unsigned order;
  if(H5Pget_link_creation_order(static_cast<hid_t>(*this),&order)<0)
  {
    throw std::runtime_error("Failure to retrieve link creation order from "
                             "group creation property list!");
  }


  return static_cast<LinkCreationOrder>(order);
}

//============================================================================
void GroupCreation::maximum_links_for_compact_group(unsigned nlinks) const
{
  std::string error_message = "Failure to set the number of maximum links "
      "for compact groups in a group creation property list!";
  set_link_phase_change_(nlinks,minimum_links_for_dense_group(),error_message);
}

//============================================================================
unsigned GroupCreation::maximum_links_for_compact_group() const
{
  unsigned max_links = 0,
           min_links = 0;
  std::string error_message = "Failure retrieving the maximum number of links "
      "for compact groups from group creation property list!";
  get_link_phase_change_(max_links,min_links,error_message);
  return max_links;
}

//============================================================================
void GroupCreation::minimum_links_for_dense_group(unsigned nlinks) const
{
  std::string error_message = "Failure to set the minimum number of links "
      "for dense groups in a group creation property list!";
  set_link_phase_change_(maximum_links_for_compact_group(),nlinks,error_message);
}

//============================================================================
unsigned GroupCreation::minimum_links_for_dense_group() const
{
  unsigned max_links = 0,
           min_links = 0;
  std::string error_message = "Failure retrieving the minimum number of links "
      "for dense groups from group creation property list!";
  get_link_phase_change_(max_links,min_links,error_message);
  return min_links;

}

} // namespace property_list
} // namespace hdf5
