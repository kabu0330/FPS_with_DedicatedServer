// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignUpPage.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void USignUpPage::NativeConstruct()
{
	Super::NativeConstruct();

	TextBox_UserName->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_Password->OnTextChanged.AddDynamic(this, &USignUpPage::UpdatePasswordStatus);
	TextBox_ConfirmPassword->OnTextChanged.AddDynamic(this, &USignUpPage::UpdatePasswordStatus);
	TextBox_Email->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	Button_SignUp->SetIsEnabled(false);
}

void USignUpPage::UpdateSignUpButtonState(const FText& Text)
{
	constexpr int UserNameLength = 2;
	const bool bIsUsernameValid = !TextBox_UserName->GetText().ToString().IsEmpty() && TextBox_UserName->GetText().ToString().Len() >= UserNameLength;

	const bool bIsValidEmail = IsValidEmail(TextBox_Email->GetText().ToString());
	const bool bIsEmailEmpty = !TextBox_Email->GetText().ToString().IsEmpty();
	
	if (false == bIsUsernameValid)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("User Name은 두 글자 이상이어야 합니다.")));
	}
	else if (false == bIsValidEmail && false == bIsEmailEmpty)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("유효하지 않은 이메일 형식입니다.")));
	}
	else
	{
		TextBlock_StatusMessage->SetText(FText::GetEmpty());
	}

	Button_SignUp->SetIsEnabled(bIsUsernameValid && IsValidPassword() && bIsValidEmail);
}

bool USignUpPage::IsValidEmail(const FString& Email)
{
	// FRegex : Regular Expression : 정규 표현식을 미리 컴파일하여 효율적인 검색을 준비
	// FRegexPattern : 규칙을 정의하고 한 번만 컴파일하여 객체를 생성, 재사용하여 패턴 검사
	const FRegexPattern EmailPattern(TEXT(R"((^[^\s@]+@[^\s@]+\.[^\s@]{2,}$))"));
	FRegexMatcher EmailMatcher(EmailPattern, Email);
	return EmailMatcher.FindNext();
}

void USignUpPage::UpdatePasswordStatus(const FText& Text)
{
	const bool bArePasswordsEqual = TextBox_Password->GetText().ToString() == TextBox_ConfirmPassword->GetText().ToString();
	constexpr int PasswordLength = 6;
	const bool bIsPasswordLongEnough = TextBox_Password->GetText().ToString().Len() >= PasswordLength;
	const bool bIsValidConfirmPassword = !TextBox_ConfirmPassword->GetText().ToString().IsEmpty();
	
	FString StatusMessage;
	const bool bIsStrongPassword = IsStrongPassword(TextBox_Password->GetText().ToString(), StatusMessage);
	if (false == bIsStrongPassword)
	{
		TextBlock_PasswordStatusMessage->SetText(FText::FromString(StatusMessage));
	}
	else if (false == bIsPasswordLongEnough)
	{
		const FString Message = FString::Printf(TEXT("비밀번호는 최소 %d자리 이상이어야 합니다."), PasswordLength);
		TextBlock_PasswordStatusMessage->SetText(FText::FromString(Message));
	}
	else if (false == bArePasswordsEqual && bIsValidConfirmPassword)
	{
		TextBlock_PasswordStatusMessage->SetText(FText::FromString(TEXT("입력한 비밀번호와 일치하지 않습니다.")));
	}
	else if (false == bArePasswordsEqual && false == bIsValidConfirmPassword)
	{
		TextBlock_PasswordStatusMessage->SetText(FText::FromString(TEXT("사용 가능한 비밀번호입니다. 비밀번호 확인을 입력해주세요.")));
	}
	else
	{
		TextBlock_PasswordStatusMessage->SetText(FText::FromString(TEXT("사용 가능한 비밀번호입니다.")));
	}
}

bool USignUpPage::IsValidPassword()
{
	const bool bArePasswordsEqual = TextBox_Password->GetText().ToString() == TextBox_ConfirmPassword->GetText().ToString();
	constexpr int PasswordLength = 6;
	const bool bIsPasswordLongEnough = TextBox_Password->GetText().ToString().Len() >= PasswordLength;
	
	FString StatusMessage;
	const bool bIsStrongPassword = IsStrongPassword(TextBox_Password->GetText().ToString(), StatusMessage);
	if (false == bIsStrongPassword && false == bIsPasswordLongEnough && false == bArePasswordsEqual)
	{
		return false;
	}
	return true;
}

bool USignUpPage::IsStrongPassword(const FString& Password, FString& StatusMessage)
{
	const FRegexPattern NumberPattern(TEXT(R"(\d)")); // 최소 숫자 하나 이상
	const FRegexPattern SpecialCharPattern(TEXT(R"([^\w\s])")); // 최소 특수문자 하나 이상
	const FRegexPattern UpperCasePattern(TEXT(R"([A-Z])")); // 최소 대문자 하나 이상
	const FRegexPattern LowerCasePattern(TEXT(R"([a-z])")); // 최소 소문자 하나 이상

	FRegexMatcher NumberMatcher(NumberPattern, Password);
	FRegexMatcher SpecialCharMatcher(SpecialCharPattern, Password);
	FRegexMatcher UpperCaseMatcher(UpperCasePattern, Password);
	FRegexMatcher LowerCaseMatcher(LowerCasePattern, Password);

	if (false == NumberMatcher.FindNext())
	{
		StatusMessage = TEXT("비밀번호에 숫자를 입력해주세요.");
		return false;
	}
	if (false == SpecialCharMatcher.FindNext())
	{
		StatusMessage = TEXT("비밀번호에 특수 문자를 하나 이상 입력해주세요.");
		return false;
	}
	if (false == UpperCaseMatcher.FindNext())
	{
		StatusMessage = TEXT("비밀번호에 영문 대문자를 하나 이상 입력해주세요.");
		return false;
	}
	if (false == LowerCaseMatcher.FindNext())
	{
		StatusMessage = TEXT("비밀번호에 영문 소문자를 하나 이상 입력해주세요.");
		return false;
	}
	return true;
}
