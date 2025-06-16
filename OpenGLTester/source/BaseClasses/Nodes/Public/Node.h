#pragma once
#include "Defines.h"
#include "pch.h"
#include "Utilities/Public/Utilities.h"

class Renderer;

class Node : public Object, public std::enable_shared_from_this<Node>
{
    friend Renderer;
    
public:
    explicit Node(const std::string& _name);
    Node() = delete;
    Node(const Node& origin) = delete; //node cloning is disabled for now
    void operator=(const Node& origin) = delete; //node cloning is disabled for now
    ~Node() override;
    
    virtual void setEnabled(const bool _isEnabled){m_isEnabled = _isEnabled;}
    [[nodiscard]] virtual bool isEnabled() const {return m_isEnabled;}
    virtual bool isInFrustum() const;
    
    void addChild(const SPTR<Node>& _child, bool _bResetTransforms = false);
    void removeChild(const SPTR<Node>& _child);
    SPTR<Node> getParent() {return m_parent;}
    [[nodiscard]] const std::unordered_map<std::string, SPTR<Node>>& getAllChildren() const {return m_children;}

    //iterate over node and its children with _nodeVisitor
    void traversal(std::function<void(SPTR<Node>)> _nodeVisitor, bool _bEnabledOnly = false);
    void traverseChildren(std::function<void(SPTR<Node>)> _nodeVisitor, bool _bEnabledOnly = false);
    
    const Transform& getTransform() {return m_transform;}
    void subscribeToTransformChange(const uint32_t _subscriberId, std::function<void(const Transform&)>&& _onTransformCb);
    void unsubscribeFromTransformChange(uint32_t _subscriberId);
    
    virtual [[nodiscard]] vec3 getWorldPos() const {return m_transform.getTranslation();}
    virtual [[nodiscard]] vec3 getWorldScale() const {return m_transform.getScale();}
    virtual [[nodiscard]] vec3 getLocalPos() const;
    virtual [[nodiscard]] vec3 getLocalScale() const;

    virtual void setWorldPos(const vec3 _newPos);
    virtual void setLocalPos(const vec3 _newPos);
    virtual void setWorldScale(const vec3 _newScale);
    virtual void setLocalScale(const vec3 _newScale);

protected:
    
    virtual void onTransformChange(); //todo: consider removing virtual
    
private:

    //Handles situations, where a child node changes its name
    void handleChildNameChange(const SubscriberEventPayload& _eventInfo);
    //used to recursively iterate down the Node hierarchy
    //static void recursiveVisitor(Node& _node, std::function<void(SPTR<Node>)>& _nodeVisitor);
    static void recursiveVisitor(SPTR<Node> _node, std::function<void(SPTR<Node>)>& _nodeVisitor, bool _bEnabledOnly = false);

protected:
    
    SPTR<Node> m_parent = nullptr;
    
private:

    bool m_isEnabled = true;
    Transform m_transform;
    std::unordered_map<std::string, SPTR<Node>> m_children; //consider switching to UIDs as keys if level info can be saved without it
};
