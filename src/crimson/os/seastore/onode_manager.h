// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:nil -*-
// vim: ts=8 sw=2 smarttab

#pragma once

#include <iostream>

#include <boost/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include <seastar/core/future.hh>

#include "include/buffer_fwd.h"
#include "include/ceph_assert.h"
#include "common/hobject.h"

#include "crimson/os/seastore/onode.h"
#include "crimson/os/seastore/seastore_types.h"
#include "crimson/os/seastore/transaction_manager.h"
#include "crimson/osd/exceptions.h"

namespace crimson::os::seastore {

class OnodeManager {
  using base_ertr = TransactionManager::base_ertr;
public:
  using mkfs_ertr = TransactionManager::mkfs_ertr;
  using mkfs_ret = mkfs_ertr::future<>;
  virtual mkfs_ret mkfs(Transaction &t) = 0;

  using contains_onode_ertr = base_ertr;
  using contains_onode_ret = contains_onode_ertr::future<bool>;
  virtual contains_onode_ret contains_onode(
    Transaction &trans,
    const ghobject_t &hoid) = 0;

  using get_onode_ertr = base_ertr::extend<
    crimson::ct_error::enoent>;
  using get_onode_ret = get_onode_ertr::future<
    OnodeRef>;
  virtual get_onode_ret get_onode(
    Transaction &trans,
    const ghobject_t &hoid) = 0;

  using get_or_create_onode_ertr = base_ertr::extend<
    crimson::ct_error::value_too_large>;
  using get_or_create_onode_ret = get_or_create_onode_ertr::future<
    OnodeRef>;
  virtual get_or_create_onode_ret get_or_create_onode(
    Transaction &trans,
    const ghobject_t &hoid) = 0;

  using get_or_create_onodes_ertr = base_ertr::extend<
    crimson::ct_error::value_too_large>;
  using get_or_create_onodes_ret = get_or_create_onodes_ertr::future<
    std::vector<OnodeRef>>;
  virtual get_or_create_onodes_ret get_or_create_onodes(
    Transaction &trans,
    const std::vector<ghobject_t> &hoids) = 0;

  using write_dirty_ertr = base_ertr;
  using write_dirty_ret = write_dirty_ertr::future<>;
  virtual write_dirty_ret write_dirty(
    Transaction &trans,
    const std::vector<OnodeRef> &onodes) = 0;

  using erase_onode_ertr = base_ertr;
  using erase_onode_ret = erase_onode_ertr::future<>;
  virtual erase_onode_ret erase_onode(
    Transaction &trans,
    OnodeRef &onode) = 0;

  using list_onodes_ertr = base_ertr;
  using list_onodes_bare_ret = std::tuple<std::vector<ghobject_t>, ghobject_t>;
  using list_onodes_ret = list_onodes_ertr::future<list_onodes_bare_ret>;
  virtual list_onodes_ret list_onodes(
    Transaction &trans,
    const ghobject_t& start,
    const ghobject_t& end,
    uint64_t limit) = 0;

  virtual ~OnodeManager() {}
};
using OnodeManagerRef = std::unique_ptr<OnodeManager>;

}
