/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright (C) 2011 - 2015                                                  *
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

#ifndef CAF_MIXIN_RESPONSE_SENDER_HPP
#define CAF_MIXIN_RESPONSE_SENDER_HPP

#include <tuple>

#include "caf/actor.hpp"
#include "caf/message.hpp"
#include "caf/duration.hpp"
#include "caf/response_handle.hpp"
#include "caf/message_priority.hpp"
#include "caf/check_typed_input.hpp"

namespace caf {
namespace mixin {

template <class Base, class Subtype, class HandleTag>
class request_sender_impl : public Base {
public:
  using response_handle_type = response_handle<Subtype, message, HandleTag>;

  /****************************************************************************
   *                            response_send(...)                            *
   ****************************************************************************/

  /// Sends `{xs...}` as a synchronous message to `dest` with priority `mp`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  template <class... Ts>
  response_handle_type request(message_priority mp, const actor& dest,
                               Ts&&... xs) {
    static_assert(sizeof...(Ts) > 0, "no message to send");
    return {dptr()->request_impl(mp, dest, std::forward<Ts>(xs)...), dptr()};
  }

  /// Sends `{xs...}` as a synchronous message to `dest`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  template <class... Ts>
  response_handle_type request(const actor& dest, Ts&&... xs) {
    return request(message_priority::normal, dest, std::forward<Ts>(xs)...);
  }

  /// Sends `{xs...}` as a synchronous message to `dest` with priority `mp`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  template <class... Sigs, class... Ts>
  response_handle<Subtype,
                  typename detail::deduce_output_type<
                    detail::type_list<Sigs...>,
                    detail::type_list<
                      typename detail::implicit_conversions<
                        typename std::decay<Ts>::type
                      >::type...>
                  >::type,
                  HandleTag>
  request(message_priority mp, const typed_actor<Sigs...>& dest, Ts&&... xs) {
    static_assert(sizeof...(Ts) > 0, "no message to send");
    using token =
      detail::type_list<
        typename detail::implicit_conversions<
          typename std::decay<Ts>::type
        >::type...>;
    token tk;
    check_typed_input(dest, tk);
    return {dptr()->request_impl(mp, dest, std::forward<Ts>(xs)...), dptr()};
  }

  /// Sends `{xs...}` as a synchronous message to `dest`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  template <class... Sigs, class... Ts>
  response_handle<Subtype,
                  typename detail::deduce_output_type<
                    detail::type_list<Sigs...>,
                    detail::type_list<
                      typename detail::implicit_conversions<
                        typename std::decay<Ts>::type
                      >::type...>
                  >::type,
                  HandleTag>
  request(const typed_actor<Sigs...>& dest, Ts&&... xs) {
    static_assert(sizeof...(Ts) > 0, "no message to send");
    using token =
      detail::type_list<
        typename detail::implicit_conversions<
          typename std::decay<Ts>::type
        >::type...>;
    token tk;
    check_typed_input(dest, tk);
    return {dptr()->request_impl(message_priority::normal,
                                 dest, std::forward<Ts>(xs)...),
            dptr()};
  }

  /****************************************************************************
   *                           timed_response_send(...)                       *
   ****************************************************************************/

  /// Sends `{xs...}` as a synchronous message to `dest` with priority `mp`
  /// and relative timeout `rtime`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  template <class... Ts>
  response_handle_type timed_request(message_priority mp, const actor& dest,
                                     const duration& rtime, Ts&&... xs) {
    static_assert(sizeof...(Ts) > 0, "no message to send");
    return {dptr()->timed_request_impl(mp, dest, rtime,
                                       std::forward<Ts>(xs)...),
            dptr()};
  }

  /// Sends `{xs...}` as a synchronous message to `dest` with
  /// relative timeout `rtime`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  template <class... Ts>
  response_handle_type timed_request(const actor& dest, const duration& rtime,
                                     Ts&&... xs) {
    return timed_request(message_priority::normal, dest, rtime,
                         std::forward<Ts>(xs)...);
  }

  /// Sends `{xs...}` as a synchronous message to `dest` with priority `mp`
  /// and relative timeout `rtime`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  template <class... Sigs, class... Ts>
  response_handle<Subtype,
                  typename detail::deduce_output_type<
                    detail::type_list<Sigs...>,
                    typename detail::implicit_conversions<
                      typename std::decay<Ts>::type
                    >::type...
                  >::type,
                  HandleTag>
  timed_request(message_priority mp, const typed_actor<Sigs...>& dest,
                const duration& rtime, Ts&&... xs) {
    static_assert(sizeof...(Ts) > 0, "no message to send");
    using token =
      detail::type_list<
        typename detail::implicit_conversions<
          typename std::decay<Ts>::type
        >::type...>;
    token tk;
    check_typed_input(dest, tk);
    return {dptr()->timed_request_impl(mp, dest, rtime,
                                       std::forward<Ts>(xs)...),
            dptr()};
  }

  /// Sends `{xs...}` as a synchronous message to `dest` with
  /// relative timeout `rtime`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  template <class... Sigs, class... Ts>
  response_handle<Subtype,
                  typename detail::deduce_output_type<
                    detail::type_list<Sigs...>,
                    typename detail::implicit_conversions<
                      typename std::decay<Ts>::type
                    >::type...
                  >::type,
                  HandleTag>
  timed_request(const typed_actor<Sigs...>& dest, const duration& rtime,
                Ts&&... xs) {
    return timed_request(message_priority::normal, dest, rtime,
                         std::forward<Ts>(xs)...);
  }

  /****************************************************************************
   *                       deprecated member functions                        *
   ****************************************************************************/

  // <backward_compatibility version="0.14.2">

  /// Sends `{xs...}` as a synchronous message to `dest` with priority `mp`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  /// @deprecated
  template <class... Ts>
  CAF_DEPRECATED response_handle_type sync_send(message_priority mp,
                                                const actor& dest,
                                                Ts&&... xs) {
    return request(std::move(mp), dest, std::forward<Ts>(xs)...);
  }

  /// Sends `{xs...}` as a synchronous message to `dest`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  /// @deprecated
  template <class... Ts>
  CAF_DEPRECATED response_handle_type sync_send(const actor& dest, Ts&&... xs) {
    return request(dest, std::forward<Ts>(xs)...);
  }

  /// Sends `{xs...}` as a synchronous message to `dest` with priority `mp`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  /// @deprecated
  template <class... Sigs, class... Ts>
  CAF_DEPRECATED response_handle<Subtype,
                                 typename detail::deduce_output_type<
                                   detail::type_list<Sigs...>,
                                   detail::type_list<
                                     typename detail::implicit_conversions<
                                       typename std::decay<Ts>::type
                                     >::type...>
                                 >::type,
                                 HandleTag>
  sync_send(message_priority mp, const typed_actor<Sigs...>& dest, Ts&&... xs) {
    return request(std::move(mp), dest, std::forward<Ts>(xs)...);
  }

  /// Sends `{xs...}` as a synchronous message to `dest`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  /// @deprecated
  template <class... Sigs, class... Ts>
  CAF_DEPRECATED response_handle<Subtype,
                                 typename detail::deduce_output_type<
                                   detail::type_list<Sigs...>,
                                   detail::type_list<
                                     typename detail::implicit_conversions<
                                       typename std::decay<Ts>::type
                                     >::type...>
                                 >::type,
                                 HandleTag>
  sync_send(const typed_actor<Sigs...>& dest, Ts&&... xs) {
    return request(dest, std::forward<Ts>(xs)...);
  }

  /// Sends `{xs...}` as a synchronous message to `dest` with priority `mp`
  /// and relative timeout `rtime`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  /// @deprecated
  template <class... Sigs, class... Ts>
  CAF_DEPRECATED response_handle<Subtype,
                                 typename detail::deduce_output_type<
                                   detail::type_list<Sigs...>,
                                   typename detail::implicit_conversions<
                                     typename std::decay<Ts>::type
                                   >::type...
                                 >::type,
                                 HandleTag>
  timed_sync_send(message_priority mp, const typed_actor<Sigs...>& dest,
                  const duration& rtime, Ts&&... xs) {
    return timed_request(std::move(mp), dest, rtime, std::forward<Ts>(xs)...);
  }

  /// Sends `{xs...}` as a synchronous message to `dest` with
  /// relative timeout `rtime`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  /// @deprecated
  template <class... Sigs, class... Ts>
  CAF_DEPRECATED response_handle<Subtype,
                                 typename detail::deduce_output_type<
                                   detail::type_list<Sigs...>,
                                   typename detail::implicit_conversions<
                                     typename std::decay<Ts>::type
                                   >::type...
                                 >::type,
                                 HandleTag>
  timed_sync_send(const typed_actor<Sigs...>& dest, const duration& rtime,
                  Ts&&... xs) {
    return timed_request(dest, rtime, std::forward<Ts>(xs)...);
  }

  /// Sends `{xs...}` as a synchronous message to `dest` with priority `mp`
  /// and relative timeout `rtime`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  /// @deprecated
  template <class... Ts>
  CAF_DEPRECATED response_handle_type timed_sync_send(message_priority mp,
                                                      const actor& dest,
                                                      const duration& rtime,
                                                      Ts&&... xs) {
    return timed_request(std::move(mp), dest, rtime, std::forward<Ts>(xs)...);
  }

  /// Sends `{xs...}` as a synchronous message to `dest` with
  /// relative timeout `rtime`.
  /// @returns A handle identifying a future-like handle to the response.
  /// @warning The returned handle is actor specific and the response to the
  ///          sent message cannot be received by another actor.
  /// @throws std::invalid_argument if `dest == invalid_actor`
  /// @deprecated
  template <class... Ts>
  CAF_DEPRECATED response_handle_type timed_sync_send(const actor& dest,
                                                      const duration& rtime,
                                                      Ts&&... xs) {
    return timed_request(dest, rtime, std::forward<Ts>(xs)...);
  }

  // </backward_compatibility>

private:
  Subtype* dptr() {
    return static_cast<Subtype*>(this);
  }
};

template <class ResponseHandleTag>
class request_sender {
public:
  template <class Base, class Subtype>
  using impl = request_sender_impl<Base, Subtype, ResponseHandleTag>;
};

} // namespace mixin
} // namespace caf

#endif // CAF_MIXIN_RESPONSE_SENDER_HPP