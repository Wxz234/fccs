#pragma once
#include <fccs/rhi.h>
#include <dxgiformat.h>
namespace fccs {
    namespace rhi {
        struct DxgiFormatMapping
        {
            DxgiFormatMapping(Format abstractFormat, DXGI_FORMAT resourceFormat, DXGI_FORMAT srvFormat, DXGI_FORMAT rtvFormat):abstractFormat(abstractFormat), resourceFormat(resourceFormat), srvFormat(srvFormat), rtvFormat(rtvFormat) {}
            Format abstractFormat;
            DXGI_FORMAT resourceFormat;
            DXGI_FORMAT srvFormat;
            DXGI_FORMAT rtvFormat;
        };

        const DxgiFormatMapping& getDxgiFormatMapping(Format abstractFormat);
    }

}