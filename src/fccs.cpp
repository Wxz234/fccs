#include <fccs/core.h>
namespace fccs {
	IResource::~IResource() {

	}

	FCCS_API void DestroyResource(IResource* pResource) {
		delete pResource;
	}
}