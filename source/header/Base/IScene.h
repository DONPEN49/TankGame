#pragma once
#include	<string>
#include	<unordered_map>
#include	<functional>
#include	<iostream>
#include	<memory>
#include	<directxmath.h>
#include	<chrono>
#include	<thread>
#include    <vector>
#include	"SceneManager.h"
#include	"uncopyable.h"
#include	"StopWatch.h"
#include	"quad2d.h"
#include	"Application.h"
#include    "Object.h"
#include    "Base_UI.h"

class SceneManager;

class IScene : Uncopyable {
private:
	SceneManager*	m_pManager;
protected:
	std::vector<Object*>	m_pListObject;
	std::vector<UI*>		m_pListUI;
	std::string m_tag;
public:

	const std::string GetSceneTag() const { return m_tag; };

	IScene();

	void SetSceneManager(SceneManager* sm) {
		m_pManager = sm;
	}

	virtual ~IScene() {}
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Dispose() = 0;

	virtual void updateFadeIn(double t);
	virtual void updateFadeOut(double t);

	virtual void drawFadeIn(double t);
	virtual void drawFadeOut(double t);

	void AddSceneObject(Object* _obj) {
		m_pListObject.emplace_back(_obj);
	}

	void AddSceneUI(UI* _ui) {
		m_pListUI.emplace_back(_ui);
	}

	template<typename T>
	T* GetSceneObject(std::string _tag) {
		for (Object* obj : m_pListObject) {
			if (obj->m_tag == _tag) {
				return (T*)obj;
			}
		}

		return nullptr;
	}

	template<typename T>
	std::vector<T*> GetSceneObjects(std::string _tag) {
		std::vector<T*> temp;
		for (Object* obj : m_pListObject) {
			if (obj->m_tag == _tag) {
				temp.emplace_back((T*)obj);
			}
		}
		return temp;
	}

	template<typename T>
	T* GetSceneUI(std::string _tag) {
		for (UI* ui : m_pListUI) {
			if (ui->m_tag == _tag) {
				return (T*)ui;
			}
		}

		return nullptr;
	}
};