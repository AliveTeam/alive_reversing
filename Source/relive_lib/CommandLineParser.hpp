#pragma once

class CommandLineParser final
{
public:
    explicit CommandLineParser(const char* pCmdLine)
        : mCmdLine(pCmdLine)
    {

    }

    bool SwitchExists(const char* cmdSwitch)
    {
        return strstr(mCmdLine, cmdSwitch) ? true : false;
    }

    bool ExtractNamePairArgument(char* pOutArgument, const char* argumentPrefix)
    {
        const char* pArg = strstr(mCmdLine, argumentPrefix);
        if (!pArg)
        {
            return false;
        }

        pArg += strlen(argumentPrefix);

        u32 len = 0;
        const char* pArgIter = pArg;
        while (*pArgIter && *pArgIter != ' ')
        {
            len++;
            pArgIter++;
        }

        if (len == 0 || len >= 254)
        {
            return false;
        }

        memcpy(pOutArgument, pArg, len);
        return true;
    }

private:
    const char* mCmdLine = nullptr;
};
