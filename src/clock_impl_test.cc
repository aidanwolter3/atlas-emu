#include "src/clock_impl.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/platform/platform.h"

namespace {

using std::chrono::nanoseconds;

class MockPlatform : public Platform {
 public:
  MOCK_METHOD(void, Sleep, (nanoseconds));
};

class MockTimerObserver : public Clock::TimerObserver {
 public:
  MOCK_METHOD(void, OnTimerCalled, (), (override));
};

TEST(ClockImplTest, RunZeroObservers) {
  MockPlatform platform;
  ClockImpl ClockImpl(platform);
  ClockImpl.RunForDuration(nanoseconds{10});
}

TEST(ClockImplTest, RunUntilTimer) {
  MockPlatform platform;
  MockTimerObserver observer;

  ClockImpl clock(platform);
  clock.RegisterTimerObserver(&observer, nanoseconds{10});

  EXPECT_CALL(platform, Sleep(nanoseconds{10}));
  EXPECT_CALL(observer, OnTimerCalled);
  clock.RunUntilTimer();
}

TEST(ClockImplTest, RunSingleObserver) {
  MockPlatform platform;
  MockTimerObserver observer;

  ClockImpl clock(platform);
  clock.RegisterTimerObserver(&observer, nanoseconds{10});

  EXPECT_CALL(platform, Sleep(nanoseconds{10})).Times(10);
  EXPECT_CALL(observer, OnTimerCalled).Times(10);
  clock.RunForDuration(nanoseconds{100});
}

TEST(ClockImplTest, RunMultipleObservers) {
  MockPlatform platform;
  MockTimerObserver observer1;
  MockTimerObserver observer2;

  ClockImpl clock(platform);
  clock.RegisterTimerObserver(&observer1, nanoseconds{10});
  clock.RegisterTimerObserver(&observer2, nanoseconds{15});

  EXPECT_CALL(platform, Sleep(nanoseconds{10}));
  EXPECT_CALL(observer1, OnTimerCalled);
  clock.RunForDuration(nanoseconds{10});

  EXPECT_CALL(platform, Sleep(nanoseconds{5}));
  EXPECT_CALL(observer2, OnTimerCalled);
  clock.RunForDuration(nanoseconds{5});

  EXPECT_CALL(platform, Sleep(nanoseconds{5}));
  EXPECT_CALL(observer1, OnTimerCalled);
  clock.RunForDuration(nanoseconds{5});

  EXPECT_CALL(platform, Sleep(nanoseconds{10}));
  EXPECT_CALL(observer1, OnTimerCalled);
  EXPECT_CALL(observer2, OnTimerCalled);
  clock.RunForDuration(nanoseconds{10});

  EXPECT_CALL(platform, Sleep(nanoseconds{10}));
  EXPECT_CALL(observer1, OnTimerCalled);
  clock.RunForDuration(nanoseconds{10});

  EXPECT_CALL(platform, Sleep(nanoseconds{5}));
  EXPECT_CALL(observer2, OnTimerCalled);
  clock.RunForDuration(nanoseconds{5});
}

}  // namespace
