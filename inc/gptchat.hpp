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
    GptChat()
    {}
    GptChat(const std::string&);
    GptChat(const std::string&, Callback&&);
    ~GptChat(){};

    std::string history() const;
    std::pair<std::string, std::string> output(int, Callback&&);

  private:
    const std::string question;
    std::string gptChatEnv;

    void init();
    void run(Callback&&);
};

} // namespace gpt
