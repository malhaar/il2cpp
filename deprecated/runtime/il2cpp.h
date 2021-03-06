#pragma once

#include <stdint.h>
#include <stdlib.h>

#if defined(__clang__) || defined(__GNUC__)
#define GNU_LIKE
#elif defined(_MSC_VER)
#include <intrin.h>
#define MSVC_LIKE
#endif

#ifdef GNU_LIKE
#define IL2CPP_ATOMIC_CAS(_dst, _cmp, _new)	__sync_val_compare_and_swap(_dst, _cmp, _new)
#define IL2CPP_UNLIKELY(x)					__builtin_expect(!!(x), 0)
#else
#define IL2CPP_ATOMIC_CAS(_dst, _cmp, _new)	_InterlockedCompareExchange8((volatile char*)_dst, _new, _cmp)
#define IL2CPP_UNLIKELY(x)					x
#endif

#define IL2CPP_CALL_ONCE(_flag, _locktid, _func) \
		if (IL2CPP_UNLIKELY(_flag != -1)) \
		{ \
			if (IL2CPP_ATOMIC_CAS(&_flag, 0, 1) == 0) \
			{ \
				_locktid = il2cpp_ThreadID(); \
				_func(); \
				IL2CPP_ATOMIC_CAS(&_flag, 1, -1); \
			} \
			else if (_locktid != il2cpp_ThreadID()) \
			{ \
				while (_flag != -1) \
					il2cpp_Yield(); \
			} \
		}

#define IL2CPP_THROW(_obj) \
		throw il2cppException((il2cppObject*)_obj)

#define IL2CPP_OBJECT_BODY \
		uint32_t objectTypeID

struct il2cppObject
{
	IL2CPP_OBJECT_BODY;
};

struct il2cppString : il2cppObject
{
	int len;
	uint16_t str[1];
};

struct il2cppValueType
{
};

struct il2cppException
{
	il2cppObject* obj;

	il2cppException(il2cppObject* p) : obj(p) {}
};

il2cppObject* il2cpp_New(uint32_t sz, uint32_t typeID);
void il2cpp_Yield();
uintptr_t il2cpp_ThreadID();

il2cppString* met_icall_System_Environment__GetResourceFromDefault(il2cppString* str);
