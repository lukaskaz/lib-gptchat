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

class GptChatIf
{
  public:
    virtual ~GptChatIf(){};
    virtual std::pair<std::string, std::string>
        run(const std::string&, Callback&&, Callback&&, int32_t) = 0;
    virtual std::string history() const = 0;
};

class GptChat : public GptChatIf
{
  public:
    GptChat();
    ~GptChat(){};

    std::pair<std::string, std::string> run(const std::string&, Callback&&,
                                            Callback&&, int32_t) override;
    std::string history() const override;

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

    static std::shared_ptr<GptChatIf> create();
};

} // namespace gpt
