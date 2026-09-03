#include "BaseAbe.hpp"
#include "GameType.hpp"

#include "../../AliveLibAE/Abe.hpp"
#include "../../AliveLibAO/Abe.hpp"

BaseAbe::BaseAbe(s16 resourceArraySize)
 : BaseAliveGameObject(resourceArraySize)
{

}


BaseAbe* GetAbe()
{
    if (GetGameType() == GameType::eAe)
    {
        return gAbe;
    }
    else
    {
        return AO::gAbe;
    }
}
