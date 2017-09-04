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


#include "Paxos/Legislator.h"

using namespace Paxos;

Configuration create_conf();

void legislator_test() {
  uint32_t seed = rand();
  std::cout << std::endl << "legislaor_test(): seed = " << seed << std::endl;
  srand(seed);

  auto conf = create_conf();
  OutsideWorld world;
  Legislator legislator(world, 1, 0, 0, conf);
}
