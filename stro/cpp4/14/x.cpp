namespace X {
    int i, j, k;
}

int k;

void f1()
{
    int i = 0;

    using namespace X;
    i++;    // local i
    j++;    // X::j
    k++;    // error: X::k or ::k?
    ::k++;  // the global k;
    X::k++; // X::k
}

void f2()
{
    int i = 0;

    using X::i; // error i declared twice in f2()
    using X::j;
    using X::k; // hides global k;

    i++;
    j++;    // X::j
    k++;    // X::k
}