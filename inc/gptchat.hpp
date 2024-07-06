#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace gpt
{
using Callback = std::function<void()>;

class GptChat
{
  public:
    GptChat();
    ~GptChat(){};

    std::pair<std::string, std::string> run(const std::string&, Callback&&,
                                            Callback&&, int32_t);
    std::string history() const;

  private:
    void init();
    std::pair<std::string, std::string> output(Callback&&, int32_t);
};

class GptChatFactory
{
  public:
    GptChatFactory() = delete;
    GptChatFactory(const GptChatFactory&) = delete;
    GptChatFactory(GptChatFactory&&) = delete;
    GptChatFactory& operator=(const GptChatFactory&) = delete;
    GptChatFactory& operator=(GptChatFactory&&) = delete;

    static std::shared_ptr<GptChat> create();
};

} // namespace gpt
