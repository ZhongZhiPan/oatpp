/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef oatpp_data_mapping_type_Any_hpp
#define oatpp_data_mapping_type_Any_hpp

#include "./Type.hpp"

#include "oatpp/core/collection/LinkedList.hpp"

#include "oatpp/core/base/memory/ObjectPool.hpp"
#include "oatpp/core/base/Countable.hpp"

namespace oatpp { namespace data { namespace mapping { namespace type {

namespace __class {

  class Any {
  public:
    static const ClassId CLASS_ID;

    static Type *getType() {
      static Type type(CLASS_ID, nullptr);
      return &type;
    }

  };

}

class AnyHandle : public base::Countable {
public:

  AnyHandle(const std::shared_ptr<base::Countable>& objPtr, const Type* const objType)
    : ptr(objPtr)
    , type(objType)
  {}

  std::shared_ptr<base::Countable> ptr;
  const Type* const type;

};

class Any : public PolymorphicWrapper<AnyHandle>{
public:

  Any();

  Any(std::nullptr_t);
  Any(const AbstractObjectWrapper& polymorph);

  Any(const Any& other);
  Any(Any&& other);

  void store(const AbstractObjectWrapper& polymorph);

  const Type* const getStoredType();

  template<class WrapperType>
  WrapperType retrieve() {

    if(m_ptr) {

      if(m_ptr->type != WrapperType::Class::getType()) {
        throw std::runtime_error("[oatpp::data::mapping::type::Any::retrieve()]: Error. The value type doesn't match.");
      }

      return WrapperType(std::static_pointer_cast<typename WrapperType::ObjectType>(m_ptr->ptr), m_ptr->type);

    }

    return nullptr;

  }

  Any& operator=(std::nullptr_t);
  Any& operator=(const AbstractObjectWrapper& other);

  Any& operator=(const Any& other);
  Any& operator=(Any&& other);

  bool operator == (const Any& other);
  bool operator != (const Any& other);

};

}}}}

#endif //oatpp_data_mapping_type_Any_hpp
