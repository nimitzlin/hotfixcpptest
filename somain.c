
#include <dlfcn.h>
#include <stdint.h>

int v1print(const char*);
int main()
{
        v1print("hello world\n");
        char* PltAddr = (char*)&v1print;
        volatile uint64_t* GotAddr = (uint64_t*)(PltAddr + *(int32_t*)(PltAddr + 2) + 6);
        void* Handle = dlopen("./lib2.so", RTLD_LAZY);
        void* V2Addr = dlsym(Handle, "v2print");
        *GotAddr = (uint64_t)V2Addr;
        v1print("hello again\n");
        return 0;
}


//gcc -shared -g -fPIC -o lib1.so lib1.c
//gcc -shared -g -fPIC -o lib2.so lib2.c

gcc -g -o main main.c lib1.so -ldl -Wl,-rpath=./
