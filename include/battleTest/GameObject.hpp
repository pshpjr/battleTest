#pragma once

#include <cstdint>
#include <memory>

namespace battleTest {

using ObjectId = uint64_t;

class GameObject
{
public:
  static std::unique_ptr<GameObject> create(ObjectId objectId);
  static void destroy(GameObject *obj);

  virtual ~GameObject() = default;

  GameObject(const GameObject &) = delete;
  GameObject &operator=(const GameObject &) = delete;
  GameObject(GameObject &&) = delete;
  GameObject &operator=(GameObject &&) = delete;

  [[nodiscard]] ObjectId getObjectId() const noexcept { return objectId_; }

  virtual void init() {}
  virtual void afterInit() {}
  virtual void beforeDestroy() {}
  virtual void destroy() {}

protected:
  explicit GameObject(ObjectId objectId);

  ObjectId objectId_;
};

}// namespace battleTest
