package rapidjson_bench

import (
  //"fmt"
  "github.com/amosmzhang/rapidjson"
)

type ClientRequest struct {
  method int
  seq    int
  params rapidjson.Container
}

type ServerResponse struct {
  document *rapidjson.Doc
  method   int
  seq      int
}

type ServiceNotification struct {
  event  string
  params rapidjson.Container
}

func (clientRequest ClientRequest) getParameter(param string) (value interface{}) {
  var err error
  value, err = parseJSONParams(clientRequest.params.GetMemberOrNil(param))
  if err != nil {
    return nil
  }
  return value
}

func (clientRequest *ClientRequest) load(parsedMessage rapidjson.Doc) {
  msgContainer := parsedMessage.GetContainer()
  clientRequest.method, _ = msgContainer.GetMemberOrNil("method").GetInt()
  clientRequest.seq, _ = msgContainer.GetMemberOrNil("seq").GetInt()
  clientRequest.params = *msgContainer.GetMemberOrNil("params")
}

func (serverResponse ServerResponse) dump() []byte {
  return serverResponse.document.Bytes()
}

func NewServerResponse(method int, seq int) ServerResponse {
  doc := rapidjson.NewDoc()
  cont := doc.GetContainerNewObj()
  params := doc.NewContainerObj()
  _ = cont.AddValue("method", method)
  _ = cont.AddValue("seq", seq)
  _ = cont.AddMember("params", params)
  return ServerResponse{
    document: doc,
    method:   method,
    seq:      seq,
  }
}

func (serverResponse *ServerResponse) setParameter(param string, value interface{}) {
  var err error
  params, err := serverResponse.document.GetContainer().GetMember("params")
  if params.HasMember(param) {
    cont, _ := params.GetMember(param)
    err = cont.SetValue(value)
  } else {
    err = params.AddValue(param, value)
  }
  if err != nil {
    panic(err)
  }
}