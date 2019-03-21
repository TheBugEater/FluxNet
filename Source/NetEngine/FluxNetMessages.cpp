#include "NetEngine/FluxNetMessages.h"

namespace Flux
{
    FLUX_BEGIN_CLASS_ROOT(MessageHeader)
        FLUX_PROPERTY(m_sequence)
        FLUX_PROPERTY(m_channel)
    FLUX_END_CLASS(MessageHeader)
}