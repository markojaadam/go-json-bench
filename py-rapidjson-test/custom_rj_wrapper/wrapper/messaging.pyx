cdef extern from "wrapper.cpp":
    pass

cdef extern from "wrapper.hpp":
    cdef cppclass ClientRequest:
        ClientRequest() except +
        void read(const char *message)
