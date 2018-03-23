#include <iostream>
#include <vector>

#include <boost/signals2.hpp>
#include <boost/type_index.hpp>

class ConnectionGuard
{
public:
    ConnectionGuard(const boost::signals2::connection& c) : m_c(c) {}
    ~ConnectionGuard()
    {
        if (m_c.connected())
        {
            std::cout << "Disconnecting\n";
            m_c.disconnect();
        }
    }

private:
    boost::signals2::connection m_c;
};

class SafeSlot
{
public:
    void addConnection(const boost::signals2::connection& c) const { m_connections.emplace_back(c); }
private:
    mutable std::list<ConnectionGuard> m_connections;
};

void hello()
{
    std::cout << "Hello,\n";
}

class world : public SafeSlot
{
public:
    world(std::string str) : m_str(str) { std::cout << "Constructing " << m_str << std::endl; }
    ~world() { std::cout << "Destroying " << m_str << std::endl; m_str.clear(); }

    void operator ()()
    {
        std::cout << "str: " << m_str << std::endl;
    }

    void printSqr(int value)
    {
        std::cout << value * value << std::endl;
    }

private:
    std::string m_str;
};

class SafeSignaler
{
    using BaseSignalType = boost::signals2::signal<void()>;
public:
    void operator()()
    {
        sig();
    }

    template <typename SlotType>
    typename std::enable_if<!std::is_base_of<SafeSlot, typename std::remove_reference<SlotType>::type>::value, void>::type
    connect(const SlotType& slot)
    {
        std::cout << "Connecting unsafe slot\n";
        std::cout << boost::typeindex::type_id<SlotType>().pretty_name() << std::endl;

        auto c = sig.connect(slot);
        m_connections.emplace_back(std::move(c));
    }

    template <typename SlotType>
    typename std::enable_if<std::is_base_of<SafeSlot, typename std::remove_reference<SlotType>::type>::value, void>::type
    connect(const SlotType& slot)
    {
        std::cout << "Connecting safe slot\n";
        std::cout << boost::typeindex::type_id<SlotType>().pretty_name() << std::endl;

        auto c = sig.connect(std::bind(&SlotType::operator(), slot));
        slot.addConnection(c);
        m_connections.emplace_back(std::move(c));
    }

    template <typename SlotType>
    typename std::enable_if<std::is_base_of<SafeSlot, typename std::remove_reference<SlotType>::type>::value, void>::type
    connect(const std::reference_wrapper<SlotType>& slot)
    {
        std::cout << "Connecting safe slot\n";
        std::cout << boost::typeindex::type_id<SlotType>().pretty_name() << std::endl;

        auto c = sig.connect(std::bind(&SlotType::operator(), slot));
        slot.get().addConnection(c);
        m_connections.emplace_back(std::move(c));
    }

    const BaseSignalType& getSignal() const
    {
        return sig;
    }

private:
    BaseSignalType sig;
    std::list<ConnectionGuard> m_connections;
};

int main()
{
    SafeSignaler sig;
    //boost::signals2::signal<void ()> sig;
    sig.connect(&hello);
    sig.getSignal()();

    {
        world some(" some ");

        sig.connect(std::ref(some));
        sig();
        {
            world w(" World ");
            sig.connect(std::ref(w));

            sig();
        }
    }


    sig();

    world new_w(" new world ");
    sig.connect(std::ref(new_w));
    sig();

    //boost::signals2::signal<void(int)> int_sig;
    //int_sig
    //int_sig(23);

    return 0;
}
