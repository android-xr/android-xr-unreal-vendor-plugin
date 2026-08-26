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

using System.IO;
using EpicGames.Core;

namespace UnrealBuildTool.Rules
{
    public class AndroidXRTrackingSubsystem : ModuleRules
    {
        public AndroidXRTrackingSubsystem(ReadOnlyTargetRules Target) : base(Target)
        {
            PublicIncludePathModuleNames.AddRange(
                new string[]
                {
                    "AndroidXR",
                }
            );

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "AndroidXR",
                    "AndroidPermission",
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "OpenXRHMD",
                    "RenderCore"
                }
            );
        }
    }
}
