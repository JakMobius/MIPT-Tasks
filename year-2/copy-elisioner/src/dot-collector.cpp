//
// Creat
// ed by Артем on 17.02.2022.
//

#include "dot-collector.hpp"

std::ostream &operator<<(std::ostream &stream, const DotNode &op) {
    stream << op.m_prefix << op.m_index;
    return stream;
}
