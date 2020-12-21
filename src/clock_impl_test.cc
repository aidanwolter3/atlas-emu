#include "src/clock_impl.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/platform/platform.h"

namespace {

class MockPlatform : public Platform {
 public:
  MOCK_METHOD(void, SleepNanoseconds, (uint64_t));
};

class MockTimerObserver : public Clock::TimerObserver {
 public:
  MOCK_METHOD(void, OnTimerCalled, (), (override));
};

TEST(ClockImplTest, RunZeroObservers) {
  MockPlatform platform;
  ClockImpl ClockImpl(platform);
  ClockImpl.RunUntilTime(10);
}

TEST(ClockImplTest, RunSingleObserver) {
  MockPlatform platform;
  MockTimerObserver observer;

  ClockImpl clock(platform);
  clock.RegisterTimerObserver(&observer, 10);

  EXPECT_CALL(platform, SleepNanoseconds(10)).Times(10);
  EXPECT_CALL(observer, OnTimerCalled).Times(10);
  clock.RunUntilTime(100);
}

TEST(ClockImplTest, RunMultipleObservers) {
  MockPlatform platform;
  MockTimerObserver observer1;
  MockTimerObserver observer2;

  ClockImpl clock(platform);
  clock.RegisterTimerObserver(&observer1, 10);
  clock.RegisterTimerObserver(&observer2, 15);

  EXPECT_CALL(platform, SleepNanoseconds(10));
  EXPECT_CALL(observer1, OnTimerCalled);
  clock.RunUntilTime(10);

  EXPECT_CALL(platform, SleepNanoseconds(5));
  EXPECT_CALL(observer2, OnTimerCalled);
  clock.RunUntilTime(15);

  EXPECT_CALL(platform, SleepNanoseconds(5));
  EXPECT_CALL(observer1, OnTimerCalled);
  clock.RunUntilTime(20);

  EXPECT_CALL(platform, SleepNanoseconds(10));
  EXPECT_CALL(observer1, OnTimerCalled);
  EXPECT_CALL(observer2, OnTimerCalled);
  clock.RunUntilTime(30);

  EXPECT_CALL(platform, SleepNanoseconds(10));
  EXPECT_CALL(observer1, OnTimerCalled);
  clock.RunUntilTime(40);

  EXPECT_CALL(platform, SleepNanoseconds(5));
  EXPECT_CALL(observer2, OnTimerCalled);
  clock.RunUntilTime(45);
}

}  // namespace
