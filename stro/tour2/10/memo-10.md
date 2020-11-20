# Input and Output

## Output

Note that a character is output as a character rather than as a numerical value.
The integer value of the character `'b'` is `98`, so this will output `a98c`.

```c++
void k()
{
    // note: char implicitly converted to int
    int b = 'b';
    char c = 'c';
    cout << 'a' << b << c;
}
```
