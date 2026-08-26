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

using EpicGames.Core;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace UnrealBuildTool.Rules
{
    public class AndroidXR : ModuleRules
    {
        struct AndroidXRPluginInfo
        {
            public string PluginName;
            public string[] HardwareFeatures;
            public int SpatialSDKLevel;
        }

        private static readonly string SpatialFeatureRequiredKey = "bRequireSpatialSDK";
        private static readonly string AndroidXRSettingsKey = "/Script/AndroidXREditor.AndroidXRRuntimeManifestSettings";
        private static readonly string RequiredHardwareFeatureKey = "RequiredHardwareFeatures";
        private static readonly string RegistererdHardwareFeaturesKey = "RegisteredHardwareFeatures";
        private static readonly string RegisteredSDKLevelsKey = "RegisteredSpatialSDKLevels";
        private static readonly string AndroidSpatialFeatureName = "android.software.xr.api.SPATIAL";

        public AndroidXR(ReadOnlyTargetRules Target) : base(Target)
        {
            PublicIncludePathModuleNames.AddRange(
                new string[]
                {
                    "OpenXR"
                }
            );

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "RenderCore",
                }
            );

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "OpenXRHMD",
                }
            );

            if (Target.Platform == UnrealTargetPlatform.Android)
            {
                string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                // AndroidXR_APL.xml adds android-specific build rules, like making modications to
                // the generated AndroidManifest.xml and copying libraries into the apk.
                AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "AndroidXR_APL.xml"));
                UpdateAndroidXRSpatialFeature(this);
            }
        }

        public static void UpdateAndroidXRSpatialFeature(ModuleRules Rules, List<string> Permissions = null)
        {
            if (Rules.Target.Platform != UnrealTargetPlatform.Android)
            {
                return;
            }
            var APLFile = new FileInfo($"{Path.Combine(Path.Combine(Rules.PluginDirectory, "Intermediate"), Rules.Name)}_APL.xml");
            if (APLFile.Exists)
            {
                File.Delete(APLFile.FullName);
            }
            var Content = new StringBuilder();
            Content.AppendLine("<root xmlns:android=\"http://schemas.android.com/apk/res/android\"><androidManifestUpdates>");
            Content.AppendLine(GetManifestStringsForFeatureInfo(Rules));
            if (Permissions != null)
            {
                //Add permissions if enabled:
                Content.AppendLine("<setBoolFromProperty result = \"bEnablePermission\" ini = \"Engine\" section = \"/Script/AndroidXREditor.AndroidXRRuntimeManifestSettings\" property = \"bEnablePermissions\" default = \"true\" />");
                Content.AppendLine("<if condition=\"bEnablePermission\"><true>");
                foreach (var Permission in Permissions)
                {
                    Content.AppendLine($"<addPermission android:name=\"{Permission}\"/>\r\n");
                }
                Content.AppendLine("</true></if>");
            }
            Content.AppendLine("</androidManifestUpdates></root>");
            File.WriteAllText(APLFile.FullName, Content.ToString());
            Rules.AdditionalPropertiesForReceipt.Add("AndroidPlugin", APLFile.FullName);
        }

        private static (HashSet<string>, bool) GetUserSettings(ModuleRules Module)
        {
            DirectoryReference ProjectDirectory = Module.Target.ProjectFile?.Directory;
            ConfigHierarchy Configs = ConfigCache.ReadHierarchy(ConfigHierarchyType.Engine, ProjectDirectory, Module.Target.Platform);
            if (Configs == null)
            {
                return ([], false);
            }
            var Section = Configs.FindSection(AndroidXRSettingsKey);
            if (Section == null)
            {
                return ([], false);
            }
            var RequiredFeatures = new HashSet<string>();
            var RegisteredSDKLevels = new HashSet<string>();

            if (Section.TryGetValues(RequiredHardwareFeatureKey, out var RequiredFeaturesStr))
            {
                RequiredFeatures = [.. RequiredFeaturesStr];
            }
            var SpatialFeatureRequired = false;
            if (Section.TryGetValue(SpatialFeatureRequiredKey, out var SpatialFeatureRequiredStr))
            {
                SpatialFeatureRequired = bool.Parse(SpatialFeatureRequiredStr);
            }
            return (RequiredFeatures, SpatialFeatureRequired);
        }

        public static string GetManifestStringsForFeatureInfo(ModuleRules Module)
        {
            var AndroidXRPluginInfo = UnrealBuildTool.Plugins.GetPlugin("AndroidXR");
            if (AndroidXRPluginInfo == null)
            {
                throw new System.NullReferenceException("AndroidXR Plugin was not found. Please enable the plugin");
            }
            var AndroidXRConfigFileReference = new FileReference($"{AndroidXRPluginInfo.Directory.FullName}{Path.DirectorySeparatorChar}Config{Path.DirectorySeparatorChar}DefaultAndroidXR.ini");
            var AndroidXRConfigFile = new ConfigFile(AndroidXRConfigFileReference);
            var AndroidXRConfigHierarchy = new ConfigHierarchy([AndroidXRConfigFile]);
            var (RequiredFeatures, IsSpatialFeatureRequired) = GetUserSettings(Module);
            if (!AndroidXRConfigHierarchy.TryGetValuesGeneric<AndroidXRPluginInfo>("/Script/AndroidXREditor.AndroidXRPluginManifestSettings", "PluginFeatures", out var Plugins))
            {
                throw new System.Exception("Unable to parse AndroidXR Config");
            }
            if (Plugins == null)
            {
                throw new System.Exception("AndroidXR Plugin Info was null");
            }
            var RegisteredFeatures = new HashSet<string>();
            var SpatialVersion = 0;
            foreach (var Plugin in Plugins)
            {
                if (Plugin.PluginName != Module.Name)
                {
                    continue;
                }
                SpatialVersion = Plugin.SpatialSDKLevel;
                RegisteredFeatures.UnionWith(Plugin.HardwareFeatures);
            }
            var MaxSDKWrittenFile = new FileInfo($"{AndroidXRPluginInfo.Directory.FullName}{Path.DirectorySeparatorChar}Intermediate{Path.DirectorySeparatorChar}SpatialSDKVersion.dat");
            var SpatialSDKWritten = 0;
            if (File.Exists(MaxSDKWrittenFile.FullName))
            {
                var SDKText = File.ReadAllText(MaxSDKWrittenFile.FullName);
                if (int.TryParse(SDKText, out SpatialSDKWritten))
                {
                    SpatialVersion = System.Math.Max(SpatialSDKWritten, SpatialVersion);
                }
            }
            File.WriteAllText(MaxSDKWrittenFile.FullName, SpatialVersion.ToString());
            var Builder = new StringBuilder();
            Builder.AppendLine($"<addFeature android:name=\"{AndroidSpatialFeatureName}\" android:version=\"{SpatialVersion}\" android:required=\"{(IsSpatialFeatureRequired ? "true" : "false")}\"/>");
            foreach (var Feature in RegisteredFeatures)
            {
                Builder.AppendLine($"<addFeature android:name=\"{Feature}\" android:required=\"{(RequiredFeatures.Contains(Feature) ? "true" : "false")}\"/>");
            }
            return Builder.ToString();
        }
    }
}
