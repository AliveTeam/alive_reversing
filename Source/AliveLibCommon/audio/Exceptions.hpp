#include <stdexcept>

namespace psx {

class Exception : public std::exception
{
public:
    explicit Exception(const char* msg)
        : mMsg(msg)
    {
    }

    const char* what() const throw() override
    {
        return mMsg;
    }

private:
    const char* mMsg;
};

} // namespace psx