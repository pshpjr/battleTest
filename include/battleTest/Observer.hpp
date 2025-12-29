#pragma once

#include <vector>
#include <algorithm>
#include <memory>

namespace battleTest {

// 범용 옵저버 인터페이스
// EventType: 전달될 이벤트 타입
template<typename EventType>
class Observer
{
public:
  virtual ~Observer() = default;

  // 이벤트 발생 시 호출됨
  virtual void onNotify(const EventType &event) = 0;
};

// 옵저버를 관리하는 Subject 클래스
// EventType: 전달될 이벤트 타입
template<typename EventType>
class Subject
{
public:
  using ObserverPtr = Observer<EventType> *;

  Subject() = default;
  virtual ~Subject() = default;

  Subject(const Subject &) = delete;
  Subject &operator=(const Subject &) = delete;
  Subject(Subject &&) = default;
  Subject &operator=(Subject &&) = default;

  // 옵저버 등록
  void addObserver(ObserverPtr observer)
  {
    if (observer != nullptr) {
      // 중복 등록 방지
      if (std::find(observers_.begin(), observers_.end(), observer) == observers_.end()) {
        observers_.push_back(observer);
      }
    }
  }

  // 옵저버 제거
  void removeObserver(ObserverPtr observer)
  {
    observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
  }

  // 모든 옵저버 제거
  void clearObservers() { observers_.clear(); }

  // 등록된 옵저버 수
  [[nodiscard]] size_t observerCount() const noexcept { return observers_.size(); }

protected:
  // 모든 옵저버에게 이벤트 알림
  void notify(const EventType &event)
  {
    // 알림 중 옵저버 리스트가 변경될 수 있으므로 복사본 사용
    auto observersCopy = observers_;
    for (auto *observer : observersCopy) {
      if (observer != nullptr) { observer->onNotify(event); }
    }
  }

private:
  std::vector<ObserverPtr> observers_;
};

}// namespace battleTest
