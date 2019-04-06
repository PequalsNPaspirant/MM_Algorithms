




/*
TODO:

// minimal C++11 allocator with debug output
template <class Tp>
struct NAlloc {
typedef Tp value_type;
NAlloc() = default;
template <class T> NAlloc(const NAlloc<T>&) {}
Tp* allocate(std::size_t n) {
n *= sizeof(Tp);
std::cout << "allocating " << n << " bytes\n";
return static_cast<Tp*>(::operator new(n));
}
void deallocate(Tp* p, std::size_t n) {
std::cout << "deallocating " << n*sizeof*p << " bytes\n";
::operator delete(p);
}
};
template <class T, class U>
bool operator==(const NAlloc<T>&, const NAlloc<U>&) { return true; }
template <class T, class U>
bool operator!=(const NAlloc<T>&, const NAlloc<U>&) { return false; }

int main()
{
int sz = 100;
std::cout << "using reserve: \n";
{
std::vector<int, NAlloc<int>> v1;
v1.reserve(sz);
for(int n = 0; n < sz; ++n)
v1.push_back(n);
}
std::cout << "not using reserve: \n";
{
std::vector<int, NAlloc<int>> v1;
for(int n = 0; n < sz; ++n)
v1.push_back(n);
}
}
Possible output:
using reserve:
allocating 400 bytes
deallocating 400 bytes
not using reserve:
allocating 4 bytes
allocating 8 bytes
deallocating 4 bytes
allocating 16 bytes
deallocating 8 bytes
allocating 32 bytes
deallocating 16 bytes
allocating 64 bytes
deallocating 32 bytes
allocating 128 bytes
deallocating 64 bytes
allocating 256 bytes
deallocating 128 bytes
allocating 512 bytes
deallocating 256 bytes
deallocating 512 bytes
*/