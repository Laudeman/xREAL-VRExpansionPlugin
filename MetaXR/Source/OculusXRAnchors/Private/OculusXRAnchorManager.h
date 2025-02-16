/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/
#pragma once

#include "CoreMinimal.h"
#include "OculusXRAnchorComponent.h"
#include "OculusXRHMDPrivate.h"

namespace OculusXRAnchors
{
	struct OCULUSXRANCHORS_API FOculusXRAnchorManager
	{
		static EOculusXRAnchorResult::Type CreateAnchor(const FTransform& InTransform, uint64& OutRequestId, const FTransform& CameraTransform);
		static EOculusXRAnchorResult::Type DestroySpace(uint64 Space);
		static EOculusXRAnchorResult::Type SetSpaceComponentStatus(uint64 Space, EOculusXRSpaceComponentType SpaceComponentType, bool Enable, float Timeout, uint64& OutRequestId);
		static EOculusXRAnchorResult::Type GetSpaceComponentStatus(uint64 Space, EOculusXRSpaceComponentType SpaceComponentType, bool& OutEnabled, bool& OutChangePending);
		static EOculusXRAnchorResult::Type SaveAnchor(uint64 Space, EOculusXRSpaceStorageLocation StorageLocation, EOculusXRSpaceStoragePersistenceMode StoragePersistenceMode, uint64& OutRequestId);
		static EOculusXRAnchorResult::Type SaveAnchorList(const TArray<uint64>& Spaces, EOculusXRSpaceStorageLocation StorageLocation, uint64& OutRequestId);
		static EOculusXRAnchorResult::Type EraseAnchor(uint64 AnchorHandle, EOculusXRSpaceStorageLocation StorageLocation, uint64& OutRequestId);
		static EOculusXRAnchorResult::Type QuerySpaces(const FOculusXRSpaceQueryInfo& QueryInfo, uint64& OutRequestId);
		static EOculusXRAnchorResult::Type ShareSpaces(const TArray<uint64>& Spaces, const TArray<uint64>& UserIds, uint64& OutRequestId);
		static EOculusXRAnchorResult::Type GetSpaceScenePlane(uint64 Space, FVector& OutPos, FVector& OutSize);
		static EOculusXRAnchorResult::Type GetSpaceSceneVolume(uint64 Space, FVector& OutPos, FVector& OutSize);
		static EOculusXRAnchorResult::Type GetSpaceSemanticClassification(uint64 Space, TArray<FString>& OutSemanticClassification);

		static void OnPollEvent(ovrpEventDataBuffer* EventDataBuffer, bool& EventPollResult);
	};
}


