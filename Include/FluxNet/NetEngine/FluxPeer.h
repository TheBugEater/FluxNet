namespace Flux
{
    enum class EChannelType
    {
        Reliable_Ordered,
        Unreliable_Unordered
    };

    struct Channel
    {
        EChannelType    ChannelType;
    };

    class Peer
    {
    private:
    };
}