/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright (C) 2011 - 2016                                                  *
 * Dominik Charousset <dominik.charousset (at) haw-hamburg.de>                *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#ifndef CAF_IO_NETWORK_TEST_MULTIPLEXER_HPP
#define CAF_IO_NETWORK_TEST_MULTIPLEXER_HPP

#include "caf/io/receive_policy.hpp"
#include "caf/io/abstract_broker.hpp"

#include "caf/io/network/multiplexer.hpp"

namespace caf {
namespace io {
namespace network {

class test_multiplexer : public multiplexer {
public:
  explicit test_multiplexer(actor_system* sys);

  ~test_multiplexer();

  expected<connection_handle> new_tcp_scribe(const std::string& host,
                                   uint16_t port) override;

  expected<void> assign_tcp_scribe(abstract_broker* ptr,
                                   connection_handle hdl) override;

  connection_handle add_tcp_scribe(abstract_broker*, native_socket) override;

  expected<connection_handle> add_tcp_scribe(abstract_broker* ptr,
                                             const std::string& host,
                                             uint16_t port) override;

  expected<std::pair<accept_handle, uint16_t>>
  new_tcp_doorman(uint16_t port, const char*, bool) override;

  expected<void> assign_tcp_doorman(abstract_broker* ptr,
                                    accept_handle hdl) override;

  accept_handle add_tcp_doorman(abstract_broker*, native_socket) override;

  expected<std::pair<accept_handle, uint16_t>>
  add_tcp_doorman(abstract_broker* ptr, uint16_t prt,
                  const char* in, bool reuse_addr) override;

  expected<datagram_sink_handle> new_datagram_sink(const std::string& host,
                                                   uint16_t port) override;

  expected<void> assign_datagram_sink(abstract_broker* ptr,
                                      datagram_sink_handle hdl) override;

  datagram_sink_handle add_datagram_sink(abstract_broker* ptr,
                                         native_socket fd) override;

  expected<datagram_sink_handle> add_datagram_sink(abstract_broker* ptr,
                                                   const std::string& host,
                                                   uint16_t port) override;

  expected<std::pair<datagram_source_handle, uint16_t>>
  new_datagram_source(uint16_t port, const char* in, bool reuse_addr) override;

  expected<void> assign_datagram_source(abstract_broker* ptr,
                                        datagram_source_handle hdl) override;

  datagram_source_handle add_datagram_source(abstract_broker* ptr,
                                             native_socket fd) override;

  expected<std::pair<datagram_source_handle, uint16_t>>
  add_datagram_source(abstract_broker* ptr, uint16_t port,
                      const char* in, bool reuse_addr) override;

  expected<endpoint_handle>
  new_remote_endpoint(const std::string& host, uint16_t port) override;

  expected<std::pair<endpoint_handle, uint16_t>>
  new_local_endpoint(uint16_t port, const char* in, bool reuse_addr) override;

  expected<void> assign_endpoint(abstract_broker* ptr,
                                 endpoint_handle hdl) override;

  expected<endpoint_handle> add_remote_endpoint(abstract_broker* ptr,
                                                const std::string& host,
                                                uint16_t port) override;

  expected<std::pair<endpoint_handle, uint16_t>>
  add_local_endpoint(abstract_broker* ptr, uint16_t port, const char* in,
                     bool reuse_addr) override;

  endpoint_handle add_endpoint(abstract_broker* ptr,
                               network::native_socket fd) override;


  supervisor_ptr make_supervisor() override;

  void run() override;

  void provide_scribe(std::string host, uint16_t port, connection_handle hdl);

  void provide_acceptor(uint16_t port, accept_handle hdl);

  void provide_datagram_sink(std::string host, uint16_t port,
                             datagram_sink_handle hdl);

  void provide_datagram_source(uint16_t port, datagram_source_handle hdl);

  /// A buffer storing bytes.
  using buffer_type = std::vector<char>;

  /// Models pending data on the network, i.e., the network
  /// input buffer usually managed by the operating system.
  buffer_type& virtual_network_buffer(connection_handle hdl);

  /// Returns the output buffer of the scribe identified by `hdl`.
  buffer_type& output_buffer(connection_handle hdl);

  /// Returns the output buffer of the datagram source identified by `hdl`.
  buffer_type& output_buffer(datagram_sink_handle hdl);

  /// Returns the output buffer of the datagram source identified by `hdl`.
  buffer_type& output_buffer(endpoint_handle hdl);

  /// Returns the input buffer of the scribe identified by `hdl`.
  buffer_type& input_buffer(connection_handle hdl);

  /// Returns the input buffer of the datagram source identified by `hdl`.
  buffer_type& input_buffer(datagram_source_handle hdl);

  /// Returns the input buffer of the datagram source identified by `hdl`.
  buffer_type& input_buffer(endpoint_handle hdl);

  /// Returns the configured read policy of the scribe identified by `hdl`.
  receive_policy::config& read_config(connection_handle hdl);

  /// Returns whether the scribe identified by `hdl` receives write ACKs.
  bool& ack_writes(connection_handle hdl);

  /// Returns `true` if this handle has been closed
  /// for reading, `false` otherwise.
  bool& stopped_reading(connection_handle hdl);

  /// Returns `true` if this handle is inactive, otherwise `false`.
  bool& passive_mode(connection_handle hdl);

  intrusive_ptr<scribe>& impl_ptr(connection_handle hdl);

  uint16_t& port(accept_handle hdl);

  /// Returns `true` if this handle has been closed
  /// for reading, `false` otherwise.
  bool& stopped_reading(datagram_sink_handle hdl);

  /// Returns whether the scribe identified by `hdl` receives write ACKs.
  bool& ack_writes(datagram_sink_handle hdl);

  /// Returns `true` if this handle is inactive, otherwise `false`.
  bool& passive_mode(datagram_sink_handle hdl);

  intrusive_ptr<datagram_sink>& impl_ptr(datagram_sink_handle hdl);

  uint16_t& port(datagram_sink_handle hdl);

  /// Returns `true` if this handle has been closed
  /// for reading, `false` otherwise.
  bool& stopped_reading(datagram_source_handle hdl);

  /// Returns `true` if this handle is inactive, otherwise `false`.
  bool& passive_mode(datagram_source_handle hdl);

  intrusive_ptr<datagram_source>& impl_ptr(datagram_source_handle hdl);

  uint16_t& port(datagram_source_handle hdl);

  size_t& buffer_size(datagram_source_handle hdl);

  uint16_t& local_port(endpoint_handle hdl);

  uint16_t& remote_port(endpoint_handle hdl);

  intrusive_ptr<endpoint>& impl_ptr(endpoint_handle hdl);

  /// Returns `true` if this handle has been closed
  /// for reading, `false` otherwise.
  bool& stopped_reading(endpoint_handle hdl);

  /// Returns `true` if this handle is inactive, otherwise `false`.
  bool& passive_mode(endpoint_handle hdl);

  /// Returns whether the scribe identified by `hdl` receives write ACKs.
  bool& ack_writes(endpoint_handle hdl);

  /// Returns size of the receive buffer for the next datagram.
  size_t& buffer_size(endpoint_handle hdl);

  /// Returns `true` if this handle has been closed
  /// for reading, `false` otherwise.
  bool& stopped_reading(accept_handle hdl);

  /// Returns `true` if this handle is inactive, otherwise `false`.
  bool& passive_mode(accept_handle hdl);

  intrusive_ptr<doorman>& impl_ptr(accept_handle hdl);

  /// Stores `hdl` as a pending connection for `src`.
  void add_pending_connect(accept_handle src, connection_handle hdl);

  using pending_connects_map = std::unordered_multimap<accept_handle,
                                                       connection_handle>;

  pending_connects_map& pending_connects();

  using pending_scribes_map = std::map<std::pair<std::string, uint16_t>,
                                       connection_handle>;

  bool has_pending_scribe(std::string host, uint16_t port);


  using pending_datagram_sinks_map = std::map<std::pair<std::string, uint16_t>,
                                              datagram_sink_handle>;

  bool has_pending_datagram_sink(std::string host, uint16_t port);

  using pending_datagram_sources_map
    = std::map<uint16_t, datagram_source_handle>;

  bool has_pending_datagram_source(uint16_t port);

  using pending_local_endpoints_map = std::map<uint16_t, endpoint_handle>;

  bool has_pending_local_endpoint(std::string host, uint16_t port);

  using pending_remote_endpoints_map
    = std::map<std::pair<std::string, uint16_t>, endpoint_handle>;

  bool has_pending_remote_endpoint(std::string host, uint16_t port);

  /// Accepts a pending connect on `hdl`.
  bool accept_connection(accept_handle hdl);

  /// Reads data from the external input buffer until
  /// the configured read policy no longer allows receiving.
  void read_data(connection_handle hdl);

  /// Appends `buf` to the virtual network buffer of `hdl`
  /// and calls `read_data(hdl)` afterwards.
  void virtual_send(connection_handle hdl, const buffer_type& buf);

  /// Waits until a `runnable` is available and executes it.
  void exec_runnable();

  /// Returns `true` if a `runnable` was available, `false` otherwise.
  bool try_exec_runnable();

  /// Executes all pending `runnable` objects.
  void flush_runnables();

protected:
  void exec_later(resumable* ptr) override;

private:
  using resumable_ptr = intrusive_ptr<resumable>;

  void exec(resumable_ptr& ptr);

  using guard_type = std::unique_lock<std::mutex>;

  struct scribe_data {
    buffer_type xbuf;
    buffer_type rd_buf;
    buffer_type wr_buf;
    receive_policy::config recv_conf;
    bool stopped_reading = false;
    bool passive_mode = false;
    intrusive_ptr<scribe> ptr;
    bool ack_writes = false;
  };

  struct doorman_data {
    uint16_t port;
    bool stopped_reading = false;
    bool passive_mode = false;
    intrusive_ptr<doorman> ptr;
  };

  struct datagram_sink_data {
    uint16_t port;
    buffer_type wr_buf;
    size_t buffer_size;
    bool stopped_reading = false;
    bool passive_mode = false;
    bool ack_writes = false;
    intrusive_ptr<datagram_sink> ptr;
  };

  struct datagram_source_data {
    uint16_t port;
    buffer_type rd_buf;
    size_t buffer_size;
    bool stopped_reading = false;
    bool passive_mode = false;
    intrusive_ptr<datagram_source> ptr;
  };

  struct endpoint_data {
    uint16_t local_port;
    uint16_t remote_port;
    buffer_type xbuf;
    buffer_type re_buf;
    buffer_type wr_buf;
    size_t re_buf_size;
    bool stopped_reading = false;
    bool passive_mode = false;
    intrusive_ptr<endpoint> ptr;
    bool ack_writes = false;
  };

  // guards resumables_ and scribes_
  std::mutex mx_;
  std::condition_variable cv_;
  std::list<resumable_ptr> resumables_;
  pending_scribes_map scribes_;
  pending_local_endpoints_map local_endpoints_;
  pending_remote_endpoints_map remote_endpoints_;
  pending_datagram_sinks_map datagram_sinks_;
  pending_datagram_sources_map datagram_sources_;
  std::unordered_map<uint16_t, accept_handle> doormen_;
  std::unordered_map<connection_handle, scribe_data> scribe_data_;
  std::unordered_map<accept_handle, doorman_data> doorman_data_;
  std::unordered_map<endpoint_handle, endpoint_data> endpoint_data_;
  std::unordered_map<datagram_sink_handle, datagram_sink_data> datagram_sink_data_;
  std::unordered_map<datagram_source_handle, datagram_source_data> datagram_source_data_;
  pending_connects_map pending_connects_;
};

} // namespace network
} // namespace io
} // namespace caf

#endif // CAF_IO_NETWORK_TEST_MULTIPLEXER_HPP
