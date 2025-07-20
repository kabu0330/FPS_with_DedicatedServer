// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignUpPage.generated.h"

class UTextBlock;
class UButton;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API USignUpPage : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_UserName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Password;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_ConfirmPassword;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Email;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_SignUp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Back;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PasswordStatusMessage;

	UFUNCTION()
	void UpdateStatusMessage(const FString& Message, bool bShouldResetWidget);

	UFUNCTION()
	void ClearTextBoxes();

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void UpdateSignUpButtonState(const FText& Text);

	UFUNCTION()
	void UpdatePasswordStatus(const FText& Text);
	bool IsValidPassword();

	bool IsValidEmail(const FString& Email);
	bool IsStrongPassword(const FString& Password, FString& StatusMessage);
};
