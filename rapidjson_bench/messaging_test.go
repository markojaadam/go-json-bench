package rapidjson_bench

import (
	"github.com/amosmzhang/rapidjson"
	"testing"
)

func BenchmarkParse(b *testing.B) {
	testMsg := []byte(`{
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
  }`)
	for n := 0; n < b.N; n++ {
		jsonDoc, _ := rapidjson.NewParsedJson(testMsg)
		if jsonDoc.HasParseError() {
			panic("Parsing error")
		}
		var request ClientRequest
		request.load(*jsonDoc)
		request.getParameter("test_param_1")
		request.getParameter("test_param_2")
		request.getParameter("test_param_3")
	}
}

func BenchmarkConstruction(b *testing.B) {
	for n := 0; n < b.N; n++ {
		response := NewServerResponse(10000, 1)
		response.setParameter("test_param_1", "a")
		response.setParameter("test_param_2", false)
		response.setParameter("test_param_3", nil)
		response.dump()
	}
}
