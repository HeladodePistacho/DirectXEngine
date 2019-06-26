#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Render& ren)
{
	return ren.direct_context.Get();
}

ID3D11Device* Bindable::GetDevice(Render& ren)
{
	return ren.direct_device.Get();
}