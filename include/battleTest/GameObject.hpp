#pragma once

#include <cstdint>

namespace battleTest {

class GameObject
{
public:
  explicit GameObject(uint64_t objectId);
  virtual ~GameObject() = default;

  GameObject(const GameObject &) = delete;
  GameObject &operator=(const GameObject &) = delete;
  GameObject(GameObject &&) = delete;
  GameObject &operator=(GameObject &&) = delete;

  [[nodiscard]] uint64_t getObjectId() const noexcept { return objectId_; }

  virtual void init() {}
  virtual void afterInit() {}
  virtual void beforeDestroy() {}
  virtual void destroy() {}

protected:
  uint64_t objectId_;
};

}// namespace battleTest
