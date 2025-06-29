#include "../Public/Subscriber.h"

Subscriber::SubscriberHandler::SubscriberHandler(Subscriber* _subscriberRef): m_subscriberRef(_subscriberRef)
{}

void Subscriber::SubscriberHandler::unsubscribe()
{
    m_scheduledUnsubscribe = true;
    if(m_subscriberRef)
        m_subscriberRef->checkForInvalidatedSubscriptions();
}

Subscriber::SubscriberInfo::SubscriberInfo(SubscriberCallback&& _cb, const bool _bOneTimeEvent,
    SPTR<SubscriberHandler> _handler):
    cb(_cb),
    bOneTimeEvent(_bOneTimeEvent),
    handler(_handler)
{}

Subscriber::SubscriberInfo::SubscriberInfo(SubscriberCallback&& _cb, const bool _bOneTimeEvent):
    cb(_cb),
    bOneTimeEvent(_bOneTimeEvent)
{}

Subscriber::Subscriber() = default;
Subscriber::~Subscriber() = default;

SPTR<Subscriber::SubscriberHandler> Subscriber::subscribeToEventWithHandler(const uint32_t _subscriberId,
    const std::string& _eventName, SubscriberCallback _onTriggerCallback, const bool bUseOnce)
{
    SPTR<SubscriberHandler> handler = M_SPTR<SubscriberHandler>(this);
    m_subscribers[_eventName].emplace( std::pair<uint32_t, SubscriberInfo>{
        _subscriberId,
        SubscriberInfo(
            std::move(_onTriggerCallback),
            bUseOnce,
            handler
        )
    });
		
    return handler;
}

void Subscriber::subscribeToEvent(const uint32_t _subscriberId, const std::string& _eventName,
    SubscriberCallback _onTriggerCallback, const bool _bUseOnce)
{
    m_subscribers[_eventName].emplace( std::pair<uint32_t, SubscriberInfo>{
        _subscriberId,
        SubscriberInfo(
            std::move(_onTriggerCallback),
            _bUseOnce
        )
    });
}

void Subscriber::unsubscribeFromEvent(const std::string& eventName, uint32_t _subscriberId)
{
    if (!m_subscribers.contains(eventName) || !m_subscribers[eventName].contains(_subscriberId))
        return;

    m_subscribers[eventName].erase(_subscriberId);
}

void Subscriber::unsubscribeFromAllEvents(uint32_t _subscriberId)
{
    for(auto& [event, subsInfo] : m_subscribers)
        m_subscribers[event].erase(_subscriberId);
}

void Subscriber::triggerEvent(const std::string& _eventName, SubscriberEventPayload _payload)
{
    if (!m_subscribers.contains(_eventName))
        return;

    for (auto& [subscriberId, subInfo] : m_subscribers[_eventName])
    {
        //first we check if subscription is still valid
        if(subInfo.handler) //SubscriberInfo has handler 
        {
            const WPTR<SubscriberHandler>& w_handler = (*subInfo.handler);
            if(w_handler.expired() || w_handler.lock()->m_scheduledUnsubscribe) //handler expired or ordered unsubscribing, deleting subscription
                subInfo.bNeedsDeletion = true;
        }
        if(subInfo.bNeedsDeletion)
            continue;
			
        if (subInfo.cb)
        {
            SubscriberCallback tempHolder = subInfo.cb;
            tempHolder(_payload);
            if (subInfo.bOneTimeEvent)
                subInfo.bNeedsDeletion = true;
        }
    }
    std::erase_if(m_subscribers[_eventName],
                  [this](auto& pair) {return pair.second.bNeedsDeletion; });
}

void Subscriber::checkForInvalidatedSubscriptions()
{
    for (auto& [eventName, eventSubscriptions] : m_subscribers)
    {
        std::erase_if(eventSubscriptions,
                      [this](auto& pair)
                      {
                          if(pair.second.handler) //SubscriberInfo has handler 
                          {
                              const WPTR<SubscriberHandler>& w_handler = (*pair.second.handler);
                              if(w_handler.expired() || w_handler.lock()->m_scheduledUnsubscribe) //handler expired or ordered unsubscribing, deleting subscription
                                  pair.second.bNeedsDeletion = true;
                          }
						
                          return pair.second.bNeedsDeletion;
                      }
        );
    }
}
