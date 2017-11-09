#include "stdafx.h"
#include "cInventroy.h"
#include "cUIImageView.h"
#include "cTextView.h"

cInventroy::cInventroy()
	:m_pRootInvBTN(NULL)
	,m_pRootInvIMGV(NULL)
{
}


cInventroy::~cInventroy()
{
	SAFE_DELETE(m_pRootInvBTN);
	SAFE_DELETE(m_pRootInvIMGV);
}

void cInventroy::SetUp()
{
	cUIButton* InvBTN = new cUIButton;
	InvBTN->SetTexture("InventoryButton", "MYJ/InventoryButton1.bmp");
	InvBTN->SetPosition(D3DXVECTOR3(100, 600, 0));
	InvBTN->SetDelegate(this);
	InvBTN->SetTag(E_BUTTON_OK);

	cUIImageView* InvIMGV = new cUIImageView;
	InvIMGV->SetTexture("EquipmentBasic", "MYJ/WinsdowBasic2.bmp");
	InvIMGV->SetPosition(D3DXVECTOR3(350, 60, 0));

	m_pRootInvBTN = InvBTN;
	m_pRootInvIMGV = InvIMGV;

	cUIImageView* UIEquipment = new cUIImageView;
	UIEquipment->SetTexture("Equipment", "MYJ/Equipment1.bmp");
	UIEquipment->SetPosition(D3DXVECTOR3(15, 50, 0));

	cUIImageView* UIBag = new cUIImageView;
	UIBag->SetTexture("EquipmentBag", "MYJ/Equipment2.bmp");
	UIBag->SetPosition(D3DXVECTOR3(15, -20, 0));

	cUIImageView* UIInven = new cUIImageView;
	UIInven->SetTexture("Inventroy", "MYJ/InvenBasic2.png");
	UIInven->SetPosition(D3DXVECTOR3(200, 0, 0));

	cUIImageView* UIInvenIcon = new cUIImageView;
	UIInvenIcon->SetTexture("InventroyIcon", "MYJ/InventroyIcon.png");
	UIInvenIcon->SetPosition(D3DXVECTOR3(210, - 30, 0));

	cUIImageView* UIInvenGold = new cUIImageView;
	UIInvenGold->SetTexture("InventroyGold", "MYJ/InventroyGold.png");
	UIInvenGold->SetPosition(D3DXVECTOR3(220, 330, 0));

	cUIButton* closeButton = new cUIButton;
	closeButton->SetTexture("InventoryCloseButton", "MYJ/CloseButton.png");
	closeButton->SetPosition(D3DXVECTOR3(390, 10, 0));

	closeButton->SetDelegate(this);
	closeButton->SetTag(E_BUTTON_OK);

	m_pRootInvIMGV->AddChild(UIEquipment);
	m_pRootInvIMGV->AddChild(UIBag);
	m_pRootInvIMGV->AddChild(UIInven);
	m_pRootInvIMGV->AddChild(UIInvenIcon);
	m_pRootInvIMGV->AddChild(UIInvenGold);
	m_pRootInvIMGV->AddChild(closeButton);

	cTextView * pText1 = new cTextView;
	pText1->SetText("전투 소지품");
	pText1->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	pText1->SetPosition(60,	15);
	pText1->SetTag(E_TEXT_1);
	pText1->m_cFile = "Equipment";

	cTextView * pText2 = new cTextView;
	pText2->SetText("보관함");
	pText2->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	pText2->SetPosition(300, 25);
	pText2->SetTag(E_TEXT_2);
	pText2->m_cFile = "Equipment";

	cTextView * pText3 = new cTextView;
	pText3->SetText("얼마 있냐?");
	pText3->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	pText3->SetPosition(270, 340);
	pText3->SetTag(E_TEXT_3);
	pText3->m_cFile = "Equipment";


	m_pRootInvIMGV->AddChild(pText1);
	m_pRootInvIMGV->AddChild(pText2);
	m_pRootInvIMGV->AddChild(pText3);
}

void cInventroy::Update()
{
	m_pRootInvBTN->Update();
	m_pRootInvIMGV->Update();
}

void cInventroy::Render()
{
	m_pRootInvBTN->Render(m_pRootInvBTN->m_cFile, 100);
	m_pRootInvIMGV->Render(m_pRootInvIMGV->m_cFile, 255);
	
}

void cInventroy::OnClick(cUIButton * pSender)
{
	cTextView * pTextView = (cTextView*)m_pRootInvIMGV->FindChildByTag(E_TEXT_1);

	if (pSender->GetTag() == E_BUTTON_OK)
	{
		if (m_pRootInvIMGV->GetActivation())
			m_pRootInvIMGV->SetActivation(false);
		else
			m_pRootInvIMGV->SetActivation(true);
	}

}
