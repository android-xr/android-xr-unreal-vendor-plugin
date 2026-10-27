/* Copyright 2026 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "HandInteraction.h"
#include "AndroidXRTypeConversions.h"
#include "IOpenXRHMD.h"
#include "IXRTrackingSystem.h"
#include "IOpenXRHMDModule.h"

DEFINE_LOG_CATEGORY(LogHandInteraction);

struct FActionCreateInfo
{
	FString InteractionName;
	FString ActionName;
	XrActionType ActionType;
};

struct FHandActionCreateInfo
{
	EControllerHand HandEnum{};
	TSet<TWeakObjectPtr<UInputAction>> InputActions{};
	float DeadZone{};
	bool bNormalize{};
};

static constexpr char kProfilePath[] = "/interaction_profiles/ext/hand_interaction_ext";
const FString kHandNames[] =
{
	"left",
	"right"
};

inline int32 ControllerHandToIndex(EControllerHand Hand)
{
	if (Hand != EControllerHand::Right)
	{
		return 0;
	}
	return 1;
}

FHandInteraction::FHandInteraction()
{
	ActionStateIndexesByHand.SetNum(2);
}

void FHandInteraction::StartupModule()
{
	IModularFeatures::Get().RegisterModularFeature(IMotionController::GetModularFeatureName(), static_cast<IMotionController*>(this));
	IHandInteractionModule::StartupModule();
	RegisterOpenXRExtensionModularFeature();
	RefreshMotionControllerSources();
}

void FHandInteraction::ShutdownModule()
{
	Cleanup();
	IModularFeatures::Get().UnregisterModularFeature(IMotionController::GetModularFeatureName(), static_cast<IMotionController*>(this));
	UnregisterOpenXRExtensionModularFeature();
	IHandInteractionModule::ShutdownModule();
}

void FHandInteraction::PostCreateInstance(XrInstance InInstance)
{
	Instance = InInstance;
}

void FHandInteraction::PostCreateSession(XrSession InSession)
{
	Session = InSession;
}

bool FHandInteraction::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
	OutExtensions.Add(XR_EXT_HAND_INTERACTION_EXTENSION_NAME);
	return true;
}

bool FHandInteraction::GetOptionalExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
	OutExtensions.Add(XR_EXT_PALM_POSE_EXTENSION_NAME);
	return true;
}

bool FHandInteraction::GetInteractionProfiles(XrInstance InInstance, TArray<FString>& OutKeyPrefixes, TArray<XrPath>& OutPaths, TArray<bool>& OutHasHaptics)
{
	auto Result = xrStringToPath(InInstance, kProfilePath, &ProfilePath);
	if (XR_UNQUALIFIED_SUCCESS(Result))
	{
		OutKeyPrefixes.Add("HandInteraction");
		OutHasHaptics.Add(false);
		OutPaths.Add(ProfilePath);
		return true;
	}

	UE_LOG(LogHandInteraction, Error,
		TEXT("xrStringToPath failed for %s with error %s"),
		UTF8_TO_TCHAR(kProfilePath),
		OpenXRResultToString(Result));

	return false;
}

void FHandInteraction::AttachActionSets(TSet<XrActionSet>& OutActionSets)
{
	Cleanup();
	CreateInteractions();
	OutActionSets.Append(ActionSets);
}

void FHandInteraction::GetActiveActionSetsForSync(TArray<XrActiveActionSet>& OutActiveSets)
{
	for (const auto& ActionSet : ActionSets)
	{
		OutActiveSets.Add({
			.actionSet = ActionSet,
			.subactionPath = XR_NULL_PATH
			});
	}
}

void FHandInteraction::PostSyncActions(XrSession InSession)
{
	XrActionStateGetInfo ActionStateGetInfo
	{
		.type = XR_TYPE_ACTION_STATE_GET_INFO
	};

	for (auto& PoseActionState : PoseActionStates)
	{
		ActionStateGetInfo.action = PoseActionState.Action;
		xrGetActionStatePose(InSession, &ActionStateGetInfo, &PoseActionState.State);
	}

	for (auto& FloatActionState : FloatActionStates)
	{
		ActionStateGetInfo.action = FloatActionState.Action;
		xrGetActionStateFloat(InSession, &ActionStateGetInfo, &FloatActionState.State);
	}

	for (auto& BooleanActionState : BooleanActionStates)
	{
		ActionStateGetInfo.action = BooleanActionState.Action;
		xrGetActionStateBoolean(InSession,
			&ActionStateGetInfo, &BooleanActionState.State);
	}
}

void FHandInteraction::UpdateDeviceLocations(XrSession InSession, XrTime DisplayTime, XrSpace TrackingSpace)
{
	for (auto& PoseActionState : PoseActionStates)
	{
		if (!!PoseActionState.State.isActive)
		{
			xrLocateSpace(PoseActionState.Space,
				TrackingSpace, DisplayTime, &PoseActionState.Location);
		}
	}
}

FName FHandInteraction::GetMotionControllerDeviceTypeName() const
{
	return "HandInteractionMotionController";
}

ETrackingStatus FHandInteraction::GetControllerTrackingStatus(const int32 ControllerIndex, const FName MotionSource) const
{
	auto HandInteractionPtr = MotionSourceNameMap.Find(MotionSource);
	if (!HandInteractionPtr)
	{
		UE_LOG(LogHandInteraction, Error, TEXT("Unable to find a motion source of the name %s"), *MotionSource.ToString());
		return ETrackingStatus::NotTracked;
	}
	auto Hand = HandInteractionPtr->Key;
	auto Interaction = HandInteractionPtr->Value;
	if (GetInteractionReady(Hand, Interaction))
	{
		return ETrackingStatus::Tracked;
	}
	return ETrackingStatus::NotTracked;
}

bool FHandInteraction::GetControllerOrientationAndPosition(const int32 ControllerIndex, const FName MotionSource, FRotator& OutOrientation, FVector& OutPosition, float WorldToMetersScale) const
{
	auto HandInteractionPtr = MotionSourceNameMap.Find(MotionSource);
	if (!HandInteractionPtr)
	{
		UE_LOG(LogHandInteraction, Error, TEXT("Unable to find a motion source of the name %s"), *MotionSource.ToString());
		return false;
	}
	auto Hand = HandInteractionPtr->Key;
	auto Interaction = HandInteractionPtr->Value;
	FTransform InteractionTransform{};
	if (GetInteractionPose(Hand, Interaction, InteractionTransform, WorldToMetersScale))
	{
		OutOrientation = InteractionTransform.Rotator();
		OutPosition = InteractionTransform.GetLocation();
		return true;
	}
	return false;
}

FString GetInteractionString(EOpenXRHandInteractionType InteractionType)
{
	switch (InteractionType)
	{
	case EOpenXRHandInteractionType::Aim:
		return TEXT("Aim");
	case EOpenXRHandInteractionType::Grip:
		return TEXT("Grip");
	case EOpenXRHandInteractionType::Pinch:
		return TEXT("Pinch");
	case EOpenXRHandInteractionType::Poke:
		return TEXT("Poke");
	case EOpenXRHandInteractionType::Palm:
		return TEXT("Palm");
	case EOpenXRHandInteractionType::GripSurface:
		return TEXT("GripSurface");
	default:
		return TEXT("Invalid");
	}
}

FString GetHandString(EControllerHand Hand)
{
	switch (Hand)
	{
	case EControllerHand::Left:
		return TEXT("Left");
	case EControllerHand::Right:
		return TEXT("Right");
	default:
		return TEXT("Left");
	}
}

void FHandInteraction::PopulateMotionControllerSources(EControllerHand Hand)
{
	auto Count = static_cast<int32>(EOpenXRHandInteractionType::Count);
	for (auto i = 0; i < Count; i++)
	{
		auto Interaction = static_cast<EOpenXRHandInteractionType>(i);
		FUtf8StringBuilderBase NameBuilder;
		// NameBuilder.Append("HandInteraction_");
		NameBuilder.Append(GetHandString(Hand));
		// NameBuilder.Append("_");
		NameBuilder.Append(GetInteractionString(Interaction));
		MotionSourceNameMap.Add(FName{ NameBuilder }, { Hand, Interaction });
	}
}

void FHandInteraction::EnumerateSources(TArray<FMotionControllerSource>& SourcesOut) const
{
	/* Algo::Transform(MotionSourceNameMap, SourcesOut, [](const auto& MotionSourceNamePair) {
		 return FMotionControllerSource{ MotionSourceNamePair.Key };
	 });*/
}

bool FHandInteraction::GetInteractionPose(EControllerHand Hand, EOpenXRHandInteractionType InteractionType, FTransform& InteractionPose) const
{
	if (!GEngine || !GEngine->XRSystem.IsValid())
	{
		return false;
	}

	return GetInteractionPose(Hand, InteractionType, InteractionPose, GEngine->XRSystem.Get()->GetWorldToMetersScale());
}

bool FHandInteraction::GetInteractionPose(EControllerHand Hand,
	EOpenXRHandInteractionType InteractionType,
	FTransform& InteractionPose, float WorldToMetersScale) const
{
	auto HandIndex = ControllerHandToIndex(Hand);
	if (HandIndex >= 0)
	{
		if (auto HandInteraction = ActionStateIndexesByHand[HandIndex].Find(InteractionType))
		{
			if (auto ActionStateIndex = HandInteraction->Find(XR_ACTION_TYPE_POSE_INPUT))
			{
				const auto& ActionState = PoseActionStates[*ActionStateIndex];
				if (!!ActionState.State.isActive)
				{
					auto BitMask = XR_SPACE_LOCATION_POSITION_VALID_BIT |
						XR_SPACE_LOCATION_ORIENTATION_VALID_BIT;
					if ((ActionState.Location.locationFlags & BitMask) == BitMask)
					{
						InteractionPose = ToFTransform(ActionState.Location.pose, WorldToMetersScale);
						return true;
					}
				}
			}
		}
	}
	return false;
}

void FHandInteraction::RefreshMotionControllerSources()
{
	MotionSourceNameMap.Reset();
	PopulateMotionControllerSources(EControllerHand::Left);
	PopulateMotionControllerSources(EControllerHand::Right);
}

bool FHandInteraction::GetInteractionValue(EControllerHand Hand,
	EOpenXRHandInteractionType InteractionType,
	float& InteractionValue) const
{
	auto HandIndex = ControllerHandToIndex(Hand);
	if (HandIndex >= 0)
	{
		if (auto HandInteraction = ActionStateIndexesByHand[HandIndex].Find(InteractionType))
		{
			if (auto ActionStateIndex = HandInteraction->Find(XR_ACTION_TYPE_FLOAT_INPUT))
			{
				const auto& ActionState = FloatActionStates[*ActionStateIndex];
				if (!!ActionState.State.isActive)
				{
					InteractionValue = ActionState.State.currentState;
					return true;
				}
			}
		}
	}
	return false;
}

bool FHandInteraction::GetInteractionReady(EControllerHand Hand,
	EOpenXRHandInteractionType InteractionType) const
{
	auto HandIndex = ControllerHandToIndex(Hand);
	if (HandIndex >= 0)
	{
		if (auto HandInteraction = ActionStateIndexesByHand[HandIndex].Find(InteractionType))
		{
			if (auto ActionStateIndex = HandInteraction->Find(XR_ACTION_TYPE_BOOLEAN_INPUT))
			{
				const auto& ActionState = BooleanActionStates[*ActionStateIndex];
				if (!!ActionState.State.isActive)
				{
					return !!ActionState.State.currentState;
				}
			}
		}
	}
	return false;
}

void FHandInteraction::Cleanup()
{
	PoseActionStates.Empty();
	FloatActionStates.Empty();
	BooleanActionStates.Empty();

	for (auto& Hand : ActionStateIndexesByHand)
	{
		Hand.Empty();
	}

	for (const auto& ActionSet : ActionSets)
	{
		xrDestroyActionSet(ActionSet);
	}
	ActionSets.Empty();
}

bool FHandInteraction::CreateAction(FString ActionName,
	FString LocalizedActionName,
	XrActionType ActionType,
	XrActionSet ActionSet,
	XrAction& CreatedAction)
{
	XrActionCreateInfo ActionCreateInfo
	{
		.type = XR_TYPE_ACTION_CREATE_INFO,
		.actionType = ActionType
	};

	FCStringAnsi::Strncpy(ActionCreateInfo.actionName,
		TCHAR_TO_UTF8(*ActionName), XR_MAX_ACTION_NAME_SIZE);
	FCStringAnsi::Strncpy(ActionCreateInfo.localizedActionName,
		TCHAR_TO_UTF8(*LocalizedActionName), XR_MAX_LOCALIZED_ACTION_NAME_SIZE);

	auto Result = xrCreateAction(ActionSet, &ActionCreateInfo, &CreatedAction);
	if (XR_UNQUALIFIED_SUCCESS(Result))
	{
		return true;
	}

	UE_LOG(LogHandInteraction, Error,
		TEXT("xrCreateAction failed for %s with error %s"),
		*ActionName,
		OpenXRResultToString(Result));

	return false;
}

void FHandInteraction::CreateActionForHand(XrActionSet ActionSet,
	FActionStateIndexByActionType& ActionStateIndexes,
	EOpenXRHandInteractionType InteractionType,
	const FHandActionCreateInfo& HandActionCreateInfo,
	const FActionCreateInfo& ActionCreateInfo,
	TArray<XrActionSuggestedBinding>& SuggestedBindings)
{
	auto HandIndex = ControllerHandToIndex(HandActionCreateInfo.HandEnum);
	auto HandLower = kHandNames[HandIndex].ToLower();
	auto HandUpper = HandLower;
	HandUpper[0] = FChar::ToUpper(HandUpper[0]);

	auto InteractionNameLower = ActionCreateInfo.InteractionName.ToLower();
	auto InteractionNameUpper = InteractionNameLower;
	InteractionNameUpper[0] = FChar::ToUpper(InteractionNameUpper[0]);

	auto ActionNameLower = ActionCreateInfo.ActionName.ToLower();
	auto ActionNameUpper = ActionNameLower;
	ActionNameUpper[0] = FChar::ToUpper(ActionNameUpper[0]);

	auto PathString = FString::Printf(TEXT("/user/hand/%s/input/%s/%s"),
		*HandLower, *InteractionNameLower, *ActionNameLower);

	XrPath Path;
	auto Result = xrStringToPath(Instance, TCHAR_TO_UTF8(*PathString), &Path);
	if (!XR_UNQUALIFIED_SUCCESS(Result))
	{
		UE_LOG(LogHandInteraction, Error,
			TEXT("xrStringToPath failed for %s with error %s"),
			*PathString,
			OpenXRResultToString(Result));
		return;
	}

	UE_LOG(LogHandInteraction, Display, TEXT("Resolved path %s"), *PathString);

	XrAction Action;
	if (!CreateAction(
		FString::Printf(TEXT("handinteraction_%s_%s_%s"),
			*HandLower, *InteractionNameLower, *ActionNameLower),
		FString::Printf(TEXT("HandInteraction_%s_%s_%s"),
			*HandUpper, *InteractionNameUpper, *ActionNameUpper),
		ActionCreateInfo.ActionType,
		ActionSet,
		Action))
	{
		return;
	}

	UE_LOG(LogHandInteraction, Display,
		TEXT("Created action %s_%s_%s"),
		*HandLower,
		*InteractionNameLower,
		*ActionNameLower);

	switch (ActionCreateInfo.ActionType)
	{
	case XR_ACTION_TYPE_POSE_INPUT:
	{
		XrActionSpaceCreateInfo CreateInfo
		{
			.type = XR_TYPE_ACTION_SPACE_CREATE_INFO,
			.action = Action,
			.poseInActionSpace = ToXrPose(FTransform::Identity)
		};

		XrSpace Space;
		Result = xrCreateActionSpace(Session, &CreateInfo, &Space);
		if (XR_UNQUALIFIED_SUCCESS(Result))
		{
			UE_LOG(LogHandInteraction, Display,
				TEXT("Created space for action %s_%s_%s"),
				*HandLower,
				*InteractionNameLower,
				*ActionNameLower);

			ActionStateIndexes.Add(XR_ACTION_TYPE_POSE_INPUT, PoseActionStates.Num());
			PoseActionStates.Add(HandInteraction::FActionStatePose(Action, Space));
		}
		else
		{
			UE_LOG(LogHandInteraction, Error,
				TEXT("xrCreateActionSpace failed for %s_%s_%s with error %s"),
				*HandLower,
				*InteractionNameLower,
				*ActionNameLower,
				OpenXRResultToString(Result));
			xrDestroyAction(Action);
			return;
		}
		break;
	}
	case XR_ACTION_TYPE_FLOAT_INPUT:
	{
		// Note that input actions are mapped to the float inputs because boolean
		// inputs are always true when the action is setup and the hand is visible
		ActionStateIndexes.Add(XR_ACTION_TYPE_FLOAT_INPUT, FloatActionStates.Num());
		FloatActionStates.Add(HandInteraction::FActionStateFloat(Action));
		break;
	}
	case XR_ACTION_TYPE_BOOLEAN_INPUT:
	{
		ActionStateIndexes.Add(XR_ACTION_TYPE_BOOLEAN_INPUT, BooleanActionStates.Num());
		BooleanActionStates.Add(HandInteraction::FActionStateBoolean(Action));
		break;
	}
	default:
	{
		check(0);
		break;
	}
	}

	SuggestedBindings.Add({ Action, Path });
}

bool FHandInteraction::CreateActionSet(FString ActionSetName,
	FString LocalizedActionSetName,
	XrActionSet& CreatedActionSet)
{
	XrActionSetCreateInfo ActionSetCreateInfo{ .type = XR_TYPE_ACTION_SET_CREATE_INFO };

	FCStringAnsi::Strncpy(ActionSetCreateInfo.actionSetName,
		TCHAR_TO_UTF8(*ActionSetName), XR_MAX_ACTION_SET_NAME_SIZE);
	FCStringAnsi::Strncpy(ActionSetCreateInfo.localizedActionSetName,
		TCHAR_TO_UTF8(*LocalizedActionSetName), XR_MAX_LOCALIZED_ACTION_SET_NAME_SIZE);

	auto Result = xrCreateActionSet(Instance, &ActionSetCreateInfo, &CreatedActionSet);
	if (XR_SUCCEEDED(Result))
	{
		return true;
	}

	UE_LOG(LogHandInteraction, Error,
		TEXT("xrCreateActionSet failed for %s with error %s"),
		*ActionSetName,
		OpenXRResultToString(Result));
	return false;
}

void FHandInteraction::CreateInteraction(const FString& ActionSetName,
	EOpenXRHandInteractionType InteractionType,
	const TArray<FHandActionCreateInfo>& HandActionCreateInfos,
	const TArray<FActionCreateInfo>& ActionCreateInfos,
	TArray<XrActionSuggestedBinding>& SuggestedBindings)
{
	if (HandActionCreateInfos.Num() > 0)
	{
		auto ActionSetNameLower = ActionSetName.ToLower();
		auto ActionSetNameUpper = ActionSetNameLower;
		ActionSetNameUpper[0] = FChar::ToUpper(ActionSetNameUpper[0]);

		XrActionSet ActionSet;
		if (CreateActionSet(
			FString::Printf(TEXT("handinteraction_%s"), *ActionSetNameLower),
			FString::Printf(TEXT("HandInteraction_%s"), *ActionSetNameUpper),
			ActionSet))
		{
			UE_LOG(LogHandInteraction, Display,
				TEXT("Created action set handinteraction_%s"), *ActionSetNameLower);

			for (const auto& HandActionCreateInfo : HandActionCreateInfos)
			{
				auto& ActionStateIndexesByInteraction = ActionStateIndexesByHand
					[ControllerHandToIndex(HandActionCreateInfo.HandEnum)];

				for (const auto& ActionCreateInfo : ActionCreateInfos)
				{
					FActionStateIndexByActionType ActionIndexes;

					CreateActionForHand(ActionSet,
						ActionIndexes,
						InteractionType,
						HandActionCreateInfo,
						ActionCreateInfo,
						SuggestedBindings);

					if (!ActionIndexes.IsEmpty())
					{
						ActionStateIndexesByInteraction.
							FindOrAdd(InteractionType).Append(ActionIndexes);
					}
				}
			}

			if (!ActionStateIndexesByHand[0].IsEmpty() ||
				!ActionStateIndexesByHand[1].IsEmpty())
			{
				ActionSets.Add(ActionSet);
			}
			else
			{
				xrDestroyActionSet(ActionSet);
			}
		}
	}
}

bool FHandInteraction::CreateInteractions()
{
	TArray<XrActionSuggestedBinding> ActionSuggestedBindings;

	auto GetInputActions = [](const TSet<FSoftObjectPath>& Paths) {
		static TSet<TWeakObjectPtr<UInputAction>> Result{};
		Result.Empty();
		for (auto& Path : Paths)
		{
			Result.Add(Cast<UInputAction>(Path.TryLoad()));
		}
		return Result;
		};
	// Aim
	{
		TArray<FHandActionCreateInfo> HandActionCreateInfos;
		HandActionCreateInfos.Add(
			{
				   .HandEnum = EControllerHand::Left,
			}
			);


		HandActionCreateInfos.Add(
			{
				.HandEnum = EControllerHand::Right,
			}
			);

		CreateInteraction("aim", EOpenXRHandInteractionType::Aim,
			HandActionCreateInfos,
			{
				{ "aim", "pose", XR_ACTION_TYPE_POSE_INPUT },
				{ "aim_activate_ext", "ready_ext", XR_ACTION_TYPE_BOOLEAN_INPUT },
				{ "aim_activate_ext", "value", XR_ACTION_TYPE_FLOAT_INPUT }
			},
			ActionSuggestedBindings);
	}

	// Grip
	{

		TArray<FHandActionCreateInfo> HandActionCreateInfos;

		HandActionCreateInfos.Add(
			{
				.HandEnum = EControllerHand::Left,
			}
			);


		HandActionCreateInfos.Add(
			{
				.HandEnum = EControllerHand::Right,
			}
			);

		CreateInteraction("grip", EOpenXRHandInteractionType::Grip,
			HandActionCreateInfos,
			{
				{ "grip", "pose", XR_ACTION_TYPE_POSE_INPUT },
				{ "grasp_ext", "ready_ext", XR_ACTION_TYPE_BOOLEAN_INPUT },
				{ "grasp_ext", "value", XR_ACTION_TYPE_FLOAT_INPUT }
			},
			ActionSuggestedBindings);
	}

	// Pinch
	{
		TArray<FHandActionCreateInfo> HandActionCreateInfos;

		HandActionCreateInfos.Add(
			{
				.HandEnum = EControllerHand::Left,
			}
			);


		HandActionCreateInfos.Add(
			{
				.HandEnum = EControllerHand::Right,
			}
			);

		CreateInteraction("pinch", EOpenXRHandInteractionType::Pinch,
			HandActionCreateInfos,
			{
				{ "pinch_ext", "pose", XR_ACTION_TYPE_POSE_INPUT },
				{ "pinch_ext", "ready_ext", XR_ACTION_TYPE_BOOLEAN_INPUT },
				{ "pinch_ext", "value", XR_ACTION_TYPE_FLOAT_INPUT }
			},
			ActionSuggestedBindings);
	}

	// Poke
	{
		TArray<FHandActionCreateInfo> HandActionCreateInfos;

		HandActionCreateInfos.Add(
			{
				.HandEnum = EControllerHand::Left
			}
		);


		HandActionCreateInfos.Add(
			{
				.HandEnum = EControllerHand::Right
			}
		);

		CreateInteraction("poke", EOpenXRHandInteractionType::Poke,
			HandActionCreateInfos,
			{
				{ "poke_ext", "pose", XR_ACTION_TYPE_POSE_INPUT },
			},
			ActionSuggestedBindings);
	}

	// Palm
	if (IOpenXRHMDModule::Get().IsExtensionEnabled(XR_EXT_PALM_POSE_EXTENSION_NAME))
	{
		TArray<FHandActionCreateInfo> HandActionCreateInfos;

		HandActionCreateInfos.Add(
			{
				.HandEnum = EControllerHand::Left
			}
		);


		HandActionCreateInfos.Add(
			{
				.HandEnum = EControllerHand::Right
			}
		);

		CreateInteraction("palm", EOpenXRHandInteractionType::Palm,
			HandActionCreateInfos,
			{
				{ "palm_ext", "pose", XR_ACTION_TYPE_POSE_INPUT },
			},
			ActionSuggestedBindings);
	}

	// Grip Surface
	{
		TArray<FHandActionCreateInfo> HandActionCreateInfos;

		HandActionCreateInfos.Add(
			{
				.HandEnum = EControllerHand::Left
			}
		);


		HandActionCreateInfos.Add(
			{
				.HandEnum = EControllerHand::Right
			}
		);

		CreateInteraction("grip_surface", EOpenXRHandInteractionType::GripSurface,
			HandActionCreateInfos,
			{
				{ "grip_surface", "pose", XR_ACTION_TYPE_POSE_INPUT },
			},
			ActionSuggestedBindings);
	}

	if (ActionSuggestedBindings.Num() > 0)
	{
		XrInteractionProfileSuggestedBinding ProfileBinding
		{
			.type = XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING,
			.interactionProfile = ProfilePath,
			.countSuggestedBindings = static_cast<uint32>(ActionSuggestedBindings.Num()),
			.suggestedBindings = ActionSuggestedBindings.GetData(),
		};

		auto Result = xrSuggestInteractionProfileBindings(Instance, &ProfileBinding);
		if (XR_UNQUALIFIED_SUCCESS(Result))
		{
			UE_LOG(LogHandInteraction, Display, TEXT("Created interaction bindings"));
			return true;
		}

		UE_LOG(LogHandInteraction, Error,
			TEXT("xrSuggestInteractionProfileBindings failed with error %s"),
			OpenXRResultToString(Result));
	}
	return false;
}

IMPLEMENT_MODULE(FHandInteraction, HandInteraction);

