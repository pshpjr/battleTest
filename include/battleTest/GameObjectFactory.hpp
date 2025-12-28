#pragma once

#include "battleTest/GameObject.hpp"

namespace battleTest {

class GameObjectFactory
{
public:
  static GameObjectPtr create(ObjectId objectId);

  // 추후 ObjectId에 따라 다른 GameObject 파생 클래스를 생성할 수 있음
  // static GameObjectPtr createByType(ObjectId objectId, GameObjectType type);
};

}// namespace battleTest
