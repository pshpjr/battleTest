#include "battleTest/GameObjectFactory.hpp"

namespace battleTest {

GameObjectPtr GameObjectFactory::create(ObjectId objectId)
{
  return GameObjectPtr(new GameObject(objectId));
}

}// namespace battleTest
