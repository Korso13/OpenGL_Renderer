#pragma once

#include "Defines.h"
#include "pch.h"

//You must control correctness of sending and reading payload yourself!
struct SubscriberEventPayload
{
	std::optional<bool> boolPayload = std::nullopt;
	std::optional<int> intPayload = std::nullopt;
	std::optional<float> floatPayload = std::nullopt;
	std::optional<std::string> stringPayload = std::nullopt;
	std::optional<vec2> vec2Payload = std::nullopt;
	std::optional<vec3> vec3Payload = std::nullopt;
	std::optional<vec4> vec4Payload = std::nullopt;
};

class Subscriber
{
public:
	class SubscriberHandler; //forward decl
	
//===================================================
// Typedefs and helper class / structs of Subscriber:
//===================================================
private:
	
	typedef std::function<void(const SubscriberEventPayload&)> SubscriberCallback;

	struct SubscriberInfo
	{
		explicit SubscriberInfo(SubscriberCallback&& _cb, const bool _bOneTimeEvent, SPTR<SubscriberHandler> _handler);

		explicit SubscriberInfo(SubscriberCallback&& _cb, const bool _bOneTimeEvent);

		SubscriberCallback cb;
		bool bOneTimeEvent = false;
		bool bNeedsDeletion = false;

		std::optional<WPTR<SubscriberHandler>> handler = std::nullopt;
	};

public:
	
	class SubscriberHandler
	{
		friend SubscriberInfo;
		friend Subscriber;
		
	public:
		explicit SubscriberHandler(Subscriber* _subscriberRef);

		void unsubscribe();

	private:

		bool m_scheduledUnsubscribe = false;
		Subscriber* m_subscriberRef = nullptr; //used to immediately flush subscription;
	};

	
//===================================================
// Subscriber Methods:
//===================================================
public:

	Subscriber() = default;
	virtual ~Subscriber() = default;

	//Subscribe to events triggered by this object. _subscriberId must be unique and taken from Object class!
	//Returns handler SPTR. If handler called unsubscribe() or was invalidated/deleted, subscription will be automatically cancelled!
	[[nodiscard]] SPTR<SubscriberHandler> subscribeToEventWithHandler(const uint32_t _subscriberId,
																	const std::string& _eventName,
																	SubscriberCallback _onTriggerCallback,
																	const bool bUseOnce = false);

	//Subscribe to events triggered by this object. _subscriberId must be unique and taken from Object class!
	//Less optimal subscription method - must manually unsubscribe before deleting subscribing object!
	void subscribeToEvent(const uint32_t _subscriberId,
							const std::string& _eventName,
							SubscriberCallback _onTriggerCallback,
							const bool _bUseOnce = false);

	//unsubscribe from event using subscriberId (from Object class)
	void unsubscribeFromEvent(const std::string& eventName, uint32_t _subscriberId);
	//unsubscribe from all events using subscriberId (from Object class). Try to avoid it (more costly than regular unsubscribeFromEvent)
	void unsubscribeFromAllEvents(uint32_t _subscriberId);

	//use it, if you need to make sure no shared pointers were caught by this object via event subscriptions!
	void flushSubscriptions() { m_subscribers.clear(); }

protected:

	//Notify subscribers of eventName and optionally pass payload
	void triggerEvent(const std::string& _eventName, SubscriberEventPayload _payload = SubscriberEventPayload());

	void checkForInvalidatedSubscriptions();

//===================================================
// Subscriber fields:
//===================================================
private:
	
	std::unordered_map<std::string, std::map<uint32_t, SubscriberInfo>> m_subscribers;
};