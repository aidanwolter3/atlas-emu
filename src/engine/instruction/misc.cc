#include "src/engine/instruction/misc.h"

#include "src/engine/base/log.h"

uint8_t NOP::Execute(uint16_t operand) { return 0; }

uint8_t BRK::Execute(uint16_t operand) {
  // The accumulator holds the test result.
  // 0 is success; everything else is failure.
  if (reg_.acc == 0) {
    LOG(TEST_PASSED) << "ACC = " << Log::Hex(reg_.acc);
  } else {
    LOG(TEST_FAILED) << "ACC = " << Log::Hex(reg_.acc);
  }
  return 0;
}
