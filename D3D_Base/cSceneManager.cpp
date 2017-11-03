#include "stdafx.h"
#include "cSceneManager.h"
#include "cMainGame.h"

cGameNode* cSceneManager::_currentScene = NULL;
cGameNode* cSceneManager::_readyScene = NULL;

cSceneManager::cSceneManager()
{
	_currentScene = NULL;
	_readyScene = NULL;
}

cSceneManager::~cSceneManager()
{
}

void cSceneManager::Destroy(void)
{
	//�ش�Ű �˻��ϰ�..
	mapSceneIter iter = _mSceneList.begin();

	//�� ��ü�� ���鼭 �����ش�
	for (; iter != _mSceneList.end();)
	{
		//�������� �ݺ��� ������Ű�� �ʴ´�
		if (iter->second != NULL)
		{
			SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else
		{
			//�ݺ��� ����..
			++iter;
		}
	}

	_mSceneList.clear();
}

void cSceneManager::Update(void)
{
	if (_currentScene) _currentScene->Update();
}

void cSceneManager::Render(void)
{
	if (_currentScene) _currentScene->Render();
}

void cSceneManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_currentScene) _currentScene->WndProc(hWnd, message, wParam, lParam);
}

cGameNode* cSceneManager::addScene(std::string sceneName, cGameNode* scene)
{
	if (!scene) return NULL;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

HRESULT cSceneManager::changeScene(std::string sceneName)
{
	//�ٲ� �� ã��...
	mapSceneIter find = _mSceneList.find(sceneName);

	//��ã���� �޽�
	if (find == _mSceneList.end()) return E_FAIL;

	//�ٲ� �� �ʱ�ȭ
	if (SUCCEEDED(find->second->Setup()))
	{
		//���� ���� ������ ������
		if (_currentScene) _currentScene->~cGameNode();
		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}
