namespace His_string {
    class String {};
    template<typename T> class Vector {};
    
    String operator+(const String&, const String&);
    String operator+(const String&, const char*);
    void fill(char);
}

namespace His_lib {
    class String {};
    template<typename T> class Vector {};
}

namespace Her_vector {
    template<typename T> class Vector {};
    class String {};
}

namespace Her_lib {
    template<typename T> class Vector {};
    class String;
}

namespace My_lib {
    using namespace His_string;
    using namespace Her_vector;

    void my_fct(String&);
}

namespace Lib2 {
    using namespace His_lib;    // everything from His_lib
    using namespace Her_lib;    // everything from Her_lib
    using His_lib::String;  // resolve potential clash
    using Her_lib::Vector;  // resolve potential clash
    using Her_string = Her_lib::String;
    template<typename T> using His_vec = His_lib::Vector<T>;
}

void f()
{
    My_lib::String s = "Byron"; // finds My_lib::His_string::String
}

using namespace My_lib;

void g(Vector<String>& vs)
{
    my_fct(vs[5]);
}