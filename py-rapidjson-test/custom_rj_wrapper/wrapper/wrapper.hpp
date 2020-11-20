//
// Created by adam on 2020. 11. 20..
//

#ifndef CPP_JSONTEST_BASICS_HPP
#define CPP_JSONTEST_BASICS_HPP

#endif // CPP_JSONTEST_BASICS_HPP

#include "rapidjson.hpp"

template<class Encoding, class Allocator>
std::string toJsonString(const rapidjson::GenericValue<Encoding, Allocator>& value) {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};
  value.Accept(writer);
  return std::string{buffer.GetString(), buffer.GetSize()};
}

template <class Allocator>
static void setMember(rapidjson::Value &node, const std::string_view name,
                      rapidjson::Value value, Allocator &alloc) {
  const auto nameRef = rapidjson::StringRef(name.data(), name.size());
  if (auto member = node.FindMember(nameRef); member != node.MemberEnd())
    member->value = std::move(value);
  else
    node.AddMember(rapidjson::Value{std::string{name}, alloc}, std::move(value),
                   alloc);
}

struct ServerResponse {
  int method;
  int seq;
  rapidjson::Document document;
  void setParameter(std::string param, rapidjson::Value value);
  rapidjson::Value &paramsMember() noexcept;
  const rapidjson::Value &paramsMember() const noexcept;
  void init(int methodCode, int seqValue);
};

struct ClientRequest {
  rapidjson::Document document;
  int method;
  int seq;
  rapidjson::Value &paramsMember() noexcept;
  const rapidjson::Value &paramsMember() const noexcept;
  const rapidjson::Value& getParameter(std::string_view name) const noexcept;
  void read(const char *message);
};