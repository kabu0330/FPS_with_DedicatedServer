// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/ConfirmSignUpPage.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UConfirmSignUpPage::NativeConstruct()
{
	Super::NativeConstruct();
	TextBox_ConfirmationCode->OnTextChanged.AddDynamic(this, &UConfirmSignUpPage::UpdateConfirmButtonState);
	Button_Confirm->SetIsEnabled(false);
	
	if (TextBox_ConfirmationCode)
	{
		TextBox_ConfirmationCode->SetKeyboardFocus();
	}
}

void UConfirmSignUpPage::UpdateConfirmButtonState(const FText& Text)
{
	const FRegexPattern SixDigitsPattern(TEXT(R"(^\d{6}$)"));
	FRegexMatcher SixDigitsMatcher(SixDigitsPattern, Text.ToString());

	const bool bValidConfirmationCode = SixDigitsMatcher.FindNext();
	Button_Confirm->SetIsEnabled(bValidConfirmationCode);
	if (!bValidConfirmationCode)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("입력하신 이메일 계정에 발송된 6자리 숫자를 입력해주세요.")));
	}
	else
	{
		TextBlock_StatusMessage->SetText(FText::FromString(""));
	}
}

void UConfirmSignUpPage::ClearTextBoxes()
{
	TextBox_ConfirmationCode->SetText(FText::GetEmpty());
	TextBlock_StatusMessage->SetText(FText::GetEmpty());
	TextBlock_Destination->SetText(FText::GetEmpty());
}

void UConfirmSignUpPage::UpdateStatusMessage(const FString& Message, bool bShouldResetWidget)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));
	if (bShouldResetWidget)
	{
		//Button_Confirm->SetIsEnabled(true);
	}
}

