// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignInOverlay.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Portal/PortalManager.h"
#include "UI/API/GameSessions/JoinGame.h"
#include "UI/Portal/SignIn/ConfirmSignUpPage.h"
#include "UI/Portal/SignIn/SignInPage.h"
#include "UI/Portal/SignIn/SignInQuitPage.h"
#include "UI/Portal/SignIn/SignUpPage.h"
#include "UI/Portal/SignIn/SuccessConfirmedPage.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true); // 키보드 입력 포커스
	
	check(PortalManagerClass);
	
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);
	
#if WITH_EDITORONLY_DATA
	Button_SignIn_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	Button_SignUp_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	Button_ConfirmSignUp_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowConfirmSignUpPage);
	Button_SuccessConfirmed_Test->OnClicked.AddDynamic(this, &USignInOverlay::ShowSuccessConfirmedPage);
#endif
	
	// Sign In
	SignInPage->Button_SignIn->OnClicked.AddDynamic(this, &USignInOverlay::SignInButtonClicked);
	SignInPage->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	SignInPage->Button_Quit->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignQuitPage);
	PortalManager->SignInStatusMessageDelegate.AddDynamic(SignInPage, &USignInPage::UpdateStatusMessage);

	// Sign In Quit
	SignInQuitPage->Button_OK->OnClicked.AddDynamic(PortalManager, &UPortalManager::QuitGame);
	SignInQuitPage->Button_Cancel->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);

	// Sign Up
	SignUpPage->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::SignUpButtonClicked);
	SignUpPage->Button_Back->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	PortalManager->SignUpStatusMessageDelegate.AddDynamic(SignUpPage, &USignUpPage::UpdateStatusMessage);
	PortalManager->OnSignUpSucceededDelegate.AddDynamic(this, &USignInOverlay::OnSignUpSucceeded);

	// confirm Sign Up
	ConfirmSignUpPage->Button_Confirm->OnClicked.AddDynamic(this, &USignInOverlay::ConfirmSignUpButtonClicked);
	ConfirmSignUpPage->Button_Back->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	PortalManager->OnConfirmSucceededDelegate.AddDynamic(this, &USignInOverlay::OnConfirmSucceeded);
	PortalManager->ConfirmStatusMessageDelegate.AddDynamic(ConfirmSignUpPage, &UConfirmSignUpPage::UpdateStatusMessage);

	// Success Confirmed 
	SuccessConfirmedPage->Button_OK->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
}

void USignInOverlay::ShowSignInPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(SignInPage);
}

void USignInOverlay::ShowSignQuitPage()
{
	check(IsValid(WidgetSwitcher) && IsValid(SignInQuitPage))
	WidgetSwitcher->SetActiveWidget(SignInQuitPage);
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
	ConfirmSignUpPage->Button_Confirm->SetIsEnabled(false);
	PortalManager->Confirm(ConfirmationCode);
}

void USignInOverlay::OnSignUpSucceeded()
{
	SignUpPage->ClearTextBoxes();
	ConfirmSignUpPage->TextBlock_Destination->SetText(FText::FromString(PortalManager->LastSignUpResponse.CodeDeliveryDetails.Destination));
	ShowConfirmSignUpPage();
}

void USignInOverlay::OnConfirmSucceeded()
{
	ConfirmSignUpPage->ClearTextBoxes();
	ShowSuccessConfirmedPage();
}
