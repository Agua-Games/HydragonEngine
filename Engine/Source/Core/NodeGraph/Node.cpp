// Node.cpp
//
// Implementation of hd::Node hierarchy and mapping functions.

#include "Node.h"
#include <algorithm>

namespace hd {

Node::Node(const std::string& name)
    : m_name(name) {
}

void Node::addChild(std::shared_ptr<Node> child) {
    if (!child) return;
    
    // If the child already has a parent, remove it first
    if (auto oldParent = child->getParent()) {
        oldParent->removeChild(child);
    }
    
    child->m_parent = std::static_pointer_cast<Node>(shared_from_this());
    m_children.push_back(child);
}

void Node::removeChild(std::shared_ptr<Node> child) {
    if (!child) return;

    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        child->m_parent.reset();
        m_children.erase(it);
    }
}

std::shared_ptr<Node> Node::getParent() const {
    return m_parent.lock();
}

void Node::bindUsdPrim(const pxr::UsdPrim& prim) {
    m_usdPrim = prim;
}

} // namespace hd
