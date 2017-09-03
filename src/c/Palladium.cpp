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



#include "Paxos/Palladium.h"

namespace Paxos {

Palladium::Palladium(const NodeId         id,
                     const Slot           initial_slot,
                     const Era            initial_era,
                     const Configuration &initial_configuration)
  : _node_id                (id)
  , first_unchosen_slot     (initial_slot)
  , first_inactive_slot     (initial_slot)
  , current_era             (initial_era)
  , current_configuration   (initial_configuration) {}

std::ostream& operator<<(std::ostream &o, const Palladium &palladium) {
  return palladium.write_to(o);
}

std::ostream& Palladium::write_to(std::ostream &o) const {
  o << "node_id = " << node_id() << std::endl;
  return o;
}

}
