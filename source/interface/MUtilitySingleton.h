#pragma once
namespace MUtility
{
	template<class T>
	class Singleton
	{
	public:
		static T*& GetInstance()
		{
			static T* instance = nullptr;

			if (instance == nullptr)
				instance = new T();

			return instance;
		}

		static void Destroy()
		{
			T*& instance = GetInstance();
			delete instance;
			instance = nullptr;
		}

		Singleton() {};
		virtual ~Singleton() {}

	private:
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
	};
}