#include <iostream>
#include <memory>
#include <vector>
#include <string>

class IEditAction
{
public:
    virtual ~IEditAction() = default;
    virtual void apply(std::string&) = 0;
    virtual void undo(std::string&) = 0;
};

using ActionPtr = std::shared_ptr<IEditAction>;

class ReverseAction : public IEditAction
{
public:
    ReverseAction(const ActionPtr& action)
        : m_action(action)
    {}

    void apply(std::string& data) override
    {
        m_action->undo(data);
    }

    void undo(std::string& data) override
    {
        m_action->apply(data);
    }

private:
    ActionPtr m_action;
};

class AppendStr : public IEditAction
{
public:
    AppendStr(const std::string& str)
        : m_str(str)
    {}

    void apply(std::string& data) override
    {
        data += m_str;
    }

    void undo(std::string& data) override
    {
        data.erase(data.end() - m_str.size(), data.end());
    }

private:
    std::string m_str;
};

class CTextEditor
{
public:
    void append(const std::string& s)
    {
        auto action = std::make_shared<AppendStr>(s);
        action->apply(m_data);
        m_action_chain.push_back(action);
    }

    void remove(int count)
    {
        auto append_action = std::make_shared<AppendStr>(
                m_data.substr(m_data.size() - count));
        auto remove_action = std::make_shared<ReverseAction>(append_action);

        remove_action->apply(m_data);
        m_action_chain.push_back(remove_action);
    }

    char get(int index) const
    {
        return m_data[index];
    }

    void undo()
    {
        m_action_chain.back()->undo(m_data);
        m_action_chain.pop_back();
    }

private:
    std::vector<ActionPtr> m_action_chain;
    std::string m_data;
};

int main()
{
    int n;
    std::cin >> n;

    CTextEditor editor;

    for (int i = 0; i < n; ++i)
    {
        int action;
        std::cin >> action;

        switch (action)
        {
            case 1: // append
                {
                    std::string substr;
                    std::cin >> substr;
                    editor.append(substr);
                }
                break;
            case 2: // remove
                {
                    int len;
                    std::cin >> len;
                    editor.remove(len);
                }
                break;
            case 3: // print
                {
                    int k;
                    std::cin >> k;
                    char c = editor.get(k - 1);
                    std::cout << c << std::endl;
                }
                break;
            case 4:
                editor.undo();
                break;
            default:
                break;
        };
    }

    return 0;
}
