#pragma once

template<class T>
class MUtilitySingleton
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

	MUtilitySingleton() {};
	virtual ~MUtilitySingleton() {}

private:
	MUtilitySingleton(const MUtilitySingleton&) = delete;
	MUtilitySingleton& operator=(const MUtilitySingleton&) = delete;
};