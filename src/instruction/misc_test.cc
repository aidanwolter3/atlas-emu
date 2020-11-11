#include "src/instruction/misc.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/memory.h"
#include "src/registers.h"

namespace {

class MockMemory : public Memory {
 public:
  MOCK_METHOD2(Read, Memory::Status(uint16_t address, uint8_t* data));
  MOCK_METHOD2(Write, Memory::Status(uint16_t address, uint8_t data));
};

TEST(MiscTest, NoOperation) {
  MockMemory mem;
  Registers reg;
  NOP nop(mem, reg);

  reg.pc = 0;
  reg.status = 0;
  nop.Execute(0x78);
  EXPECT_EQ(reg.pc, 0);
  EXPECT_EQ(reg.status, 0);
}

}  // namespace
