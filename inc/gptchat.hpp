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
    GptChat(const std::string& question) : GptChat(question, []() {})
    {}
    GptChat(const std::string& question, Callback&& callback) :
        question{question}
    {
        init();
        run(std::move(callback));
    }
    ~GptChat(){};

    std::pair<std::string, std::string> output(int, Callback&&);

  private:
    const std::string question;
    std::string gptChatEnv;

    void init();
    void run(Callback&&);
};

} // namespace gpt
