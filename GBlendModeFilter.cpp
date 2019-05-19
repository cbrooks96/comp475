#include "GBlendMode.h"
#include "GColor.h"
#include "GFilter.h"
#include "GPixel.h"
#include "Blend.h"
#include "ColorUtils.h"

class GBlendModeFilter : public GFilter {
public:
    GBlendModeFilter(GBlendMode mode, const GColor& color) : fMode(mode), fColor(color) {}

    bool preservesAlpha() override {
        return false;
    }

    void filter(GPixel output[], const GPixel input[], int count) override {
        GPixel src = GPixel_PackARGB(fColor.fA*255, fColor.fR*fColor.fA*255, fColor.fG*fColor.fA*255, fColor.fB*fColor.fA*255);

        for(int i = 0; i < count; i++) {
            ModeProc blend = blendProc(fMode);
            output[i] = blend(src, input[i]);
        }
    }

private:
    const GBlendMode fMode;
    const GColor fColor;
};

std::unique_ptr<GFilter> GCreateBlendFilter(GBlendMode mode, const GColor& src) {
    return std::unique_ptr<GFilter>(new GBlendModeFilter(mode, src));
}
