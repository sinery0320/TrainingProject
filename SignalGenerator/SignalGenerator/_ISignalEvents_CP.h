#pragma once

using namespace ATL;

template <class T>
class CProxy_ISignalEvents : public IConnectionPointImpl<T, &__uuidof( _ISignalEvents ), CComDynamicUnkArray>
{
	// ����:  �������������������
public:
};
