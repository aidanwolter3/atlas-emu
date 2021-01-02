#include "src/engine/bus_impl.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/public/bus.h"

namespace {

using testing::_;
using testing::Return;

class MockPeripheral : public Peripheral {
 public:
  MockPeripheral(uint16_t length) : length_(length) {}

  MOCK_METHOD(Peripheral::Status, Read, (uint16_t address, uint8_t* data),
              (override));
  MOCK_METHOD(Peripheral::Status, Write, (uint16_t address, uint8_t data),
              (override));
  uint16_t GetAddressLength() override { return length_; }

 private:
  const uint16_t length_;
};

TEST(BusImplTest, MultiplePeripherals) {
  MockPeripheral peripheral1(10);
  MockPeripheral peripheral2(10);
  MockPeripheral peripheral3(10);
  BusImpl bus;
  bus.RegisterPeripheral(peripheral1, 0);
  bus.RegisterPeripheral(peripheral2, 10);
  bus.RegisterPeripheral(peripheral3, 40);

  Peripheral::Status status;
  EXPECT_CALL(peripheral1, Read(5, _)).WillOnce(Return(Peripheral::Status::OK));
  status = bus.Read(5, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OK);

  EXPECT_CALL(peripheral2, Read(5, _)).WillOnce(Return(Peripheral::Status::OK));
  status = bus.Read(15, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OK);

  status = bus.Read(25, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OUT_OF_BOUNDS);

  EXPECT_CALL(peripheral3, Read(5, _)).WillOnce(Return(Peripheral::Status::OK));
  status = bus.Read(45, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OK);
}

TEST(BusImplTest, RegisterPeripheralFront) {
  MockPeripheral peripheral1(10);
  MockPeripheral peripheral2(10);
  BusImpl bus;
  bus.RegisterPeripheral(peripheral1, 10);
  bus.RegisterPeripheral(peripheral2, 0);

  Peripheral::Status status;
  EXPECT_CALL(peripheral2, Read(0, _)).WillOnce(Return(Peripheral::Status::OK));
  status = bus.Read(0, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OK);

  EXPECT_CALL(peripheral1, Read(0, _)).WillOnce(Return(Peripheral::Status::OK));
  status = bus.Read(10, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OK);
}

TEST(BusImplTest, RegisterPeripheralEnd) {
  MockPeripheral peripheral1(10);
  MockPeripheral peripheral2(10);
  BusImpl bus;
  bus.RegisterPeripheral(peripheral1, 0x10000 - 20);
  bus.RegisterPeripheral(peripheral2, 0x10000 - 10);

  Peripheral::Status status;
  EXPECT_CALL(peripheral2, Read(9, _)).WillOnce(Return(Peripheral::Status::OK));
  status = bus.Read(0xFFFF, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OK);

  EXPECT_CALL(peripheral1, Read(9, _)).WillOnce(Return(Peripheral::Status::OK));
  status = bus.Read(0xFFFF - 10, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OK);
}

TEST(BusImplTest, PeripheralAddressCollision) {
  MockPeripheral peripheral1(10);
  MockPeripheral peripheral2(10);
  MockPeripheral peripheral3(10);
  BusImpl bus;
  bus.RegisterPeripheral(peripheral1, 0);
  bus.RegisterPeripheral(peripheral2, 20);

  // This is overlapping |peripheral1|, and will fail.
  bus.RegisterPeripheral(peripheral3, 5);

  Peripheral::Status status;
  status = bus.Read(15, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OUT_OF_BOUNDS);

  // This is overlapping |peripheral2|, and will fail.
  bus.RegisterPeripheral(peripheral3, 15);

  status = bus.Read(15, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OUT_OF_BOUNDS);
}

TEST(BusImplTest, PeripheralAddressTooLarge) {
  MockPeripheral peripheral(100);
  BusImpl bus;
  bus.RegisterPeripheral(peripheral, 0x10000 - 50);

  Peripheral::Status status;
  status = bus.Read(0xFFFF, nullptr);
  EXPECT_EQ(status, Peripheral::Status::OUT_OF_BOUNDS);
}

}  // namespace
