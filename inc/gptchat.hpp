#include <cstdint>
#include <functional>
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

} // namespace gpt
