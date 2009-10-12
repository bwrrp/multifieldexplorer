#pragma once

#include <NQVTK/Rendering/SliceRenderer.h>

namespace VFE
{
	class SliceRenderer : public NQVTK::SliceRenderer
	{
	public:
		typedef NQVTK::SliceRenderer Superclass;

		SliceRenderer();

	protected:
		virtual bool Initialize();
	};
}
