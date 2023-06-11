#include <iostream>
#include <windows.h>

#define ADDIN_API __declspec(dllexport)

typedef struct tagDEBUGHELPER
{
    DWORD dwVersion;
    HRESULT(WINAPI* ReadDebuggeeMemory)(struct tagDEBUGHELPER* pThis, DWORD dwAddr, DWORD nWant, VOID* pWhere, DWORD* nGot);
    // from here only when dwVersion >= 0x20000
    DWORDLONG(WINAPI* GetRealAddress)(struct tagDEBUGHELPER* pThis);
    HRESULT(WINAPI* ReadDebuggeeMemoryEx)(struct tagDEBUGHELPER* pThis, DWORDLONG qwAddr, DWORD nWant, VOID* pWhere, DWORD* nGot);
    int (WINAPI* GetProcessorType)(struct tagDEBUGHELPER* pThis);
} DEBUGHELPER;

typedef HRESULT(WINAPI* CUSTOMVIEWER)(DWORD dwAddress, DEBUGHELPER* pHelper, int nBase, BOOL bUniStrings, char* pResult, size_t max, DWORD reserved);

extern "C" ADDIN_API HRESULT MyClassFormatter(DWORD dwAddress, DEBUGHELPER * pHelper, int nBase, BOOL bUniStrings, char* pResult, size_t max, DWORD reserved);

class MyClass
{
public: 
    virtual int getValue() { return value1_; }

    static void initOffsets();
    static long long offset_value1_;
    static long long offset_value2_;

private:
    int value1_;
    int value2_;
};

long long MyClass::offset_value1_ = -55;
long long MyClass::offset_value2_ = -57;

void
MyClass::initOffsets()
{
    MyClass c1;
    offset_value1_ = (char*)(&(c1.value1_)) - (char*)(&c1);
    offset_value2_ = (char*)(&(c1.value2_)) - (char*)(&c1);
}

template <typename T>
const T& getValue(void* c, long long offset)
{
    return *(T*)((char*)(c) + offset);
}

// Pour que ça marche dans 
// Tools > Options > Debugging, and then deselecting Load debug symbols in external process (native only).
// Cf: https://learn.microsoft.com/en-us/visualstudio/debugger/visual-cpp-custom-visualizer-compatibility?view=vs-2022
// Pour debugger dans  Tools > Options > Debugging : output windows : natvis diagnostic message: verbose
ADDIN_API HRESULT MyClassFormatter(DWORD dwAddress, DEBUGHELPER* pHelper, int nBase, BOOL bUniStrings, char* pResult, size_t max, DWORD reserved)
{
    static bool init = false;
    if (!init)
    {
        MyClass::initOffsets();
        init = true;
    }
    
    MyClass c;
    DWORD nGot;
    auto realAddress = pHelper->GetRealAddress(pHelper);
    pHelper->ReadDebuggeeMemoryEx(pHelper, realAddress, sizeof(MyClass), &c, &nGot);
    int value1 = getValue<int>(&c, MyClass::offset_value1_);
    int value2 = getValue<int>(&c, MyClass::offset_value2_);
    sprintf_s(pResult, max, "Dll MyClass: max=%llu nGot=%d MyClass=%llx Value1=%d Value2=%d o1=%lld o2=%lld",
        max, nGot, realAddress, value1, value2,
        MyClass::offset_value1_, MyClass::offset_value2_);
    return S_OK;
}