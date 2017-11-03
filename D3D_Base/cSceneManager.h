#pragma once

#define g_pSceneManager cSceneManager::GetInstance()

class cGameNode;

class cSceneManager
{
private:
	SINGLETONE(cSceneManager);

public:
	typedef std::map<std::string, cGameNode*> mapSceneList;
	typedef std::map<std::string, cGameNode*>::iterator mapSceneIter;
private:
	static cGameNode* _currentScene; //���� ��
	static cGameNode* _readyScene; //��ü�غ����� ��

	mapSceneList _mSceneList; //������Ʈ
public:
	void Destroy(void);
	void Update(void);
	void Render(void);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//�� �߰��Ѵ�
	cGameNode* addScene(std::string sceneName, cGameNode* scene);

	//�� ü����
	HRESULT changeScene(std::string sceneName);
};
