
//    The MIT License (MIT) 
// 
//    Copyright (c) 2016 Federico Saldarini 
//    https://www.linkedin.com/in/federicosaldarini 
//    https://github.com/saldavonschwartz 
//    http://0xfede.io 
// 
// 
//    Permission is hereby granted, free of charge, to any person obtaining a copy 
//    of this software and associated documentation files (the "Software"), to deal 
//    in the Software without restriction, including without limitation the rights 
//    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
//    copies of the Software, and to permit persons to whom the Software is 
//    furnished to do so, subject to the following conditions: 
// 
//    The above copyright notice and this permission notice shall be included in all 
//    copies or substantial portions of the Software. 
// 
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
//    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
//    SOFTWARE. 

#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

namespace OXFEDE {

	template <class OwnerT, class KeyT, class SigT>
	class Event;

	template <class OwnerT, class KeyT, class RetT, class... ParamTs>
	class Event<OwnerT, KeyT, RetT(ParamTs...)> {

		friend OwnerT;

		template <class OwnerT2, class IdxT2, class KeyT2, class SigT2>
		friend class MultiEvent;

	public:
		using Delegate = std::function<RetT(ParamTs...)>;

		struct Binding {
			KeyT key;
			Delegate delegate;

			Binding(KeyT key, Delegate delegate) {
				this->key = key;
				this->delegate = delegate;
			}

			template <class T>
			Binding(KeyT key, T* target, RetT(T::*fPtr)(ParamTs...)) {
				this->key = key;
				this->delegate = [&target, fPtr](ParamTs... args) {
					return (target->*fPtr)(args...);
				};
			}
		};

		Event& operator +=(const Binding& binding) {
			bindings[binding.key] = binding.delegate;
			return *this;
		}

		Event& operator -=(KeyT key) {
			bindings.erase(key);
			return *this;
		}

	protected:
		struct Container {
			Event event;
		};

		std::unordered_map<KeyT, Delegate> bindings;

		Event() = default;
		Event(const Event& from) = default;
		Event(Event&& from) = default;
		Event& operator=(const Event& from) = default;
		Event& operator=(Event&& from) = default;
		~Event() = default;

		void operator()(ParamTs... args) {
			auto temp = bindings;
			for (auto binding : temp) {
				binding.second(args...);
			}
		}

		void operator()(std::vector<RetT>& results, ParamTs... args) {
			auto temp = bindings;
			for (auto binding : temp) {
				results.push_back(binding.second(args...));
			}
		}
	};

	template <class OwnerT, class IdxT, class KeyT, class SigT>
	class MultiEvent;

	template <class OwnerT, class IdxT, class KeyT, class RetT, class... ParamTs>
	class MultiEvent<OwnerT, IdxT, KeyT, RetT(ParamTs...)> {

		friend OwnerT;

	public:
		using EventType = Event<OwnerT, KeyT, RetT(ParamTs...)>;

		EventType& operator[](IdxT idx) {
			return events[idx].event;
		}

	protected:
		std::unordered_map<IdxT, typename EventType::Container> events;

		MultiEvent() = default;
		MultiEvent(const MultiEvent& from) = default;
		MultiEvent(MultiEvent&& from) = default;
		MultiEvent& operator=(const MultiEvent& from) = default;
		MultiEvent& operator=(MultiEvent&& from) = default;
		~MultiEvent() = default;
	};

}
