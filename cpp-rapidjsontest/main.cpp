#include "src/rapidjson.hpp"
#include <iostream>
#include <string_view>
#include <utility>
#include <chrono>

template<class Encoding, class Allocator>
std::string toJsonString(const rapidjson::GenericValue<Encoding, Allocator>& value) {
  /*
   * Note: GCC 7.4 has a bug and fails when try to deduce
   * template parameter of rapidjson::Writer implicitly as
   * allowed in C++17.
   */
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
  rapidjson::Document &document;

  void setParameter(std::string param, rapidjson::Value value);
  rapidjson::Value &paramsMember() noexcept;
  [[nodiscard]] const rapidjson::Value &paramsMember() const noexcept;
  void init(int methodCode, rapidjson::Value params);
  const rapidjson::Value& getParameter(std::string_view name) const noexcept;
};

rapidjson::Value &ServerResponse::paramsMember() noexcept {
  return const_cast<rapidjson::Value &>(
      static_cast<const ServerResponse *>(this)->paramsMember());
}

const rapidjson::Value &ServerResponse::paramsMember() const noexcept {
  return document.FindMember("params")->value;
}

void ServerResponse::setParameter(std::string param, rapidjson::Value value) {
  setMember(paramsMember(), param, std::move(value), document.GetAllocator());
}

const rapidjson::Value& ServerResponse::getParameter(const std::string_view name) const noexcept {
  const auto nameRef = rapidjson::StringRef(name.data(), name.size());
  const auto i = paramsMember().FindMember(nameRef);
  const auto e = paramsMember().MemberEnd();
  if (i == e) {
    return core::NullJsonValue;
  } else
    return i->value;
}

void ServerResponse::init(const int methodCode, rapidjson::Value params) {
  auto& alloc = document.GetAllocator();
  document.AddMember("seq", rapidjson::Value{1}, alloc);
  document.AddMember("method", rapidjson::Value{methodCode}, alloc);
  document.AddMember("params", std::move(params), alloc);

}

struct ClientRequest {
  int method;
  int seq;
  rapidjson::Document params;
};

int main() {
  constexpr const char *msg = R"({
    "method": 10000,
        "seq": 3,
        "params": {
      "test_param_1": false,
          "test_param_2": null,
          "test_param_3": "what",
          "test_param_4": 4,
          "test_param_5": [
      1111,
          2222,
          3333
      ],
      "test_param_6": [
      false,
          true,
          false
      ],
      "test_param_7": [
      "aaa",
          "bbb",
          "ccc"
      ],
      "test_param_8": {
        "sub_param_1": 1,
            "sub_param_2": "blah",
            "nested_sub_param": {
          "nested1": "nested",
              "nested2": 1,
              "nestedList": [1, 2, 3]
        }
      }
    }
  }
)";
  rapidjson::Document document;

  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);
  document.Parse(msg);
  if (document.HasParseError()) {
//    std::cout << "oooooooooh" << std::endl;
  } else if (document.HasMember("params")) {
//    std::cout << "yeeeey" << std::endl;
  }

  using namespace std::chrono;
  auto start = high_resolution_clock::now();
  int n_cycles = 1000000;
//  for (int i = 1; i < n_cycles; i++) {
//    rapidjson::Document doc{rapidjson::Type::kObjectType};
//    auto response = ServerResponse{10000, 1, doc};
//    response.init(1000, rapidjson::Value{rapidjson::kObjectType});
//    response.setParameter("blah", rapidjson::Value("a"));
//    response.setParameter("blah2", rapidjson::Value(false));
//    response.setParameter("blah3", rapidjson::Value(true));
//    toJsonString(response.document);
//  }

  for (int i = 1; i < n_cycles; i++) {
    rapidjson::Document doc{rapidjson::Type::kObjectType};
    doc.Parse(msg);
    auto response = ServerResponse{10000, 1, doc};
    response.getParameter("test_param_1");
    response.getParameter("test_param_2");
    response.getParameter("test_param_3");
  }

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);
  auto duration_ns = duration_cast<nanoseconds>(stop - start);
  std::cout << "Elapsed: " << duration.count()/1000 << " sec\t";
  std::cout << "\t" << duration_ns.count()/n_cycles << "ns/op" << std::endl;

}