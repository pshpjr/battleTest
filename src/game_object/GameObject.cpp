#include "battleTest/GameObject.hpp"

namespace battleTest {

GameObject::GameObject(ObjectId objectId) : objectId_(objectId) {}

std::unique_ptr<GameObject> GameObject::create(ObjectId objectId)
{
  return std::unique_ptr<GameObject>(new GameObject(objectId));
}

void GameObject::destroy(GameObject *obj)
{
  if (obj != nullptr) {
    obj->beforeDestroy();
    obj->destroy();
    delete obj;
  }
}

}// namespace battleTest
