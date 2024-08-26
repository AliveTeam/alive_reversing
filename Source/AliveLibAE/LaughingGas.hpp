#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/Layer.hpp"

class LaughingGas final : public BaseGameObject
{
public:
    LaughingGas(Layer layer, relive::Path_LaughingGas* pTlv, const Guid& tlvId);
    ~LaughingGas();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

private:
    void Init();
    void DoRender();
    bool CounterOver() const;
    f32 Calc_X(f32* a2, s32 xIndex);
    f32 Calc_Y(f32* a2, s32 yIndex);
    void UpdateGasPos();

private:
    Guid mTlvInfo;
    s16 mY = 0;
    s16 mX = 0;
    s16 mH = 0;
    s16 mW = 0;
    s16 mLaughingGasSwitchId = 0;
    FP mAmountOn = {};
    Layer mLayer = Layer::eLayer_0;

    Prim_GasEffect mGasPrim = {};

    f32 mGasY[6][6] = {};
    f32 mGasX[6][6] = {};

    u16* mpGasPixels = nullptr;

    struct Data final
    {
        f32 array_4[7];
    };

    Data mDataX[321] = {};  // (640+2)/2
    Data mDataY[121] = {}; // (480+4)/4

    s32 mWidthCount = 0;
    s32 mHeightCount = 0;
};

extern s32 gLaughingGasInstanceCount;
extern bool gLaughingGasOn;
