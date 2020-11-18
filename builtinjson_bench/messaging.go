package rapidjson_bench

import (
  "encoding/json"
)

type ClientRequest struct {
  method int
  seq    int
  params map[string]interface{}
}

type ServerResponse struct {
  method   int
  seq      int
  params map[string]interface{}
}

type ServiceNotification struct {
  event  string
  params map[string]interface{}
}

func (clientRequest ClientRequest) getParameter(param string) (value interface{}) {
  return clientRequest.params[param]
}

func (clientRequest *ClientRequest) load(parsedMessage map[string]interface{}) {
  clientRequest.method, _ = parsedMessage["method"].(int)
  clientRequest.seq, _ = parsedMessage["seq"].(int)
  clientRequest.params = parsedMessage["params"].(map[string]interface{})
}

func (serverResponse ServerResponse) dump() []byte {
  responseObj := map[string]interface{}{
    "method": serverResponse.method,
    "seq": serverResponse.seq,
    "params": serverResponse.params,
  }
  raw, err := json.Marshal(responseObj)
  if err != nil {
    return nil
  }
  return raw
}

func NewServerResponse(method int, seq int) ServerResponse {
  params := make(map[string]interface{})
  return ServerResponse{
    method:   method,
    seq:      seq,
    params: params,
  }
}

func (serverResponse *ServerResponse) setParameter(param string, value interface{}) {
  serverResponse.params[param] = value
}