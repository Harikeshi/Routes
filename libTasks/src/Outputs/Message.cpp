#include <Task/Outputs/Message.hpp>

namespace Outputs {
Message::Message(const unsigned int code, const std::string& type, const std::string& text)
    : code{code}, type{type}, text{text}
{
}

nlohmann::json Message::toJson() const
{
    nlohmann::json result;

    result["code"] = code;
    result["type"] = type;
    result["text"] = text;

    return result;
}
} // namespace Outputs
