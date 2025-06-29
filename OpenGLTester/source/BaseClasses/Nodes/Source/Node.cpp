#include "../Public/Node.h"
#include "Utilities/Public/Math.h"

Node::Node(const std::string& _name)
    :
    Object(_name)
{
    
}

Node::~Node()
{
    if(m_parent)
        m_parent->removeChild(m_parent->getAllChildren().find(getName())->second);
    m_parent = nullptr;
    for(auto& [child_name, child] : m_children)
    {
        if(!child)
            continue;

        child->unsubscribeFromEvent("onNameChange", getUID());
        child->m_parent = nullptr;
    }
    m_children.clear();
}

bool Node::isInFrustum() const
{
    //todo: implement, when camera and transforms are ready
    return true;
}

void Node::addChild(const SPTR<Node>& _child, const bool _bResetTransforms )
{
    if(!_child)
    {
        std::cerr << "[Node::addChild] Passed invalid node! {" << getName() << ", id: " << std::to_string(getUID()) << "}" << std::endl;
        return;
    }

    if(_child->m_parent)
    {
        _child->m_parent->removeChild(_child);
        _child->m_parent = shared_from_this(); //todo: test this for potential deletion issues!
    }

    if(_bResetTransforms)
    {
        _child->m_transform = Transform();
    }

    m_children[_child->getName()] = _child;
    _child->subscribeToEvent(getUID(), "onNameChange", std::bind(&Node::handleChildNameChange, this, std::placeholders::_1));
}

void Node::removeChild(const SPTR<Node>& _child)
{
    if(!_child)
        return;
    
    auto child_it = m_children.find(_child->getName());
    if(child_it == m_children.end())
        return;

    (*child_it).second->unsubscribeFromEvent("onNameChange", getUID());
    m_children.erase(child_it);
}

void Node::traversal(std::function<void(SPTR<Node>)> _nodeVisitor, bool _bEnabledOnly)
{
    if(!_nodeVisitor || (_bEnabledOnly && !m_isEnabled))
        return;

    _nodeVisitor(shared_from_this());
    traverseChildren(std::move(_nodeVisitor), _bEnabledOnly);
}

void Node::traverseChildren(std::function<void(SPTR<Node>)> _nodeVisitor, bool _bEnabledOnly)
{
    for(auto& [node_name, node] : getAllChildren())
    {
        recursiveVisitor(node, _nodeVisitor, _bEnabledOnly);
    }
}

SPTR<Subscriber::SubscriberHandler>  Node::subscribeToTransformChange(const uint32_t _subscriberId, std::function<void(const Transform&)>&& _onTransformCb)
{
    return subscribeToEventWithHandler(_subscriberId, "onTransformChanged",
        [this, _onTransformCb](const SubscriberEventPayload& _payload)
        {
            _onTransformCb(m_transform);
        });
}

void Node::unsubscribeFromTransformChange(uint32_t _subscriberId)
{
    unsubscribeFromEvent("onTransformChanged", _subscriberId);
}

vec3 Node::getLocalPos() const
{
    if(m_parent)
    {
        return m_parent->getWorldPos() - getWorldPos();
    }
    else
    {
        return getWorldPos();
    }
}

vec3 Node::getLocalScale() const
{
    //todo: implement later!
    return getWorldScale();
}

void Node::setWorldPos(const vec3 _newPos)
{
    m_transform.m_translation = _newPos;
    onTransformChange();
}

void Node::setLocalPos(const vec3 _newPos)
{
    if(m_parent)
    {
        //todo: correct implementation using scale later!
        setWorldPos(m_parent->getWorldPos() + _newPos);
    }
    else
    {
        setWorldPos(_newPos);
    }
}

void Node::setWorldScale(const vec3 _newScale)
{
    m_transform.m_scale = _newScale;
    onTransformChange();
}

void Node::setLocalScale(const vec3 _newScale)
{
    //todo: implement later!
    onTransformChange();
}

void Node::onTransformChange()
{
    traverseChildren(
        [this](const SPTR<Node>& _node)
        {
            _node->setWorldPos(m_transform.m_translation + _node->getLocalPos());
            //todo: add scale transform change handling
        }
    );
    
    triggerEvent("onTransformChanged");
}

void Node::handleChildNameChange(const SubscriberEventPayload& _eventInfo)
{
    if(!_eventInfo.stringPayload.has_value())
        return;

    const auto old_name = _eventInfo.stringPayload.value();
    const auto child_it = m_children.find(_eventInfo.stringPayload.value());
    if(child_it == m_children.end())
        return;

    const SPTR<Node> child = child_it->second;
    const auto& new_name = child->getName();
    if(new_name == old_name)
        return;

    if(m_children.contains(new_name))
    {
        child->setName(new_name + "_" + std::to_string(child->getUID())); //we need unique name for the node
        return;
    }
    
    m_children[new_name] = child;
    m_children.erase(old_name);
}

void Node::recursiveVisitor(SPTR<Node> _node, std::function<void(SPTR<Node>)>& _nodeVisitor, bool _bEnabledOnly)
{
    if(!_node->isEnabled() && _bEnabledOnly)
        return;
    
    for(auto& [node_name, node] : _node->getAllChildren())
    {
        recursiveVisitor(node, _nodeVisitor);
    }
}
