// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignInOverlay.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Portal/PortalManager.h"
#include "UI/API/GameSessions/JoinGame.h"
#include "UI/Portal/SignIn/ConfirmSignUpPage.h"
#include "UI/Portal/SignIn/SignInPage.h"
#include "UI/Portal/SignIn/SignUpPage.h"
#include "UI/Portal/SignIn/SuccessConfirmedPage.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	check(PortalManagerClass);
	
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);

	JoinGameWidget->Button_JoinGame->OnClicked.AddDynamic(this, &USignInOverlay::OnJoinGameButtonClicked);

	// test
	Button_SignIn_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	Button_SignUp_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	Button_ConfirmSignUp_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowConfirmSignUpPage);
	Button_SuccessConfirmed_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSuccessConfirmedPage);
	//

	// Sign In
	// 로그인
	SignInPage->Button_SignIn->OnClicked.AddDynamic(this, &USignInOverlay::SignInButtonClicked);
	// 회원가입 페이지
	SignInPage->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	// 게임 종료
	SignInPage->Button_Quit->OnClicked.AddDynamic(PortalManager, &UPortalManager::QuitGame);

	// Sign Up
	// 회원가입 요청
	SignUpPage->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::SignUpButtonClicked);
	// 뒤로가기
	SignUpPage->Button_Back->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);

	// confirm Sign Up
	// 인증 코드 확인 요청
	ConfirmSignUpPage->Button_Confirm->OnClicked.AddDynamic(this, &USignInOverlay::ConfirmSignUpButtonClicked);
	// 뒤로가기
	ConfirmSignUpPage->Button_Back->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);

	// Success Confirmed 
	SuccessConfirmedPage->Button_OK->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
}

void USignInOverlay::OnJoinGameButtonClicked()
{
	check(IsValid(PortalManager));
	check(IsValid(JoinGameWidget));
	check(IsValid(JoinGameWidget->Button_JoinGame));
	
	PortalManager->BroadcastJoinGameSessionMessage.AddDynamic(this, &USignInOverlay::UpdateJoinGameStatusMessage);
	PortalManager->JoinGameSession();

	JoinGameWidget->Button_JoinGame->SetIsEnabled(false);
}

void USignInOverlay::UpdateJoinGameStatusMessage(const FString& StatusMessage, bool bResetJoinGameButton)
{
	check(IsValid(JoinGameWidget));
	JoinGameWidget->SetStatusMessage(StatusMessage);

	if (bResetJoinGameButton)
	{
		JoinGameWidget->Button_JoinGame->SetIsEnabled(true);
	}
	
}

void USignInOverlay::ShowSignInPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(SignInPage);
}

void USignInOverlay::ShowSignUpPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignUpPage));
	WidgetSwitcher->SetActiveWidget(SignUpPage);
}

void USignInOverlay::ShowConfirmSignUpPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(ConfirmSignUpPage));
	WidgetSwitcher->SetActiveWidget(ConfirmSignUpPage);
}

void USignInOverlay::ShowSuccessConfirmedPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SuccessConfirmedPage));
	WidgetSwitcher->SetActiveWidget(SuccessConfirmedPage);
}

void USignInOverlay::SignInButtonClicked()
{
	const FString& UserName = SignInPage->TextBox_UserName->GetText().ToString();
	const FString& Password = SignInPage->TextBox_Password->GetText().ToString();
	PortalManager->SignIn(UserName, Password);
}

void USignInOverlay::SignUpButtonClicked()
{
	// 비밀번호 확인 기능은 Sign Up Page에서 수행한다.
	const FString& UserName = SignUpPage->TextBox_UserName->GetText().ToString();
	const FString& Password = SignUpPage->TextBox_Password->GetText().ToString();
	const FString& Email = SignUpPage->TextBox_Email->GetText().ToString();
	PortalManager->SignUp(UserName, Password, Email);
}

void USignInOverlay::ConfirmSignUpButtonClicked()
{
	const FString ConfirmationCode = ConfirmSignUpPage->TextBox_ConfirmationCode->GetText().ToString();
	if (ConfirmationCode.IsEmpty()  || ConfirmationCode.Len() != 6)
	{

		return;
	}
	PortalManager->Confirm(ConfirmationCode);
}
