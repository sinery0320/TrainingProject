#pragma once

using namespace ATL;

template <class T>
class CProxy_ISignalEvents : public IConnectionPointImpl<T, &__uuidof( _ISignalEvents ), CComDynamicUnkArray>
{
	// 警告:  此类可以由向导重新生成
public:
};
