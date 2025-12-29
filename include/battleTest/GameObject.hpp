#pragma once

#include <cstdint>
#include <memory>

namespace battleTest {

using ObjectId = uint64_t;

class GameObjectFactory;

class GameObject
{
  friend class GameObjectFactory;

public:
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

struct GameObjectDeleter
{
  void operator()(GameObject *obj) const
  {
    if (obj != nullptr) {
      obj->beforeDestroy();
      obj->destroy();
      delete obj;
    }
  }
};

using GameObjectPtr = std::unique_ptr<GameObject, GameObjectDeleter>;

}// namespace battleTest
