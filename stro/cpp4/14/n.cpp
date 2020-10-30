namespace N {
    struct S { int i; };
    void f(S);
    void g(S);
    void h(int);
}

struct Base {
    void f(N::S);
};

struct D : Base {
    void mf(N::S);
    void g(N::S x)
    {
        using namespace N;
        f(x);   // call Base::f()
        mf(x);  // call D::mf()
        h(1);   // error: no h(int) available
    }
};

namespace N {
    template<typename T> void f(T, int);
    class X {};
}

namespace N2 {
    N::X x;
    void f(N::X, unsigned);
    void g()
    {
        f(x, 1);    // calls N::f(X, int);
    }
}