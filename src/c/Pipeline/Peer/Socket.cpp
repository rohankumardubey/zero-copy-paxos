/*

    Copyright 2017 David Turner

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

*/



#include "Pipeline/Peer/Socket.h"
#include "Pipeline/Pipe.h"
#include "Epoll.h"
#include "Paxos/Legislator.h"

#include <fcntl.h>
#include <limits.h>
#include <memory>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

namespace Pipeline {
namespace Peer {

void Socket::shutdown() {
  manager.deregister_close_and_clear(fd);
}

Socket::Socket
       (Epoll::Manager                  &manager,
        Paxos::Legislator               &legislator,
        const NodeName                  &node_name,
        const int                        fd)
  : manager         (manager),
    legislator      (legislator),
    node_name       (node_name),
    fd              (fd) {

  manager.register_handler(fd, this, EPOLLIN);

  Protocol::send_handshake(fd, node_name);
}

Socket::~Socket() {
  shutdown();
}

bool Socket::is_shutdown() const {
  return fd == -1;
}

void Socket::handle_readable() {
  assert(fd != -1);

  if (received_handshake_size < sizeof received_handshake) {
    assert(peer_id == 0);

    switch(Protocol::receive_handshake(fd,
                                       received_handshake,
                                       received_handshake_size,
                                       node_name.cluster)) {

      case RECEIVE_HANDSHAKE_ERROR:
      case RECEIVE_HANDSHAKE_EOF:
      case RECEIVE_HANDSHAKE_INVALID:
        shutdown();
        break;

      case RECEIVE_HANDSHAKE_INCOMPLETE:
        break;

      case RECEIVE_HANDSHAKE_SUCCESS:
        peer_id = received_handshake.node_id;

#ifndef NTRACE
        printf("%s (fd=%d): accepted handshake version %d cluster %s node %d\n",
          __PRETTY_FUNCTION__, fd,
          received_handshake.protocol_version,
          received_handshake.cluster_id,
          received_handshake.node_id);
#endif // ndef NTRACE
        break;
    }

    return;
  }

  fprintf(stderr, "%s: TODO\n", __PRETTY_FUNCTION__);
  abort();
}

void Socket::handle_writeable() {
  fprintf(stderr, "%s (fd=%d): unexpected\n",
                  __PRETTY_FUNCTION__, fd);
  abort();
}

void Socket::handle_error(const uint32_t events) {
  fprintf(stderr, "%s (fd=%d, events=%x): unexpected\n",
                  __PRETTY_FUNCTION__, fd, events);
  shutdown();
}

}
}
