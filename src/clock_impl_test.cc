#include "src/clock_impl.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

class MockPlatformSleep : public PlatformSleep {
 public:
  MOCK_METHOD(void, Sleep, (uint64_t));
};

class MockTimerObserver : public Clock::TimerObserver {
 public:
  MOCK_METHOD(void, OnTimerCalled, (), (override));
};

TEST(ClockImplTest, RunZeroObservers) {
  MockPlatformSleep sleep;
  ClockImpl ClockImpl(sleep);
  ClockImpl.RunUntilTime(10);
}

TEST(ClockImplTest, RunSingleObserver) {
  MockPlatformSleep sleep;
  MockTimerObserver observer;

  ClockImpl clock(sleep);
  clock.RegisterTimerObserver(&observer, 10);

  EXPECT_CALL(sleep, Sleep(10)).Times(10);
  EXPECT_CALL(observer, OnTimerCalled).Times(10);
  clock.RunUntilTime(100);
}

TEST(ClockImplTest, RunMultipleObservers) {
  MockPlatformSleep sleep;
  MockTimerObserver observer1;
  MockTimerObserver observer2;

  ClockImpl clock(sleep);
  clock.RegisterTimerObserver(&observer1, 10);
  clock.RegisterTimerObserver(&observer2, 15);

  EXPECT_CALL(sleep, Sleep(10));
  EXPECT_CALL(observer1, OnTimerCalled);
  clock.RunUntilTime(10);

  EXPECT_CALL(sleep, Sleep(5));
  EXPECT_CALL(observer2, OnTimerCalled);
  clock.RunUntilTime(15);

  EXPECT_CALL(sleep, Sleep(5));
  EXPECT_CALL(observer1, OnTimerCalled);
  clock.RunUntilTime(20);

  EXPECT_CALL(sleep, Sleep(10));
  EXPECT_CALL(observer1, OnTimerCalled);
  EXPECT_CALL(observer2, OnTimerCalled);
  clock.RunUntilTime(30);

  EXPECT_CALL(sleep, Sleep(10));
  EXPECT_CALL(observer1, OnTimerCalled);
  clock.RunUntilTime(40);

  EXPECT_CALL(sleep, Sleep(5));
  EXPECT_CALL(observer2, OnTimerCalled);
  clock.RunUntilTime(45);
}

}  // namespace
