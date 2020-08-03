#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "main.h"

#include "game_object.h"

#include "camera.h"
#include "polygon.h"
#include "model.h"
#include "OutlineModel.h"
#include "DrawModel.h"
#include "field.h"
#include "Meshfield.h"
#include "Skydome.h"
#include "light.h"

class CScene
{
protected:
	std::list<CGameObject*>	m_GameObject;

public:
	CScene(){}
	virtual ~CScene(){}


	virtual void Init()
	{
		AddGameObject<CCamera>();
		AddGameObject<Light>();

		AddGameObject<Meshfield>();
		
		AddGameObject<SkyDome>();

		AddGameObject<DrawModel>();
		AddGameObject<OutlineModel>();
		
		
		//AddGameObject<CPolygon>();
	}

	virtual void Uninit()
	{
		for (CGameObject* object : m_GameObject)
		{
			object->Uninit();
			delete object;
		}
		m_GameObject.clear();
	}


	virtual void Update()
	{
		for( CGameObject* object : m_GameObject )
			object->Update();
	}


	virtual void Draw()
	{
		for (CGameObject* object : m_GameObject)
			object->Draw();
	}

	virtual void DrawShadow()
	{
		for (CGameObject* object : m_GameObject)
			object->DrawShadow();
	}


	template <typename T>
	T* AddGameObject()
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject.push_back( gameObject );

		return gameObject;
	}

	template <typename T>
	std::vector <T*> GetGameObjects()
	{
		std::vector<T*> objects;
		for (CGameObject* object : m_GameObject)
		{
			if (typeid(*object) == typeid(T))
			{
				objects.push_back((T*)object);
			}
		}

		return objects;
	}

	template <typename T>
	T* GetGameObject()
	{
		T* object;
		for (CGameObject* object : m_GameObject)
		{
			if (typeid(*object) == typeid(T))
			{
				return((T*)object);
			}
		}

	}

};