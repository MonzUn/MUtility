#pragma once
#include "Handle.h"
#include <unordered_map>

template <typename Tag, typename CallbackReturnType, typename... CallbackParameters>
class CallbackRegister {
public:
	CallbackRegister() = default;
	CallbackRegister& operator=(const CallbackRegister& other) = delete;
	CallbackRegister(const CallbackRegister& other) = delete;

	typedef class Handle<Tag, int, -1> Handle;
	typedef struct HandleHasher<Tag, int, -1> HandleHasher;
	typedef std::function<CallbackReturnType(CallbackParameters...)> CallbackFunction;
	typedef std:: unordered_map<Handle, std::function<CallbackReturnType(CallbackParameters...)>, HandleHasher> CallbackUnorderedMap;

	~CallbackRegister() {
		//if (m_Callbacks.size() > 0) {
		//	std::cout << "Callback register still has registered callbacks. Please unregister your callbacks. Handles: ";
		//	for (auto& callback : m_Callbacks) {
		//		std::cout << static_cast<int>(callback.first) << ", ";
		//	}
		//	std::cout << std::endl;
		//}
	}

	Handle RegisterCallback(std::function<CallbackReturnType(CallbackParameters...)> callbackFunction) {
		Handle t = static_cast<Handle>(m_HandleIncrementor++);
		m_Callbacks.emplace(t, callbackFunction);
		return t;
	}

	bool UnregisterCallback(Handle& handle, bool silentLog = false) {
		if (handle == Handle::invalid()) {
			if (!silentLog) {
				//Logger::Log("Failed to unregister callback, handle set to invalid. Has it already been unregistered?", "CallbackRegister", LogSeverity::WARNING_MSG);
			}
			return false;
		}
		auto it = m_Callbacks.find(handle);
		if (it == m_Callbacks.end()) {
			if (!silentLog) {
				//Logger::Log("Failed to unregister callback with handle: " + rToString(static_cast<int>(handle)) + ". Handle not found", "CallbackRegister", LogSeverity::WARNING_MSG);
			}
			handle = Handle::invalid();
			return false;
		}
		else {
			m_Callbacks.erase(it);
			return true;
		}
	}

	void TriggerCallbacks(CallbackParameters... parameters) {
		for (auto& callback : m_Callbacks) {
			callback.second(parameters...);
		}
	}

	const CallbackUnorderedMap& GetUnorderedMap() const {
		return m_Callbacks;
	}

private:
	CallbackUnorderedMap m_Callbacks;
	int m_HandleIncrementor = 0;
};