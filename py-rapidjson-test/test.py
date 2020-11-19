from rapidjson import dumps as json_dump
from abc import ABC, abstractmethod

class Message(ABC):
    def __init__(self, params):
        self.params = params

    def get_parameter(self, param):
        return self.params.get(param)

    def set_parameter(self, param, value):
        self.params[param] = value

    def set_record(self, record_set):
        self.params = record_set

    def add_record(self, record_set):
        self.params.update(record_set)

    def set_datatable(self, datatable):
        self.params.update(datatable.to_dict())

    def add_record_from_table(self, table_record):
        record_set = dict((k, v[0]) for k, v in table_record.to_dict().items())
        self.params.update(record_set)

    def set_record_from_table(self, table_record):
        record_set = dict((k, v[0]) for k, v in table_record.to_dict().items())
        self.params = record_set

    def set_table_parameter(self, param, datatable):
        self.set_parameter(param, datatable.to_dict())

    @classmethod
    @abstractmethod
    def from_message(cls, message):
        raise NotImplementedError

    @abstractmethod
    def dump(self):
        raise NotImplementedError


class ClientRequest(Message):
    def __init__(self, method, seq, params):
        self.method = method
        self.seq = seq
        super().__init__(params)

    def __repr__(self):
        return '<Client Request: method={}, seq={}, params={}>'.format(self.method, self.seq, json_dump(self.params))

    @classmethod
    def from_message(cls, request):
        return cls(request['method'], request['seq'], request['params'])

    def dump(self):
        return (json_dump({
            'method': self.method,
            'seq': self.seq,
            'params': self.params
        }))

class ServerResponse(Message):
    def __init__(self, method, seq):
        self.method = method
        self.seq = seq
        super().__init__(params=dict())

    def __repr__(self):
        return '<Server Response: method={}, seq={}, params={}>'.format(self.method, self.seq, json_dump(self.params))

    @classmethod
    def from_message(cls, request):
        raise NotImplementedError

    def dump(self):
        return (json_dump({
            'method': self.method,
            'seq': self.seq,
            'params': self.params
        }))


if __name__ == "__main__":
    import time
    nowtime = time.time()
    n_cycles = 1000000
    for i in range(n_cycles):
        response = ServerResponse(method=10000, seq=1)
        response.set_parameter("blah", 1)
        response.set_parameter("what", False)
        response.set_parameter("ever", None)
        response.dump()
    elapsed = time.time() - nowtime
    print("Elapsed: %.2f sec" % elapsed)
    print("%.0d ns/op" % (elapsed/n_cycles*10**9))