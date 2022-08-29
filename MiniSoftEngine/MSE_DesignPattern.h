#pragma once

namespace MSE
{
	template<typename T>
	class SingletonPattern
	{
	public:

		//SingletonPattern(const SingletonPattern& other) = delete;

		SingletonPattern& operator=(const SingletonPattern& other) = delete;

		static T& Instance()
		{
			static T* instance_ = new T();
			return *instance_;
		}

	protected:

		SingletonPattern(){}
	};
}