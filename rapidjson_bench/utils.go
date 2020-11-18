package rapidjson_bench

import (
	"fmt"
	"github.com/amosmzhang/rapidjson"
)

func isTypeIn(t int, list []int) bool {
	for _, _t := range list {
		if _t == t {
			return true
		}
	}
	return false
}

func parseJSONParams(param *rapidjson.Container) (value interface{}, err error) {
	t := param.GetType()
	valueTypes := []int{rapidjson.TypeString, rapidjson.TypeNumber, rapidjson.TypeFalse, rapidjson.TypeTrue}
	if isTypeIn(t, valueTypes) {
		value, err = param.GetValue()
	} else if t == rapidjson.TypeArray {
		arr, memberType, err := param.GetArray()
		if err != nil {
			return nil, err
		}
		arrSize, err := param.GetArraySize()
		if err != nil {
			return nil, err
		}
		switch memberType {
		case rapidjson.TypeString:
			value := make([]string, arrSize)
			for i, each := range arr {
				val, _ := each.GetString()
				value[i] = val
			}
			return value, nil
		case rapidjson.TypeTrue, rapidjson.TypeFalse:
			value := make([]bool, arrSize)
			for i, each := range arr {
				val, _ := each.GetBool()
				value[i] = val
			}
			return value, nil
		case rapidjson.TypeNumber:
			value := make([]int64, arrSize)
			for i, each := range arr {
				val, _ := each.GetInt64()
				value[i] = val
			}
			return value, nil
		default:
			value := make([]interface{}, arrSize)
			for i, each := range arr {
				val, _ := each.GetValue()
				value[i] = val
			}
			return value, nil
		}
	} else if t == rapidjson.TypeObject {
		value := make(map[string]interface{})
		memberNames, _ := param.GetMemberNames()
		for _, each := range memberNames {
			member, err := param.GetMember(each)
			if err != nil {
				return nil, err
			}
			val, err := parseJSONParams(member)
			if err != nil {
				return nil, err
			}
			value[each] = val
		}
		return value, err
	}
	if err != nil {
		fmt.Println(err)
		return nil, err
	}
	return value, nil
}
